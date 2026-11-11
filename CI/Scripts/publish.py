"""Uploads build outputs.

Shipping -> GitHub Release (draft first; published only after every platform uploaded).
            Symbols go to the private Drive folder symbols/<platform>/.
Development -> private Drive folder dev/<platform>/, keeping the newest N builds.

gh reads GH_TOKEN and rclone reads RCLONE_CONFIG from the environment (Jenkins credentials).
"""

import argparse
import json
import os
import re

import config

DEV_KEEP = 5
BUILD_NAME = re.compile(r"-build-(\d+)")

INSTALL_NOTES = """\
### How to run
- **Windows**: unzip and run `Tenetris.exe`. If SmartScreen appears, choose *More info → Run anyway*.
- **macOS**: open the `.dmg` and copy the app anywhere. The app is not signed: right-click it → *Open*
  (on macOS 15+, use *System Settings → Privacy & Security → Open Anyway*).
"""


# --- GitHub Release ---------------------------------------------------------------------

def git(*args, check=True):
    return config.run(["git", *args], cwd=config.PROJECT_ROOT, capture=True, check=check)


def release_notes(tag, sha):
    previous = git("describe", "--tags", "--abbrev=0", "--match", "build-*", sha, check=False)
    if previous:
        log_range = f"{previous}..{sha}"
        header = f"Changes since `{previous}`"
    else:
        log_range = sha
        header = "Recent changes"

    log = git("log", "--no-merges", "--max-count=50", "--format=- %s (%h)", log_range) or "- No code changes."

    return (
        f"## {config.PROJECT_NAME} {tag}\n\n"
        f"Commit: `{sha}`\n\n"
        f"### {header}\n{log}\n\n"
        f"{INSTALL_NOTES}"
    )


def gh(*args, check=True):
    return config.run([config.tool("gh"), *args], cwd=config.PROJECT_ROOT, check=check)


def cmd_release_draft(args):
    config.CI_DIR.mkdir(parents=True, exist_ok=True)
    config.RELEASE_NOTES.write_text(release_notes(args.tag, args.sha), encoding="utf-8")
    print(config.RELEASE_NOTES.read_text(encoding="utf-8"), flush=True)

    # The tag is created on publish, pointing at the pinned commit.
    gh("release", "create", args.tag,
       "--draft",
       "--target", args.sha,
       "--title", args.title or f"Build {args.tag.removeprefix('build-')}",
       "--notes-file", config.RELEASE_NOTES)


def cmd_release_upload(args):
    manifest = read_manifest()
    gh("release", "upload", args.tag, config.DIST_DIR / manifest["game"], "--clobber")


def cmd_release_publish(args):
    gh("release", "edit", args.tag, "--draft=false", "--latest")


def cmd_release_delete(args):
    # Best effort: the draft may not exist if the build failed before it was created.
    if gh("release", "delete", args.tag, "--yes", check=False) != 0:
        print(f"WARNING: could not delete release {args.tag} (it may not exist).", flush=True)


# --- Google Drive (rclone) --------------------------------------------------------------

def drive_path(args, *parts):
    remote = args.remote or os.environ.get("RCLONE_REMOTE", "gdrive")
    root = args.root or os.environ.get("RCLONE_ROOT", config.PROJECT_NAME)
    return f"{remote}:{'/'.join([root, *parts])}"


def rclone(*args, capture=False):
    return config.run([config.tool("rclone"), *args], capture=capture)


def prune(folder, keep):
    listing = rclone("lsf", "--files-only", folder, capture=True) or ""
    builds = []
    for name in listing.splitlines():
        match = BUILD_NAME.search(name)
        if match:
            builds.append((int(match.group(1)), name))

    builds.sort(reverse=True)
    for _, name in builds[keep:]:
        rclone("deletefile", f"{folder}/{name}")


def cmd_drive_upload(args):
    manifest = read_manifest()
    platform = manifest["platform"]

    if manifest["build_type"] == "Shipping":
        if not manifest["symbols"]:
            print("No symbols to upload.", flush=True)
            return
        rclone("copy", config.DIST_DIR / manifest["symbols"], drive_path(args, "symbols", platform))
    else:
        folder = drive_path(args, "dev", platform)
        rclone("copy", config.DIST_DIR / manifest["game"], folder)
        prune(folder, args.keep)


# ----------------------------------------------------------------------------------------

def read_manifest():
    if not config.MANIFEST.is_file():
        config.fail(f"{config.MANIFEST} not found; run package.py first.")
    return json.loads(config.MANIFEST.read_text(encoding="utf-8"))


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)

    draft = sub.add_parser("release-draft", help="Create a draft release with generated notes")
    draft.add_argument("--tag", required=True)
    draft.add_argument("--sha", required=True)
    draft.add_argument("--title")
    draft.set_defaults(func=cmd_release_draft)

    for name, func, help_text in (
        ("release-upload", cmd_release_upload, "Upload this platform's game file to the draft"),
        ("release-publish", cmd_release_publish, "Publish the draft (creates the tag)"),
        ("release-delete", cmd_release_delete, "Delete the draft after a failed build"),
    ):
        command = sub.add_parser(name, help=help_text)
        command.add_argument("--tag", required=True)
        command.set_defaults(func=func)

    drive = sub.add_parser("drive-upload", help="Upload dev builds / shipping symbols to Drive")
    drive.add_argument("--remote", help="rclone remote name (default: RCLONE_REMOTE env or 'gdrive')")
    drive.add_argument("--root", help="Root folder on the remote (default: RCLONE_ROOT env or project name)")
    drive.add_argument("--keep", type=int, default=DEV_KEEP, help="Dev builds kept per platform")
    drive.set_defaults(func=cmd_drive_upload)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()

"""Runs UAT BuildCookRun for the host platform, in two steps so Jenkins shows them separately.

  compile  Stamps the build number and compiles the game and editor targets (the editor is
           needed by the cook). Starts from a clean Saved/CI folder.
  cook     Cooks with the editor built above, then stages, paks and archives together with the
           game binaries from the compile step into Saved/CI/Archive/<UE platform folder>.

Both steps must run in the same workspace, with the same build type.
"""

import argparse
import shutil

import config
import version

STEPS = ("compile", "cook")


def common_args(build_type):
    platform = config.ue_platform()
    args = [
        f"-project={config.UPROJECT}",
        f"-platform={platform}",
        f"-clientconfig={build_type}",
        "-noP4",
        "-unattended",
        "-utf8output",
    ]
    if platform == "Mac":
        # Universal binary (Apple Silicon + Intel) for the game only; the editor stays host-only.
        args.append("-clientarchitecture=arm64+x64")
    return args


STEP_ARGS = {
    # BuildCookRun always adds the project's editor target to -build.
    "compile": ["-build"],
    # No -build: reuses the binaries from the compile step.
    "cook": [
        "-cook",
        "-stage",
        "-pak",
        "-iostore",
        "-compressed",
        "-prereqs",
        "-package",
        "-archive",
        f"-archivedirectory={config.ARCHIVE_DIR}",
    ],
}


def uat_command(root, step, build_type):
    return [config.run_uat_path(root), "BuildCookRun", *common_args(build_type), *STEP_ARGS[step]]


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("step", choices=STEPS)
    config.add_build_args(parser)
    parser.add_argument("--ue-root", help="Engine root (default: UE_ROOT env or OS default)")
    parser.add_argument("--dry-run", action="store_true", help="Print the UAT command without running it")
    args = parser.parse_args()

    root = config.ue_root(args.ue_root)
    cmd = uat_command(root, args.step, args.build_type)

    if args.dry_run:
        print(" ".join(str(c) for c in cmd))
        return

    if args.step == "compile":
        # Start from a clean folder so stale files from a previous build are never shipped.
        shutil.rmtree(config.CI_DIR, ignore_errors=True)
        # Stamped before anything else; the ini is packaged during the cook step.
        version.stamp(config.DEFAULT_GAME_INI, config.version_string(args.build_number, args.build_type))
    else:
        config.ARCHIVE_DIR.mkdir(parents=True, exist_ok=True)

    config.run(cmd, cwd=config.PROJECT_ROOT)


if __name__ == "__main__":
    main()

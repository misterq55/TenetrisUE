"""Shared paths and settings for the CI scripts.

Every value is resolved in the same order: command-line argument, then environment
variable, then an OS default. Nothing here is specific to one machine.
"""

import os
import platform
import shutil
import subprocess
import sys
from pathlib import Path

PROJECT_NAME = "Tenetris"

# CI/Scripts/config.py -> project root is two levels up.
PROJECT_ROOT = Path(__file__).resolve().parents[2]
UPROJECT = PROJECT_ROOT / f"{PROJECT_NAME}.uproject"
DEFAULT_GAME_INI = PROJECT_ROOT / "Config" / "DefaultGame.ini"

# Per-build working folders (Saved/ is gitignored).
CI_DIR = PROJECT_ROOT / "Saved" / "CI"
ARCHIVE_DIR = CI_DIR / "Archive"
DIST_DIR = CI_DIR / "Dist"
MANIFEST = DIST_DIR / "manifest.json"
RELEASE_NOTES = CI_DIR / "release-notes.md"

BUILD_TYPES = ("Development", "Shipping")

_DEFAULT_UE_ROOT = {
    "Windows": r"C:\Program Files\Epic Games\UE_5.7",
    "Darwin": "/Users/Shared/Epic Games/UE_5.7",
}

# Jenkins agents on macOS often start without Homebrew on PATH.
_EXTRA_TOOL_DIRS = {
    "Darwin": ["/opt/homebrew/bin", "/usr/local/bin"],
}


def host_os():
    return platform.system()


def is_mac():
    return host_os() == "Darwin"


def ue_platform():
    """UE platform name of the machine running the script."""
    if host_os() == "Windows":
        return "Win64"
    if is_mac():
        return "Mac"
    fail(f"Unsupported host OS: {host_os()}")


def ue_root(cli_value=None):
    value = cli_value or os.environ.get("UE_ROOT") or _DEFAULT_UE_ROOT.get(host_os())
    if not value or not Path(value).is_dir():
        fail(f"UE_ROOT not found ({value!r}). Set the UE_ROOT node environment variable.")
    return Path(value)


def run_uat_path(root):
    name = "RunUAT.bat" if host_os() == "Windows" else "RunUAT.sh"
    return root / "Engine" / "Build" / "BatchFiles" / name


def tool(name, cli_value=None):
    """Path of an external CLI (gh, rclone); env override is e.g. GH_PATH / RCLONE_PATH."""
    value = cli_value or os.environ.get(f"{name.upper()}_PATH")
    if value:
        return value

    found = shutil.which(name)
    if found:
        return found

    for folder in _EXTRA_TOOL_DIRS.get(host_os(), []):
        candidate = Path(folder) / name
        if candidate.is_file():
            return str(candidate)

    fail(f"'{name}' not found. Install it or set {name.upper()}_PATH.")


def version_string(build_number, build_type):
    """Shipping builds show the plain number; everything else is marked as dev."""
    return str(build_number) if build_type == "Shipping" else f"{build_number}-dev"


def artifact_base(version):
    return f"{PROJECT_NAME}-build-{version}-{ue_platform()}"


def run(cmd, cwd=None, capture=False, check=True):
    """Runs a command, echoing it first. Exits with its code on failure when check is set."""
    printable = " ".join(f'"{c}"' if " " in str(c) else str(c) for c in cmd)
    print(f"> {printable}", flush=True)
    result = subprocess.run(
        [str(c) for c in cmd],
        cwd=str(cwd) if cwd else None,
        stdout=subprocess.PIPE if capture else None,
        text=True,
    )
    if check and result.returncode != 0:
        fail(f"Command failed with exit code {result.returncode}", result.returncode)
    if capture:
        return result.stdout.strip() if result.returncode == 0 else None
    return result.returncode


def fail(message, code=1):
    print(f"ERROR: {message}", file=sys.stderr, flush=True)
    sys.exit(code)


def add_build_args(parser):
    parser.add_argument("--build-type", choices=BUILD_TYPES, default=os.environ.get("BUILD_TYPE", "Development"))
    parser.add_argument("--build-number", default=os.environ.get("BUILD_NUMBER", "0"))

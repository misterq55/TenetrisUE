"""Turns the archived build into the distributable file plus a symbols archive.

- Win64: <name>.zip. Mac: <name>.dmg (hdiutil; a .app must never go through Python zipfile,
  it would lose symlinks and permissions).
- Development: symbols stay inside the game archive.
- Shipping: symbols are removed from the game archive and packed separately (kept private).

Writes Saved/CI/Dist/manifest.json, which publish.py reads.
"""

import argparse
import json
import shutil
import zipfile

import config


def find_build_dir():
    """BuildCookRun archives into a single platform folder (e.g. Windows/, Mac/)."""
    folders = [p for p in config.ARCHIVE_DIR.iterdir() if p.is_dir()] if config.ARCHIVE_DIR.is_dir() else []
    if len(folders) != 1:
        config.fail(f"Expected one platform folder in {config.ARCHIVE_DIR}, found {[p.name for p in folders]}")
    return folders[0]


def find_symbols(build_type):
    """Symbols of this config's game binary in the project Binaries folder.

    Binaries/ keeps every config built in this workspace (Tenetris.pdb for Development,
    Tenetris-Win64-Shipping.pdb for Shipping), so match the exact binary name.
    """
    platform = config.ue_platform()
    binary = config.PROJECT_NAME if build_type == "Development" else f"{config.PROJECT_NAME}-{platform}-{build_type}"
    extension = ".dSYM" if platform == "Mac" else ".pdb"
    binaries = config.PROJECT_ROOT / "Binaries" / platform
    return sorted(binaries.glob(f"{binary}{extension}")) if binaries.is_dir() else []


def zip_folder(folder, zip_path):
    """Zips the folder so it extracts as a single top-level folder."""
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED, allowZip64=True) as archive:
        for path in sorted(folder.rglob("*")):
            if path.is_file():
                archive.write(path, path.relative_to(folder.parent))


def package_win(build_dir, base, build_type):
    shipping = build_type == "Shipping"
    if shipping:
        for pdb in build_dir.rglob("*.pdb"):
            pdb.unlink()

    game = config.DIST_DIR / f"{base}.zip"
    zip_folder(build_dir, game)

    symbols = None
    pdbs = find_symbols(build_type) if shipping else []
    if pdbs:
        symbols = config.DIST_DIR / f"{base}-symbols.zip"
        with zipfile.ZipFile(symbols, "w", zipfile.ZIP_DEFLATED, allowZip64=True) as archive:
            for pdb in pdbs:
                archive.write(pdb, pdb.name)

    return game, symbols


def package_mac(build_dir, base, build_type):
    shipping = build_type == "Shipping"
    dsyms = find_symbols(build_type)

    if not shipping:
        for dsym in dsyms:
            shutil.copytree(dsym, build_dir / dsym.name, symlinks=True)

    game = config.DIST_DIR / f"{base}.dmg"
    config.run([
        "hdiutil", "create",
        "-volname", base,
        "-srcfolder", build_dir,
        "-format", "UDZO",
        "-ov", game,
    ])

    symbols = None
    if shipping and dsyms:
        symbols_dir = config.CI_DIR / "Symbols" / f"{base}-symbols"
        shutil.rmtree(symbols_dir.parent, ignore_errors=True)
        symbols_dir.mkdir(parents=True)
        for dsym in dsyms:
            shutil.copytree(dsym, symbols_dir / dsym.name, symlinks=True)

        symbols = config.DIST_DIR / f"{base}-symbols.zip"
        config.run(["ditto", "-c", "-k", "--keepParent", symbols_dir, symbols])

    return game, symbols


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    config.add_build_args(parser)
    args = parser.parse_args()

    shipping = args.build_type == "Shipping"
    version = config.version_string(args.build_number, args.build_type)
    base = config.artifact_base(version)

    shutil.rmtree(config.DIST_DIR, ignore_errors=True)
    config.DIST_DIR.mkdir(parents=True)

    # The archive folder name becomes the top-level folder users see after extracting.
    build_dir = find_build_dir()
    if build_dir.name != base:
        build_dir = build_dir.rename(config.ARCHIVE_DIR / base)

    if config.is_mac():
        game, symbols = package_mac(build_dir, base, args.build_type)
    else:
        game, symbols = package_win(build_dir, base, args.build_type)

    if shipping and not symbols:
        print("WARNING: no symbols found; nothing will be stored in symbols/.", flush=True)

    manifest = {
        "version": version,
        "build_type": args.build_type,
        "platform": config.ue_platform(),
        "game": game.name,
        "symbols": symbols.name if symbols else None,
    }
    config.MANIFEST.write_text(json.dumps(manifest, indent=2), encoding="utf-8")
    print(json.dumps(manifest, indent=2), flush=True)


if __name__ == "__main__":
    main()

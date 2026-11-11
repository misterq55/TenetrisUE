"""Writes the build number into ProjectVersion so the title screen can show it.

Only the CI workspace copy of DefaultGame.ini is changed; it is never committed, and the
next checkout restores the committed "local" value.
"""

import argparse
import re
from pathlib import Path

import config

SECTION = "[/Script/EngineSettings.GeneralProjectSettings]"
KEY = "ProjectVersion"


def stamp(ini_path, version):
    raw = ini_path.read_bytes()
    bom = raw.startswith(b"\xef\xbb\xbf")
    text = raw.decode("utf-8-sig")
    newline = "\r\n" if "\r\n" in text else "\n"
    lines = text.split(newline)

    try:
        start = next(i for i, line in enumerate(lines) if line.strip() == SECTION)
    except StopIteration:
        config.fail(f"{SECTION} not found in {ini_path}")

    end = next((i for i in range(start + 1, len(lines)) if lines[i].startswith("[")), len(lines))
    key_pattern = re.compile(rf"^{KEY}\s*=")

    for i in range(start + 1, end):
        if key_pattern.match(lines[i]):
            lines[i] = f"{KEY}={version}"
            break
    else:
        lines.insert(start + 1, f"{KEY}={version}")

    data = newline.join(lines).encode("utf-8")
    ini_path.write_bytes((b"\xef\xbb\xbf" if bom else b"") + data)
    print(f"{KEY}={version} -> {ini_path}", flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    config.add_build_args(parser)
    parser.add_argument("--ini", default=str(config.DEFAULT_GAME_INI))
    args = parser.parse_args()

    stamp(Path(args.ini), config.version_string(args.build_number, args.build_type))


if __name__ == "__main__":
    main()

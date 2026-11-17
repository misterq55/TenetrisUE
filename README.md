# Tenetris

A falling-block puzzle game with a twist, built with Unreal Engine 5.7 and C++.

Inspired by the film *Tenet*: the name is Tenet + Tetris, and the film's "inversion" idea
became the two core mechanics.

- **Space Inversion**: flip the whole board 180° in the middle of a game.
- **Time Inversion**: rewind your recent moves.

Also includes hold, next-piece preview, ghost piece, scoring and levels, and remappable
keyboard and gamepad controls.

**Contents:** [Download](#download) · [How to run](#how-to-run) · [Controls](#controls) ·
[System requirements](#system-requirements) · [Architecture](#architecture) ·
[Build pipeline](#build-pipeline) · [Credits](#credits)

---

## Download

Get the latest build from **[Releases](https://github.com/misterq55/TenetrisUE/releases/latest)**.

| Platform | File |
|----------|------|
| Windows  | `Tenetris-build-<N>-Win64.zip` |
| macOS    | `Tenetris-build-<N>-Mac.dmg` (Apple Silicon and Intel) |

## How to run

### Windows
1. Extract the whole zip before running it.
2. Run `Tenetris.exe` in the top folder. Do not run the exe inside `Binaries\`.
3. If you are asked to install the **Microsoft Visual C++ Redistributable**, choose *Yes*.
   The installer is included in the download and may need administrator rights.
4. If SmartScreen appears, choose *More info → Run anyway*.

### macOS
1. Open the `.dmg` and copy the app anywhere.
2. The app is not signed. Right-click it and choose *Open*.
   On macOS 15 or later, go to *System Settings → Privacy & Security → Open Anyway*.

## Controls

See **How to Play** on the title screen. It shows your current key bindings, including
any keys you changed. You can change keys in *Options*. Keyboard and gamepad are both
supported, including menu navigation.

## System requirements

Tenetris runs on any PC or Mac that meets the
[Unreal Engine 5.7 requirements for running the engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/hardware-and-software-specifications-for-unreal-engine).

- Windows 10/11 (64-bit)
- macOS on Apple Silicon or Intel

---

## Architecture

See **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)**.

## Build pipeline

A Jenkins pipeline (`CI/`) builds Windows and macOS in parallel from one commit. It
compiles, cooks and packages each platform, then publishes a GitHub Release only when every
platform succeeds. Shipping builds keep debug symbols in a private store and out of the
public download.

---

## Credits

**BGM**
- 「Hopping Popping Dance」 composed by 蒲鉾さちこ
- 「It's 8 AM! Speedy Start!」 composed by Addpico
- Provided by [OpenTracks](https://opentracks.com)

**Sound effects**
- ["success"](https://freesound.org/s/109662/) by grunz, licensed under CC BY

---

Tenetris is a personal portfolio project. It is not affiliated with or endorsed by
The Tetris Company or the makers of *Tenet*. Tetris is a trademark of The Tetris Holding, LLC.

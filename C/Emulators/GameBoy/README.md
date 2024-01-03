# Gameboy Emulator

This is a simple Gameboy Emulator written in C using SDL on Linux

Compiling:
```makefile
make release
```

Usage:
```sh
./bin/gameboy <gameboy_rom.gb>
```

Controls:
| Joypad        | keyboard  |
|---------------|-----------|
| A Button      | A Key     |
| B Button      | S Key     |
| Select Button | Space Key |
| Start Button  | Enter Key |
| D-Pad Right   | Right Key |
| D-Pad Left    | Left Key  |
| D-Pad Up      | Up Key    |
| D-Pad Down    | Down Key  |

Issues:

    - Sprite Rendering is not functioning
    - Cartridge banking issues

Unimplemented features:

    - Audio Processing Unit

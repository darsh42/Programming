# Gameboy Emulator

This is a simple Gameboy Emulator written in C using SDL on Linux

Compiling:
```makefile
make release
```

Usage:
```sh
./bin/gameboy <gameboy_rom.gb> <[optional] savefile>
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

NOTE: 
  To save some games hold down all non-D-pad keys at once

### TODO:

#### ISSUES:
- Implement Sound
- Implement Save files
- Sprites not properly loading for big games (see Pokemon Red)
  - Probably a MBC issue (Un-implemented)
    - Implement MBC3
- SCX scrolling is too fast (see Link's Awakening)

#### IMPROVEMENTS:
- Add a UI using ImGUI

#### FIXED ISSUES:
- Create "ppu_render_sprites" routine
- Create LCD Status routine
- implement all Interrupt service routines
- Check and Verify memory management routines and ROM/RAM bank switching
- Create proper start up sequence, set all correct register values and memory locations
- Create Cartridge header and load ROM correctly
- Fix Memory
  - Possible issue with RAM banking
- Fix CPU
  - potential issue with flag write routine
  - verify each opcode using Tetris ROM
- Fix Display renderer
  - Possible issue with PPU window screen
  - Sprites seem to have been drawn too much to the right (SML hitboxes)
  - Sprites dont draw middle pixels when horizontally flipped
  - SCX is choppy

#### FIXED IMPROVEMENTS:
- ROM and RAM banking as separate arrays for easier indexing

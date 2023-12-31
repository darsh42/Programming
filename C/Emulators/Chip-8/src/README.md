# Chip 8 Emulator

This is a simple Chip 8 Emulator written using C and SDL for Linux

Compiling:

``` makefile
make compile
```

Usage:

``` sh
./chip8 <chip8_rom.gb>
```

Controls:
| Chip 8 keypad | keyboard |
|---------------|----------|
| 1             | 1        |
| 2             | 2        |
| 3             | 3        |
| C             | 4        |
| 4             | Q        |
| 5             | W        |
| 6             | E        |
| D             | R        |
| 7             | A        |
| 8             | S        |
| 9             | D        |
| E             | F        |
| A             | Z        |
| 0             | X        |
| B             | C        |
| F             | V        |

Issues:
    
    - Controls aren't very responsive 
    - Due to controls some games hang and will not progress

Not Implemented:

    - Audio

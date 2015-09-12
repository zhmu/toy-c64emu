# c64emu

This is my attempt at a Commodore 64 emulator; mainly in order to teach myself how this nice computer worked. It's quite incomplete, but at least the basic character video, keyboard and .d64 disk drive emulation works. No attempt was made at cycle-accurate emulation or anything like that; however, this code should give a nice introduction on how to write a semi-decent emulator without making it too complicated :-)

## License

Everything except the ROM images and the .d64 image are licenses as beer-ware:

```
----------------------------------------------------------------------------
"THE BEER-WARE LICENSE" (Revision 42): Rink Springer <mail@rink.nu> wrote
this file. As long as you retain this notice you can do whatever you want
with this stuff. If we meet some day, and you think this stuff is worth it,
you can buy me a beer in return Rink Springer
----------------------------------------------------------------------------
```

## Building

Just go to the `src` directory and run `make`. The resulting emulator can then be started using `./c64emu`. Note that SDL 1.2 is needed to build this.

## Firmware files

All the ROM images were obtained from http://www.zimmers.net/anonftp/pub/cbm/firmware/computers/c64/. The C64 diagnostic disk was obtained from http://csdb.dk/release/?id=75835 - these items are provided for convenience only.

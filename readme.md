# scimitar_se_remapper
A tiny daemon which translates the default '1' to '=' keys on the side keypad to any other key. Currently remaps the keys to F13-F24 but a config file is in the works.

## Usage
The daemon needs to be run as root as it accesses the /dev/input files. It takes no arguments.

## Todo
- Config file with keybinds
- Better robustness (currently breaks if the mouse is unplugged while the daemon is active)
- Wired support (connecting the mouse with a wire, rather than the wireless dongle, makes it expose a different product and version ids)
- Investigate the bluetooth mode (probably the same thing as the wire but I have to check)

## Building
You will need cmake, make, pkgconf, libevdev and linux-api-headers.

After cloning the repository, cd into it and run this line.
```
cmake -B build && cd build && make release
```
The resulting program will be called scimitar_remapd and sit in the build directory.

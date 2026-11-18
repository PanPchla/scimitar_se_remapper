# scimitar_se_remapper
A tiny daemon which translates the default 1-12 keys (which print out '1' to '=') on the side keypad to any other key.

## Usage
The daemon needs to be run as root as it accesses the /dev/input files. It takes no arguments.

## Config
The config file should be at $XDG_CONFIG_HOME/scimitar_se_remapper/binds.conf (this should be /home/username/.config/scimitar_se_remapper/binds.conf by default).
The format consits of key value pairs with the key being the key number on the keypad itself. The value should be a keycode as defined in [linux/input-event-codes.h](https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h)

Example:
```
1=KEY_F14
2=KEY_F15
4=KEY_1
7=KEY_2
8=KEY_Q
9=KEY_MACRO1
```
Any skipped keys will be passed through as normal.

I found that KEY_MACRO1 through KEY_MACRO30 keys are nice if you want unique keys, as some in the KEY_F13-KEY_F25 range are bound to media keys on my system.

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

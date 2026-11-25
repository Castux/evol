# evöL

A Lua game engine in the spirit of [LÖVE](https://love2d.org/), built only on SDL surfaces, to run on machines without good graphics hardware.

At the time, LÖVE relied heavily on hardware acceleration, which made it almost unusable on older machines. evöL keeps an API similar to LÖVE's and its philosophy of very fast game prototyping in Lua, with internals based only on SDL surfaces: keyboard and mouse input, image loading, editing and blitting, fonts, fixed or variable FPS, graphic primitives, sound, an OpenGL context, and a curses-like terminal emulator.

Games made with it: [Revenge of the Orange](https://github.com/Castux/revenge-of-the-orange), [WikNukem Forever](https://github.com/Castux/wiknukem-forever), [Rise of the Taka-Pum](https://github.com/Castux/rise-of-the-taka-pum) and [Arks of Mercy](https://github.com/Castux/arks-of-mercy).

`main` is the Lua 5.2 branch (2013); the `evol-2.0` branch is the 2.0 line.

It was imported from Launchpad (Bazaar) in 2026, with its full history.

The original release archives are in the [releases](../../releases).

## License

GNU GPL v3.

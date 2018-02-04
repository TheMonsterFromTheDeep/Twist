# Twist
Twist, a sort-of-portmanteau of **Twion** and **SDL**, is a UI framework based on SDL2 built in order to provide basic UI building blocks for the [Twion](https://github.com/TheMonsterFromTheDeep/twion/edit/master/README.md) project.

Its goal is to include:
- Basic rendering abstractions based on OpenGL, such as drawing image assets and text
- Basic UI components, such as buttons and text fields
- Built in theming of UI components, including color and size

More specifically, its goal is to provide a UI framework that is enough to build a Blender-like UI: One where individual panels can be split and merged by the user, with each one containing some program function.

## Dependencies
Twist depends on [SDL2](https://www.libsdl.org/), [FreeType](https://www.freetype.org/), and [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h).

## Building
As it currently stands, Twist provides no build process. It will eventually be migrated to CMake.

## Licensing
In an effort to reciprocate the kindness of the SDL project in being so freely licensed, Twist is similarly licensed under the zlib/libpng license.

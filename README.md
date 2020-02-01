# unicode-maze-runner
A C Implementation of a simple maze game using unicode characters for the display

## Contents
1. [Installation](#Installation)
2. [Gameplay](#Gameplay)
3. [Notes](#Notes)

## [Installation](#Installation)
To install you need to have `make` installed, so on your respective linux distro, make sure you have installed `build-essential` package using your respective package manager.

Once installed run `make` or `make all` or `make main` to build the program. To run the program simply execute the `main` executable created from running `make`.


## [Gameplay](#Gameplay)
Use `wasd` keys to move the character from the upper left corner of the screen to the bottom right corner of the screen, try to do it in as few moves as possible. To increase the difficulty simply increase the resolution of the terminal with `Ctrl + '-'` and to lower do the opposite with `Ctrl + '+'`. Each round the resolution of the terminal is found and a new maze is generated any changes to the resolution of the terminal will be reflected here.


## [Notes](#Notes)
This program makes extensive use of unicode to see how it works, first look at [display.c](./display.c) and then look at [utf8.c](./utf8.c) and [utf8.h](./utf8.h) for implementation details. I used the notes and source code from this website to help me:

[https://www.cprogramming.com/tutorial/unicode.html](https://www.cprogramming.com/tutorial/unicode.html)

If you have an trouble or need help feel free to open an issue.

# console-tetris

> A terminal ui to play tetris in the console

![image](https://user-images.githubusercontent.com/49595640/177736119-8bf99c5c-db85-4b22-9287-5674cfb78e69.png)

## Features

* Support for big and small terminals 
* Progress is saved at ```~/.config/console-tetris/console-tetris.conf```
* Growing difficulty with increasing levels
* Point calculation like on [Nintendo's Tetris versions](https://tetris.fandom.com/wiki/Scoring)
* Tetromino rotation like on [the Gamebody version](https://strategywiki.org/wiki/File:Tetris_rotation_Gameboy.png)

### Shortcuts

* <kbd>E</kbd>xit the program
* <kbd>P</kbd>ause the game 
* <kbd>R</kbd>eset the game

## Build & Usage

This program has a single dependency to [ncurses](https://archlinux.org/packages/core/x86_64/ncurses/) and was tested on Arch

For building you need to have `cmake` installed:
```bash
# Setup cmake
cmake .
# Build the project
cmake --build .
```
After this, you'll find the `console-tetris` binary in the `./bin` directory
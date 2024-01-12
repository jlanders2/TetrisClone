# Tetris Clone (Incomplete)

Creating a Tetris clone in C to practice C programming and learn more about Raylib (awesome library btw!).

## Description

This game is a clone of the popular game Tetris. This clone features what you would expect from a simplistic version of Tetris, namely: falling blocks, which upon completion of a row disappear, score keeping and levels with incremental difficulty.

Here are some early screenshots of my progress:

![gameplay](screenshots/game_screenshot.png "Gameplay")
![gameover](screenshots/game_over_screenshot.png "Game Over")

Features left to implement:
- Update codebase to use cohesive style (still haven't landed on exactly what I like)
- Re-Implement rotation algorithm; my original algorithm was extremely buggy
- Empty rows that are full should "break"
- Broken rows should cause other rows to shift down without losing ordering
- scoring system
- display upcoming block
- introduce levels and increasing speed/score multiplier

I hope you enjoy!

## Getting Started

### Dependencies

* [Raylib](https://github.com/raysan5/raylib) - check out this repo for instructions on how to install
* Make - build automation tool

### Installing

* Download and extract zip of this repo or clone the repository using:
```
git clone https://github.com/jlanders2/TetrisClone
```

### Executing program

* Open terminal/command prompt and navigate to where you downloaded the repo
* execute the following commands from the TetrisClone folder
```
cd src/
make
./tetris-clone
```

## Authors

Contributors names and contact info

John Landers [jcolelanders@gmail.com](mailto:jcolelanders@gmail.com)

## Version History

Unreleased

## License

This project is licensed under the MIT License - see the LICENSE file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [Tetris](https://en.wikipedia.org/wiki/Tetris)
* [raylib](https://github.com/raysan5/raylib)

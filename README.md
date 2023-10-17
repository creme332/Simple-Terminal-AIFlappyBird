# ASCII Flappy Bird 🐦

A terminal-based ascii flappy bird game with AI.

![GIF showing single player mode gameplay](BirdVersion1.gif)

## Installation
> 🔴 **Note:** This game will work only on a Windows system because of some Windows-specific libraries used.

1. Download the project using Git or otherwise.
2. Run `birdgame.cpp` using a compiler that supports C++14.

## Usage
- On startup screen, choose an option by pressing `1` or `2` or `3`.
- When in single player mode, press `spacebar` to jump. 
- In AI mode you can only watch the AI play:
![GIF showing AI gameplay](AIGif.gif)

## How it works
![image](https://user-images.githubusercontent.com/65414576/165916397-9cc1dde2-31a7-45c6-b1f6-7c14948920d8.png)

- At any time, there are at most 2 pipes on screen. 
- The bird only moves vertically while the pipes move horizontally to the left. 
- `pipe[0]` always denotes the index of the pipe closest to the bird. Once this pipe is behind the bird, it is removed from screen and a new one is created.

## Inspirations
- https://github.com/hamikm/AsciiBird
- https://www.youtube.com/watch?v=yASrfF1C3ms

## Future work
- [ ] Add flapping wings to [bird](https://imgur.com/gallery/gKpkYqL).
- [ ] Add different difficulty modes (vary gap between pipes, speed of bird, ...)


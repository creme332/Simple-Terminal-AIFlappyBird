# 🐦AI Flappy Bird
## 💻AI mode ##
![](AIGif.gif)
## ✋ Normal ##
![](BirdVersion1.gif)

# 🌄Inspirations #
https://github.com/hamikm/AsciiBird

https://www.youtube.com/watch?v=yASrfF1C3ms

# Setup # 
> ⚠️ Game will work NOT work on online compilers and on Mac OS.

Compile the `birdgame.cpp` file found in the `birdgame` folder.

# 📓How to play #
1. Single player mode : Press spacebar to jump.
2. AI mode : watch AI bird play but keep in mind that the AI bird will never die.

# 📇Code explanation #
![image](https://user-images.githubusercontent.com/65414576/165916397-9cc1dde2-31a7-45c6-b1f6-7c14948920d8.png)

- At any time, there are at most 2 pipes on screen. 
- The bird only moves vertically while the pipes move horizontally to the left. 

- `pipe[0]` always denotes the index of the pipe closest to the bird. Once this pipe is behind the bird, it is removed from screen and a new one is created.

# 🔮Future work #
- [x] Add gameplay to README
- [x] Remove unicode characters so that code can run on IDE not supporting unicode.
- [x] Add code to get rid of flickering cursor 
- [x] Add AI bird
- [x] Fix collision bug (when bird is between 2 pipes, it can partially collide with the bottom pipe.)
- [x] Add code explanation with diagram

- [ ] Add flapping wings to [bird](https://imgur.com/gallery/gKpkYqL).
- [ ] Add different difficulty modes (vary gap between pipes, speed of bird, ...)


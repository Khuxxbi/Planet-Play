# Planet Play

Planet Play is a space game built with C++ and SplashKit. You pilot a ship
through space using the arrow keys — the foundation for a game I build up
feature by feature across the project's history.

I built this during my first year of Computer Science, working through the Space
Game example from SplashKit's [The Programmer's Field Guide](https://programmers.guide)
chapter by chapter. Rather than writing it all at once, I committed each stage as
I went — each one adding a new concept, working up from basic movement through
structs, dynamic arrays, and classes — so the history shows both the game and my
understanding growing over time.

![Planet Play screenshot](screenshot.png)

## How it works

- The ship and coins are loaded from a SplashKit resource bundle.
- The ship turns with the left and right keys and thrusts in the direction it faces, keeping its momentum.
- Flying off one edge of the screen brings the ship back on the opposite side.
- Coins appear at random spots over time, and each one collected scores a point.
- The screen clears and redraws at 60 frames per second.

## Controls

| Input | Action |
|-------|--------|
| Up arrow | Thrust forwards |
| Down arrow | Thrust backwards |
| Left / Right arrow | Turn the ship |
| Close window | Quit |

## Development log

The game is built as a series of commits, each adding one idea on top of the last:

- **Flying ship** — load a ship bitmap and move it around the window with the arrow keys.
- **Collectible coin** — a spinning coin that jumps to a new random spot each time the ship touches it.
- **Cleaner structure** — split the game loop into separate input, update, and drawing functions.
- **Scoring and momentum** — grouped the game's data into structs, gave the ship momentum and screen wrapping, and added a score for each coin collected.
- **Many coins at once** — swapped the single coin for a dynamic array, so coins spawn over time and are removed as they are collected.

## Built with

- **C++**
- **SplashKit** — used for the window, drawing, and keyboard input

## Running it

You'll need [SplashKit](https://splashkit.io) installed. From inside this folder,
compile the game:

```bash
skm clang++ planet-play.cpp -o planet-play
```

Then run it:

```bash
./planet-play
```

> On Windows or Linux you may use `skm g++` instead of `skm clang++`.

## Files

| File | Purpose |
|------|---------|
| `planet-play.cpp` | Main game — the loop, ship movement, and drawing |
| `Resources/` | Ship image, coin animation, and the SplashKit resource bundle |
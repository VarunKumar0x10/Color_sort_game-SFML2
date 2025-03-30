# Color Sort Puzzle Game

A simple 2D puzzle game implemented in C++ using SFML where players sort colored liquids in test tubes. The goal is to sort all colors into separate tubes by transferring liquids between tubes.

## Game Features

- Color sorting puzzle mechanics
- Mouse-based interaction
- Sound effects and background music
- Multiple tubes with different colored liquids
- Win condition detection
- Move counter

## Prerequisites

To build and run this game, you need:

- C++ Compiler (GCC 13.1.0 recommended)
- SFML 2.6.2 or later

## Building the Project


### Manual Compilation (GCC)
```bash
g++ main.cpp Tube.cpp Game.cpp -o ColorSortGame.exe -IC:/SFML-2.6.2/include -LC:/SFML-2.6.2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

## How to Play

1. Click on a tube to select it
2. Click on another tube to transfer the top liquid
3. Transfer liquids between tubes to sort colors
4. Game is won when all colors are sorted into separate tubes

## Game Rules

- Can only transfer liquids of the same color
- Can only transfer to empty tubes or tubes with matching colors on top
- Each tube can hold up to 4 liquid units
- Game is complete when all colors are sorted into separate tubes

## Controls

- Left Mouse Click: Select tube/Transfer liquid
- Close Window Button: Exit game

## Dependencies

- SFML 2.6.2 (Simple and Fast Multimedia Library)
  - Graphics module
  - Window module
  - System module
  - Audio module

## Development Notes

The game uses:
- Custom stack implementation for liquid management
- SFML for graphics and audio
- Event-driven architecture for game logic

## Future Improvements

- Add difficulty levels
- Implement level selection
- Add animations for liquid transfer
- Add a scoring system
- Add a level editor
- Add save/load game functionality

## Author

# Varun Kumar

// Game.h
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <stack>
#include "Tube.h"
#include "Autosolver.h"

inline bool soundon = true;
inline sf::Music mybgm;


class Game
{
private:
    sf::RenderWindow& window;
    std::vector<TestTube> tubes;
    
    // Game state variables
    
    int selectedTubeIndex;
    int moveCount;
    
    // Window dimensions (for tube positioning)
    const float WINDOW_WIDTH = 800;
    const float WINDOW_HEIGHT = 600;
    
    // Tube dimensions
    const float TUBE_WIDTH = 60;
    const float TUBE_HEIGHT = 200;
    const float TUBE_THICKNESS = 3;
    short int max_colors_count;
    short int colored_tubes;
    short int empty_tubes;    // empty tubes should be 2 to ensure the game is solvable

    const sf::IntRect undo_button{700, 10, 64, 64};

    sf::SoundBuffer temp_buffer;
    sf::Sound click_sound;
    sf::Texture game_texture;
    sf::Sprite game_sprite;
    sf::Texture undo_texture;
    sf::Sprite undo_img;

    const sf::IntRect solve_button{50, 10, 75, 75}; 
    sf::Texture solve_texture;
    sf::Sprite solve_img;

    std::vector<Move> autoSolution;
    size_t autoMoveIndex = 0;   
    bool isAutoSolving = false;
    sf::Clock autoMoveTimer;



public:
    Game(sf::RenderWindow& window,short int num_col_tubes, short int num_empty_tubes);
    
    void run();
    void handleEvents();
    void update();
    void render();
    bool isRunning;
    std::stack <std::vector <std::vector<sf::Color> > > historyStates; // contains all the prevstates
    void clearhistory(std::stack <std::vector <std::vector<sf::Color> > >& historyStates);
    
private:
    void initializeTubes();
    void handleMouseClick(int mouseX, int mouseY);
    bool isGameWon() const;
    int getTubeIndexAtPosition(int mouseX, int mouseY);
    void showwinmessage();

    void undo_function();

    short int getEmptyTubes() const;
    short int getColoredTubes() const;
    short int getColorsCount() const;

    void setEmptyTubes(short int empty_tubes);
    void setColoredTubes(short int colored_tubes);
    void setColorsCount(short int colors_count);
};

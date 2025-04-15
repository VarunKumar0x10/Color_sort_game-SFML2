// Homepage.h
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"  

class HomePage {
private:
    sf::RenderWindow& window;
    sf::Image icon; // icon of the window
    sf::Texture background_texture, loading_texture;   //temporary stores img
    sf::Sprite background_img, loading_img; 
    short int difficulty;  // 1 for easy, 2 for medium, 3 for hard

    
    const sf::IntRect playButton{200, 230, 350, 115};      // x, y, width, height
    const sf::IntRect howToPlayButton{200, 380, 240, 75};
    const sf::IntRect creditsButton{230, 475, 300, 75};
    const sf::IntRect settingsButton{700, 40, 100, 80};

    // Game state
    bool isGameStarted;
    Game* currentGame;
    short int coloredTubes=3, emptyTubes=2;  // default values for easy difficulty

    bool isButtonClicked(const sf::IntRect& button, sf::Vector2i mousePos) const;

public:
    HomePage(sf::RenderWindow& window);
    //~HomePage();

    void run();
    void draw();
    void handleMouseClick(sf::Vector2i position);
    void update();
    void howtoplaypopup();
    void creditspopup();
    void settingspopup();
    bool isInGame() const { return isGameStarted; }

    void setdifficulty(short int diff){ difficulty = diff; }
    short int getdifficulty() const{ return difficulty;}
};


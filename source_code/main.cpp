/*
Color sort puzzle game

Made by:
    - Varun Kumar
      2401MC31
      Group 20
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include "Homepage.h"


int main() {

    // Seed the random number generator for random color distribution
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Create the main window
    sf::RenderWindow window( sf::VideoMode(800, 600),"Color Sort Puzzle",sf::Style::Titlebar | sf::Style::Close);
    
    // Set frame rate limit to prevent excessive CPU usage
    window.setFramerateLimit(60);

        
     HomePage homepage(window);
     homepage.run();
    return 0;
}


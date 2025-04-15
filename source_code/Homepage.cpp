#include "Homepage.h"

HomePage::HomePage(sf::RenderWindow& window) 
    : window(window), 
      isGameStarted(false), 
      currentGame(nullptr),
      difficulty(1) // Default to easy difficulty
{
    // Set the icon of the window
    icon.loadFromFile("Resources/Images/color_sort-icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    if (!background_texture.loadFromFile("Resources/Images/home_screen_img-crp.png")) {
        std::cout << "Error loading homepage background" << std::endl;
    }
    background_img.setTexture(background_texture);

    if(loading_texture.loadFromFile("Resources/Images/loading_scr.png")){
        loading_img.setTexture(loading_texture);
    }
    

	if(mybgm.openFromFile("Resources/bgm_music.wav")){
        mybgm.setLoop(true);
        if(soundon) mybgm.play();
    }
}


bool HomePage::isButtonClicked(const sf::IntRect& button, sf::Vector2i mousePos) const {
    return button.contains(mousePos);
}

void HomePage::handleMouseClick(sf::Vector2i position) {
    if (isGameStarted) {
        return; // Ignore clicks when game is running
    }

    if (isButtonClicked(playButton, position)) {
        std::cout << "Play button clicked!" << std::endl;
        // Start new game with current difficulty
        if (currentGame != nullptr) {
            delete currentGame;
        }
        
        // Initialize game based on difficulty
        
        switch (difficulty) {
            case 1: // Easy
                coloredTubes = 3;
                emptyTubes = 2;
                break;
            case 2: // Medium
                coloredTubes = 4;
                emptyTubes = 2;
                break;
            case 3: // Hard
                coloredTubes = 5;
                emptyTubes = 2;
                break;
            default:
                coloredTubes = 3;
                emptyTubes = 2;
        }
        
        Game newgame(window, coloredTubes, emptyTubes);
        newgame.run();
        isGameStarted = true;
		return;
    }
    else if (isButtonClicked(howToPlayButton, position)) {
        std::cout << "How to Play clicked!" << std::endl;
        howtoplaypopup();
    }
    else if (isButtonClicked(creditsButton, position)) {
        std::cout << "Credits clicked!" << std::endl;
        creditspopup();
    }
    else if (isButtonClicked(settingsButton, position)) {
        std::cout << "Settings clicked!" << std::endl;
        settingspopup();
    }
}

void HomePage::run() {

    window.clear();
    window.draw(loading_img);
    window.display();
    sf::sleep(sf::seconds(1.5f)); // Display loading screen for 1.5 seconds
    window.clear();

    while (window.isOpen() && !isInGame()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    handleMouseClick(sf::Mouse::getPosition(window));
                }
            }

			if(!soundon) mybgm.pause();
        }

       draw();
    }
}

void HomePage::draw() {
    if (isGameStarted && currentGame != nullptr) {
        // Game is running, let the game handle rendering
        currentGame->render();
    } else {
        // Draw homepage
        window.clear();
        window.draw(background_img);
        window.display();
    }
}

void HomePage::update() {
    if (isGameStarted && currentGame != nullptr) {
        // Update game state
        currentGame->handleEvents();
        currentGame->update();
        
        // Check if game is finished
        if (!currentGame->isRunning) {
            isGameStarted = false;
            delete currentGame;
            currentGame = nullptr;
        }
    }
}


void HomePage::howtoplaypopup() {
    sf::RenderWindow tutorialpopup(sf::VideoMode(700, 500), "How to Play", sf::Style::Titlebar | sf::Style::Close);
    sf::Texture tutorial_texture;
    sf::Sprite tutorial_sprite;
    if (tutorial_texture.loadFromFile("Resources/Images/howtoplay.png")){
        tutorial_sprite.setTexture(tutorial_texture);
    }

    sf::Image icon;
    icon.loadFromFile("Resources/Images/color_sort-icon.png");
    tutorialpopup.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    
    // Event loop for the popup
    sf::Event event;
    while (tutorialpopup.isOpen()) {
        while (tutorialpopup.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
                tutorialpopup.close();
            }
        }

        // Render the popup
        tutorialpopup.clear(sf::Color::White);
        tutorialpopup.draw(tutorial_sprite);
        tutorialpopup.display();
    }
}

void HomePage::creditspopup() {
    // Create a small popup window for credits
    sf::RenderWindow mycreditpopup(sf::VideoMode(650, 450), "Credits", sf::Style::Titlebar | sf::Style::Close);

    sf::Texture credits_texture;
    sf::Sprite credits_sprite;

    if(credits_texture.loadFromFile("Resources/Images/credits.png")){
        credits_sprite.setTexture(credits_texture);
    }

    sf::Image icon;
    icon.loadFromFile("Resources/Images/color_sort-icon.png");
    mycreditpopup.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    sf::Event event;
    while (mycreditpopup.isOpen()) {
        while (mycreditpopup.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
                mycreditpopup.close();
            }
        }

        
        mycreditpopup.clear(sf::Color::White);
        mycreditpopup.draw(credits_sprite);
        mycreditpopup.display();
    }
}



void HomePage::settingspopup() {
    // Create a popup window for settings
    sf::RenderWindow popup(sf::VideoMode(500, 400), "Settings", sf::Style::Titlebar | sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("Resources/arial.ttf")) {  
        return;
    }

    sf::Texture setting_bg_texture;
    sf::Sprite setting_bg;
    if (setting_bg_texture.loadFromFile("Resources/Images/settings-background.png")) {
            setting_bg.setTexture(setting_bg_texture);
    }

    sf::Image icon;
    icon.loadFromFile("Resources/Images/color_sort-icon.png");
    popup.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    // Background
    sf::RectangleShape background(sf::Vector2f(500, 400));
    background.setFillColor(sf::Color(230, 230, 230)); // Light gray background

    // Title
    sf::Text titleText("Game Settings", font, 30);
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(150.f, 20.f);

    // Sound section
    sf::RectangleShape soundBox(sf::Vector2f(400, 60));
    soundBox.setFillColor(sf::Color(245, 245, 245)); // Slightly lighter than background
    soundBox.setOutlineColor(sf::Color(200, 200, 200));
    soundBox.setOutlineThickness(2);
    soundBox.setPosition(50.f, 70.f);

    sf::Text soundLabel("Sound:", font, 24);
    soundLabel.setFillColor(sf::Color::Black);
    soundLabel.setPosition(70.f, 85.f);

    sf::RectangleShape soundToggleBox(sf::Vector2f(100, 40));
    soundToggleBox.setFillColor(sf::Color(220, 220, 220));
    soundToggleBox.setOutlineColor(sf::Color::Black);
    soundToggleBox.setOutlineThickness(1);
    soundToggleBox.setPosition(300.f, 80.f);

    sf::Text soundText("ON", font, 24);
    soundText.setFillColor(sf::Color::Black);
    soundText.setPosition(335.f, 85.f);
    
    soundon = true;

    // Difficulty section
    sf::RectangleShape difficultyBox(sf::Vector2f(400, 200));
    difficultyBox.setFillColor(sf::Color(245, 245, 245));
    difficultyBox.setOutlineColor(sf::Color(200, 200, 200));
    difficultyBox.setOutlineThickness(2);
    difficultyBox.setPosition(50.f, 150.f);

    sf::Text difficultyLabel("Difficulty:", font, 24);
    difficultyLabel.setFillColor(sf::Color::Black);
    difficultyLabel.setPosition(70.f, 165.f);

    // Difficulty Options
    sf::RectangleShape easyButton(sf::Vector2f(350, 40));
    sf::RectangleShape mediumButton(sf::Vector2f(350, 40));
    sf::RectangleShape hardButton(sf::Vector2f(350, 40));

    easyButton.setPosition(75.f, 200.f);
    mediumButton.setPosition(75.f, 250.f);
    hardButton.setPosition(75.f, 300.f);

    // Set initial colors
    easyButton.setFillColor(sf::Color(152, 251, 152)); // Light green for selected
    mediumButton.setFillColor(sf::Color(220, 220, 220));
    hardButton.setFillColor(sf::Color(220, 220, 220));

    easyButton.setOutlineColor(sf::Color::Black);
    mediumButton.setOutlineColor(sf::Color::Black);
    hardButton.setOutlineColor(sf::Color::Black);

    easyButton.setOutlineThickness(1);
    mediumButton.setOutlineThickness(1);
    hardButton.setOutlineThickness(1);

    sf::Text easyText("Easy", font, 24);
    sf::Text mediumText("Medium", font, 24);
    sf::Text hardText("Hard", font, 24);

    easyText.setPosition(225.f, 205.f);
    mediumText.setPosition(215.f, 255.f);
    hardText.setPosition(225.f, 305.f);

    easyText.setFillColor(sf::Color::Black);
    mediumText.setFillColor(sf::Color::Black);
    hardText.setFillColor(sf::Color::Black);

    sf::Event event;
    while (popup.isOpen()) {
        while (popup.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                popup.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = popup.mapPixelToCoords(sf::Mouse::getPosition(popup));

                // Toggle Sound ON/OFF
                if (soundToggleBox.getGlobalBounds().contains(mousePos)) {
                    soundon = !soundon;
                    soundText.setString(soundon ? "ON" : "OFF");
                    if(!soundon) mybgm.pause();
                    if(soundon) mybgm.play();
                }

                // Change Difficulty
                if (easyButton.getGlobalBounds().contains(mousePos)) {
                    easyButton.setFillColor(sf::Color(152, 251, 152)); // Light green
                    mediumButton.setFillColor(sf::Color(220, 220, 220)); // Light gray
                    hardButton.setFillColor(sf::Color(220, 220, 220)); // Light gray
                    setdifficulty(1);
                } else if (mediumButton.getGlobalBounds().contains(mousePos)) {
                    easyButton.setFillColor(sf::Color(220, 220, 220));
                    mediumButton.setFillColor(sf::Color(152, 251, 152));
                    hardButton.setFillColor(sf::Color(220, 220, 220));
                    setdifficulty(2);
                } else if (hardButton.getGlobalBounds().contains(mousePos)) {
                    easyButton.setFillColor(sf::Color(220, 220, 220));
                    mediumButton.setFillColor(sf::Color(220, 220, 220));
                    hardButton.setFillColor(sf::Color(152, 251, 152));
                    setdifficulty(3);
                }
            }
        }

        // Render the popup
        popup.clear(sf::Color::White);
        popup.draw(setting_bg);
        //popup.draw(background);
        popup.draw(titleText);
        
        popup.draw(soundBox);
        popup.draw(soundLabel);
        popup.draw(soundToggleBox);
        popup.draw(soundText);
        
        popup.draw(difficultyBox);
        popup.draw(difficultyLabel);
        
        popup.draw(easyButton);
        popup.draw(mediumButton);
        popup.draw(hardButton);
        
        popup.draw(easyText);
        popup.draw(mediumText);
        popup.draw(hardText);
        
        popup.display();
    }
}
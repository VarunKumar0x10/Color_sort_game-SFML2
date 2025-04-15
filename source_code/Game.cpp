#include "Game.h"


Game::Game(sf::RenderWindow& enter_window,short int num_col_tubes, short int num_empty_tubes):window(enter_window){
    selectedTubeIndex=-1; 
    moveCount=0;
    setColoredTubes(num_col_tubes);
    setColorsCount(num_col_tubes);    //number of different colors should be equal to number of colored tubes
    setEmptyTubes(num_empty_tubes);


    if(temp_buffer.loadFromFile("Resources/click.wav")){
         click_sound.setBuffer(temp_buffer); //set the click sound
    }

    if (game_texture.loadFromFile("Resources/Images/simple_bg-crp.png")) {
        game_sprite.setTexture(game_texture); //set the game background
    }

    if (undo_texture.loadFromFile("Resources/Images/undo.png")) {
        undo_img.setTexture(undo_texture); //set the pause button image
        undo_img.setPosition(700,10);
        undo_img.setScale(0.5f,0.5f);
    }

    if (solve_texture.loadFromFile("Resources/Images/solver.png")) {
        solve_img.setTexture(solve_texture);
        solve_img.setPosition(50, 10);
        solve_img.setScale(0.5f, 0.5f);
    }
    
    
    initializeTubes();
}

void Game::initializeTubes() {
    // Position tubes in a row with proper spacing
    int num_tubes = colored_tubes + empty_tubes;
    float spacing = (600-(num_tubes*60))/(num_tubes+1);
    float startX = (WINDOW_WIDTH - (num_tubes * TUBE_WIDTH + (num_tubes-1) * spacing)) / 2; 
    float y = WINDOW_HEIGHT / 2;

    // Create 5 tubes (4 with colors + 1 empty)
    for (int i = 0; i < num_tubes; i++) {
        tubes.emplace_back(startX + i * (TUBE_WIDTH + spacing), y, TUBE_WIDTH, TUBE_HEIGHT, TUBE_THICKNESS);
    }


    std::vector<sf::Color> colors = {
        sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow,sf::Color::Magenta,sf::Color::Cyan
    };

    // Fill first 4 tubes with random colors
    vector<int> color_count (max_colors_count,0); 
    // Red is 0, Blue is 1, Green is 2, Yellow is 3
    for (int i = 0; i < colored_tubes; i++) {

        for (int j = 0; j < 4; j++) {
        
            int random_color = rand() % max_colors_count;

            if(color_count[random_color] < 4) {
                tubes[i].addColor(colors[random_color]);
                color_count[random_color]++;
            }
            else j--;
            // if the color is already at max count, try again
        }
    }
    
}

void Game::run() {
    isRunning = true;
    // Main game loop
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }

    mybgm.stop();  //stop the music when the game is over
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if(soundon) click_sound.play();   
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);

                if(undo_button.contains(event.mouseButton.x, event.mouseButton.y)){
                    undo_function();
                }

                if (solve_button.contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (!isAutoSolving) {
                        AutoSolver solver(tubes);  // pass current tubes
                        if (solver.solve(autoSolution)) {
                            std::cout << "Solution found: " << autoSolution.size() << " moves\n";
                            isAutoSolving = true;
                            autoMoveIndex = 0;
                            autoMoveTimer.restart();
                        } else {
                            std::cout << "No solution found!\n";
                        }
                    }
                }
                

            }
        }
        if(!soundon) mybgm.pause();
    }
}

int Game::getTubeIndexAtPosition(int mouseX, int mouseY) {
    // This function checks if the mouse click is on a tube and returns the index of the tube
    for (int i = 0; i < tubes.size(); i++) {
        
        float tubeX = tubes[i].getPosition().x;
        float tubeY = tubes[i].getPosition().y;
        
        if (mouseX >= tubeX && mouseX <= tubeX + TUBE_WIDTH &&
            mouseY >= tubeY && mouseY <= tubeY + TUBE_HEIGHT) {
            return i;
        }
    }
    return -1;
}

/* This function handles the mouse click and transfers the liquid from the 
    selected tube to the clicked tube*/
void Game::handleMouseClick(int mouseX, int mouseY) {
     

    int clickedTubeIndex = getTubeIndexAtPosition(mouseX, mouseY);
    
    if (clickedTubeIndex != -1) {
        if (selectedTubeIndex == -1) {
            // First tube selection
            selectedTubeIndex = clickedTubeIndex;
            tubes[selectedTubeIndex].isSelected = true;
        }
        else {
            if (selectedTubeIndex != clickedTubeIndex) {

                // Save current state before transferring
                std::vector<std::vector<sf::Color>> currentState;
                    for (auto& tube : tubes) {
                        currentState.push_back(tube.getStack());
                    }
                historyStates.push(currentState);

                // Second tube selection - attempt transfer
                while(tubes[selectedTubeIndex].transferLiquid(tubes[clickedTubeIndex]) ) {
                    moveCount++;
                    cout<<"Move Count: "<<moveCount<<endl;
                }
            }
            tubes[selectedTubeIndex].isSelected = false;
            selectedTubeIndex = -1;  // Reset selection
        }
    }
}

bool Game::isGameWon() const {
    // Check if all tubes are either empty or full with the same color
    for (const auto& tube : tubes) {
        // if the tube is not empty and not complete, the game is not won and returns false
        if (!tube.isEmpty() && !tube.isComplete()) {
            return false;
        }
    }
    return true;
} 

void Game::update() {

    if (isAutoSolving && autoMoveIndex < autoSolution.size()) {
        if (autoMoveTimer.getElapsedTime().asSeconds() >= 1.0f) {
            Move m = autoSolution[autoMoveIndex++];
            tubes[m.from].transferLiquid(tubes[m.to]);
            moveCount++;
            autoMoveTimer.restart();
        }
    
        if (autoMoveIndex >= autoSolution.size()) {
            isAutoSolving = false;
        }
    }
    
    // if (isGameWon()) {
    //     std::cout << "Congratulations! You won in " << moveCount << " moves!" << std::endl;
    //     isRunning = false;
    //     showwinmessage();
    // }
}

void Game::render() {
    window.clear(sf::Color::White);
    window.draw(game_sprite);
    window.draw(undo_img);
    window.draw(solve_img);

    
    // Draw all tubes
    for (int i = 0; i < tubes.size(); i++) {

        if (tubes[i].isSelected) {
            // Draw an outline around the selected tube
            sf::RectangleShape outline(sf::Vector2f(TUBE_WIDTH + 6, TUBE_HEIGHT + 6)); // Slightly larger than the tube
            outline.setPosition(tubes[i].getPosition().x - 3, tubes[i].getPosition().y - 3); // Offset to center the outline
            outline.setFillColor(sf::Color::Transparent); // outline
            outline.setOutlineThickness(5); // Thickness of the outline
            outline.setOutlineColor(sf::Color::Yellow); // Yellow outline
            window.draw(outline);
        }
        tubes[i].draw(window);
    }

    window.display();

    if (isGameWon()) {
        std::cout << "Congratulations! You won in " << moveCount << " moves!" << std::endl;
        isRunning = false;
        showwinmessage();
    }
}

// Getter functions
short int Game::getEmptyTubes() const {
    return empty_tubes;
}

short int Game::getColoredTubes() const {
    return colored_tubes;
}

short int Game::getColorsCount() const {
    return max_colors_count;
}

// Setter functions
void Game::setEmptyTubes(short int emptytubes) {
        empty_tubes = emptytubes;
}

void Game::setColoredTubes(short int coloredtubes) {
    
        colored_tubes = coloredtubes;
    
}

void Game::setColorsCount(short int colorscount) {
        max_colors_count = colorscount;
}

void Game::showwinmessage() {
    // Display a message when the game is won
    sf::Texture win_texture;
    sf::Sprite win_sprite;

    sf::IntRect playagainbutton{175, 380, 200, 60};
    sf::IntRect exitbutton{425, 385, 210, 65};

    if (!win_texture.loadFromFile("Resources/Images/win_screen.png")) {
        std::cout << "Error loading win screen" << std::endl;
    }
    win_sprite.setTexture(win_texture);

    sf::sleep(sf::seconds(0.5f));
    window.clear(sf::Color::White);
    window.draw(win_sprite);
    window.display();


    // Event loop for the win message
    bool waitingForEnter = true;
    while (waitingForEnter && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    waitingForEnter = false;
                    tubes.clear();
                    moveCount = 0;
                    initializeTubes();
                    render();
                    isRunning = true;
                    break;
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (playagainbutton.contains(event.mouseButton.x, event.mouseButton.y)) {
                        waitingForEnter = false;
                        tubes.clear();
                        clearhistory(historyStates);
                        moveCount = 0;
                        initializeTubes();
                        render();
                        isRunning = true;
                        break;
                    }
                    else if (exitbutton.contains(event.mouseButton.x, event.mouseButton.y)) {
                        window.close();
                    }
                }
            }
        }
    }
}

void Game::undo_function() {
    if (!historyStates.empty()) {
        std::vector<std::vector<sf::Color>> lastState = historyStates.top();
        historyStates.pop();

        for (int i = 0; i < tubes.size(); ++i) {
            tubes[i].setStack(lastState[i]);
        }

        moveCount = std::max(0, moveCount - 1); // Avoid negative moves
        std::cout << "Undo performed. Move Count: " << moveCount << std::endl;
    }
}

void Game::clearhistory(std::stack <std::vector <std::vector<sf::Color> > >& historyStates){
    while(!historyStates.empty()){
        historyStates.pop();
    }
}

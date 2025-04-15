#include "Tube.h"
#include <vector>
#include <iostream>
using namespace std;

#define line_thickness 3.f

TestTube::TestTube(float x, float y, float width, float height, float thickness) 
    : tube_x(x), tube_y(y), tube_width(width), tube_height(height), tube_thickness(thickness) {
    isSelected = false;
    
    // Creating the tube by drawing thin rectangles as thick lines
    leftLine.setSize(sf::Vector2f(line_thickness, tube_height));
    rightLine.setSize(sf::Vector2f(line_thickness, tube_height));
    bottomLine.setSize(sf::Vector2f(tube_width, line_thickness));
   
    leftLine.setPosition(x, y);
    rightLine.setPosition(x + width - line_thickness, y);
    bottomLine.setPosition(x, y + height - line_thickness);

    // Setting the color of the tube as black
    sf::Color tubeColor = sf::Color::Black;
    leftLine.setFillColor(tubeColor);
    rightLine.setFillColor(tubeColor);
    bottomLine.setFillColor(tubeColor);
}

void TestTube::addColor(sf::Color color) {
    liquidStack.push(color);
}

std::vector<sf::Color> TestTube::getStack() const {
    return liquidStack.getStack();
}

void TestTube::setStack(const std::vector<sf::Color>& new_stack) {
    for (int i = 0; i < 4; ++i) {  // clear by popping
        if (!liquidStack.isEmpty())
            liquidStack.pop();
    }
    for (auto color : new_stack) {
        liquidStack.push(color);
    }
}


sf::Color TestTube::removeColor() {
    return liquidStack.pop();
}

bool TestTube::transferLiquid(TestTube& targetTube) {
    // Check if source tube is empty or target tube is full
    if (liquidStack.isEmpty() || targetTube.liquidStack.isFull()) {
        return false;
    }

    // Check if target tube is empty or colors match
    if (targetTube.liquidStack.isEmpty() || 
        targetTube.liquidStack.peek() == liquidStack.peek()) {
        
        // sf::Color pouring_liq = liquidStack.peek();
        targetTube.addColor(liquidStack.pop());
        return true;
    }

    return false;
}

void TestTube::draw(sf::RenderWindow& window) {
    // Draw the tube structure
    window.draw(leftLine);
    window.draw(rightLine);
    window.draw(bottomLine);

    // Draw the liquid layers
    vector<sf::Color> colors = liquidStack.getStack();
    float liquidHeight = (tube_height - tube_thickness) / 4.0f; // Height of each liquid layer

    for (size_t i = 0; i < colors.size(); ++i) {
        sf::RectangleShape liquid;
        liquid.setSize(sf::Vector2f(tube_width - 2 * tube_thickness, liquidHeight));
        liquid.setPosition(tube_x + tube_thickness, tube_y + tube_height - tube_thickness - (i + 1) * liquidHeight);
        liquid.setFillColor(colors[i]);
        window.draw(liquid);
    }
}

//checks if the all the colors in the tube are the same
bool TestTube::isComplete() const {
    // Get the current stack of colors
    vector<sf::Color> colors = liquidStack.getStack();
    
    // if the tube is empty, it is not sorted
    if (colors.empty()) {
        return false;
    }
    
    // Tube must be full (4 colors) to be sorted
    if (colors.size() != 4) {
        return false;
    }
    
    // Check if all colors are the same
    sf::Color firstColor = colors[0];
    for (const auto& color : colors) {
        if (color != firstColor) {
            return false;
        }
    }
    
    return true;
}










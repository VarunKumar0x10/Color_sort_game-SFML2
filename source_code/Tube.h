//Tube.h header file 
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
using namespace std;

// Using stack to represent liquid layers
class Tubestack {
private:
    vector<sf::Color> stack;

public:
    void push(sf::Color c) {
        if (isFull()) {
            cout << "Stack Overflow!\n";
            return;
        }
        stack.push_back(c);
    }

    sf::Color pop() {
        if (!stack.empty()){
             sf::Color temp_col= stack.back();
             stack.pop_back();
             return temp_col;
        }
        else
            cout << "Stack Underflow!\n";
        return sf::Color::White;
    }

    sf::Color peek() const {
        if (!stack.empty())
            return stack.back();
        return sf::Color::White; // Default return value
    }

    bool isEmpty() const {
        return stack.empty();
    }

    bool isFull() const {
        return stack.size() >= 4;
    }

    int size() const {
        return stack.size();
    }

    vector<sf::Color> getStack() const {  // getter function for stack
        return stack;
    }

};

// TestTube class using ColorStack
class TestTube {
private:
    float tube_x, tube_y, tube_width, tube_height, tube_thickness;
    sf::RectangleShape leftLine, rightLine, bottomLine;
    Tubestack liquidStack; // Using the custom stack

public:
    TestTube(float x, float y, float width, float height, float thickness);

    void addColor(sf::Color color);
    sf::Color removeColor();
    bool transferLiquid(TestTube& targetTube);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return sf::Vector2f(tube_x, tube_y); }
    bool isEmpty() const { return liquidStack.isEmpty(); }
    bool isComplete() const;
    bool isSelected;
    
    float getWidth() const { return tube_width; }
    float getHeight() const { return tube_height; }

    void setStack(const std::vector<sf::Color>& new_stack);
    vector<sf::Color> getStack() const;
};

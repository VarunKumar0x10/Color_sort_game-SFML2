// AutoSolver.h
#pragma once

#include <vector>
#include <queue>
#include <unordered_set>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>

struct Move {
    int from, to;
};

class GameState {
public:
    std::vector<std::vector<int>> tubes;
    
    GameState() {}
    GameState(const std::vector<std::vector<int>>& t) : tubes(t) {}
    
    bool isSolved() const;
    std::string serialize() const;
};

class AutoSolver {
public:
    AutoSolver(const std::vector<std::vector<sf::Color>>& currentTubes);
    bool solve(std::vector<Move>& solutionMoves);
    
private:
    GameState startState;
    std::vector<Move> getMoves(const GameState& state);
    GameState apply_move(const GameState& state, const Move& move);
    bool canpour(const std::vector<int>& from, const std::vector<int>& to) const;
    
    // Convert sf::Color to int for hashing
    int colorToInt(const sf::Color& color) const;
};

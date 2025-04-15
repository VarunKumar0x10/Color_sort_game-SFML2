// AutoSolver.h
#pragma once
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include "Tube.h"

struct Move {
    int from, to;
};

struct GameState {
    std::vector<std::vector<sf::Color>> tubes;

    bool operator==(const GameState& other) const;
    std::string serialize() const;

    bool operator<(const GameState& other) const {
        return serialize() < other.serialize();
    };
};

namespace std {
    template<>
    struct hash<GameState> {
        std::size_t operator()(const GameState& state) const {
            return hash<std::string>()(state.serialize());
        }
    };
}

class AutoSolver {
       public:
            AutoSolver(std::vector<TestTube> initialTubes);
            bool solve(std::vector<Move>& solution);
        
    private:
            GameState initialState;
            bool isSolved(const GameState& state) const;
            std::vector<GameState> getNextStates(const GameState& state, std::unordered_map<GameState, Move>& moveMap);
};

// OptimizedAutoSolver.cpp
#include "AutoSolver.h"
#include <sstream>
#include <iostream>
#include <algorithm>

std::string GameState::serialize() const {
    std::stringstream ss;
    for (const auto& tube : tubes) {
        for (const auto& color : tube) {
            ss << static_cast<int>(color.toInteger()) << ",";
        }
        ss << "|";
    }
    return ss.str();
}

bool GameState::operator==(const GameState& other) const {
    return tubes == other.tubes;
}

AutoSolver::AutoSolver(std::vector<TestTube> initialTubes) {
    for (const auto& tube : initialTubes) {
        initialState.tubes.push_back(tube.getStack());
    }
}

bool AutoSolver::isSolved(const GameState& state) const {
    for (const auto& tube : state.tubes) {
        if (tube.empty()) continue;
        if (tube.size() != 4) return false;
        sf::Color c = tube[0];
        for (const auto& color : tube) {
            if (color != c) return false;
        }
    }
    return true;
}

std::vector<GameState> AutoSolver::getNextStates(const GameState& state, std::unordered_map<GameState, Move>& moveMap) {
    std::vector<GameState> nextStates;
    int n = state.tubes.size();

    for (int i = 0; i < n; ++i) {
        if (state.tubes[i].empty()) continue;
        sf::Color color = state.tubes[i].back();

        for (int j = 0; j < n; ++j) {
            if (i == j) continue;

            if (state.tubes[j].size() < 4 && (state.tubes[j].empty() || state.tubes[j].back() == color)) {
                // Optimization: Avoid pouring single unit to empty tube
                if (state.tubes[i].size() == 1 && state.tubes[j].empty()) continue;

                GameState newState = state;
                newState.tubes[j].push_back(color);
                newState.tubes[i].pop_back();

                if (moveMap.count(newState) == 0) {
                    moveMap[newState] = {i, j};
                    nextStates.push_back(newState);
                }
            }
        }
    }
    return nextStates;
}

bool AutoSolver::solve(std::vector<Move>& solution) {
    std::queue<GameState> q;
    std::unordered_set<GameState> visited;
    std::unordered_map<GameState, Move> moveMap;
    std::unordered_map<GameState, GameState> parentMap;

    q.push(initialState);
    visited.insert(initialState);
    parentMap[initialState] = initialState;

    GameState goal;
    bool found = false;

    while (!q.empty()) {
        GameState current = q.front(); q.pop();

        if (isSolved(current)) {
            goal = current;
            found = true;
            break;
        }

        for (const auto& next : getNextStates(current, moveMap)) {
            if (visited.count(next) == 0) {
                q.push(next);
                visited.insert(next);
                parentMap[next] = current;
            }
        }
    }

    if (!found) return false;

    std::vector<Move> moves;
    GameState state = goal;
    while (!(state == initialState)) {
        Move move = moveMap[state];
        moves.push_back(move);
        state = parentMap[state];
    }

    std::reverse(moves.begin(), moves.end());
    solution = moves;
    return true;
}

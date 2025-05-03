//Autosolver.cpp
#include "AutoSolver.h"

bool GameState::isSolved() const {
    for(const auto& tube : tubes) {
        if(tube.empty()) continue;
        if(tube.size() != 4) return false;
        for (int i=1; i < tube.size(); ++i) {
            if(tube[i] != tube[0]) return false;
        }
    }
    return true;
}

//represent all the testtubes as a string
std::string GameState::serialize() const {
    std::string result;
    for (const auto& tube : tubes) {
        for (const auto& color : tube) {
            result += std::to_string(color);
        }
        result += "|";
    }
    return result;
}

//Constructor, stores input gamestate into startState variable
AutoSolver::AutoSolver(const std::vector<std::vector<sf::Color>>& currentTubes) {
    std::vector<std::vector<int>> Tube_as_int;
    
    for(const auto& tube : currentTubes) {
        std::vector<int> intTube;
        for(const auto& color : tube) {
            intTube.push_back(colorToInt(color));
        }
        Tube_as_int.push_back(intTube);
    }
    
    startState = GameState(Tube_as_int);
}



int AutoSolver::colorToInt(const sf::Color& color) const {
    if (color == sf::Color::Red)     return 1;
    if (color == sf::Color::Blue)    return 2;
    if (color == sf::Color::Green)   return 3;
    if (color == sf::Color::Yellow)  return 4;
    if (color == sf::Color::Magenta) return 5;
    if (color == sf::Color::Cyan)    return 6;
    return -1; 
}

bool AutoSolver::canpour(const std::vector<int>& from, const std::vector<int>& to) const {
    if(from.empty() || to.size() >= 4) return false;
    if(to.empty()) return true;
    return from.back() == to.back();
}

//Generates all possible moves from current state
std::vector<Move> AutoSolver::getMoves(const GameState& state) {
    std::vector<Move> moves;
    int n = state.tubes.size();
    
    for(int from = 0; from < n; ++from) {
        for(int to = 0; to < n; ++to) {
            if(from != to && canpour(state.tubes[from], state.tubes[to])) {
                moves.push_back({from, to});
            }
        }
    }
    
    return moves;
}

//Generates the new state, dont modify the currenct state
GameState AutoSolver::apply_move(const GameState& state, const Move& move) {
    GameState newState = state;
    int color = newState.tubes[move.from].back();
    newState.tubes[move.from].pop_back();
    newState.tubes[move.to].push_back(color);
    return newState;
}

bool AutoSolver::solve(std::vector<Move>& solution) {

    //Queue used to generate and explore the game tree by BFS.
    //Each node of the tree contains the game state and the set of moves to reach it
    std::queue<std::pair<GameState, std::vector<Move>>> q;
    std::unordered_set<std::string> visited;
    
    q.push({startState, {}});
    visited.insert(startState.serialize());
    
    while (!q.empty()) {
        auto [current, path] = q.front();
        q.pop();
        
        if (current.isSolved()) {
            solution = path;
            return true;
        }
        
        for (const auto& move : getMoves(current)) {
            GameState next = apply_move(current, move);
            std::string next_str = next.serialize();
            
            if (visited.find(next_str) == visited.end()) {
                visited.insert(next_str);
                std::vector<Move> newpath = path;
                newpath.push_back(move);
                q.push({next, newpath});
            }
        }
    }
    
    return false;
}

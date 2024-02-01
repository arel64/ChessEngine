#ifndef MACRO_MOVEGENERATOR_HPP
#define MACRO_MOVEGENERATOR_HPP
#include "GameState.hpp"
class MoveGenerator {
    public:
        virtual std::shared_ptr<std::vector<move>> generateMoves(std::shared_ptr<GameState> gameState);
    private:
};
#endif
#ifndef MACRO_PSEUDOLEGALMOVEGENERATOR_HPP
#define MACRO_PSEUDOLEGALMOVEGENERATOR_HPP
#include "MoveGenerator.hpp"
class PseudoLegalMoveGenerator : MoveGenerator {
    public:
        std::shared_ptr<std::vector<move>> generateMoves (std::shared_ptr<GameState> gameState);
    private:

};
#endif
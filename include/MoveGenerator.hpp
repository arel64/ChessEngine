#ifndef MACRO_MOVEGENERATOR_HPP
#define MACRO_MOVEGENERATOR_HPP
#include "GameState.hpp"
class MoveGenerator {
    public:
        virtual std::shared_ptr<std::vector<std::shared_ptr<Ply>>> generateMoves(GameState& gameState) = 0;
//        virtual ~MoveGenerator() = default;
    private:
};
#endif
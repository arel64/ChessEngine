
#include "Board.hpp"
#include "GameState.hpp"
#include <memory>
#include <iostream>
int main(int argc,char **argv)
{
    std::unique_ptr<GameState> gameState = std::make_unique<GameState>();
    gameState->printGameState();
    auto nextState = gameState->playPly(9,25);
    nextState->printGameState();
    
    return 0;
}

#include "Board.hpp"
#include "GameState.hpp"
#include <memory>
#include <iostream>
int main(int argc,char **argv)
{
    auto board = std::make_shared<Board>(new Board(1<<8,0,0,0,0,0,0,1<<17,0,0,0,0));
    board.get()->printBoard();
    std::cout << "\n" << std::endl;

    auto gameState = std::make_unique<GameState>(new GameState(board.get(), 0, WHITE, 0));
    gameState.get()->generateMoveBoard().get()->printBoard();

    return 0;
}
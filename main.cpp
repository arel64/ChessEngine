
#include "Board.hpp"
#include "GameState.hpp"
#include <memory>
#include <iostream>
int main(int argc,char **argv)
{
    auto board = std::make_shared<Board>(new Board(0,1ull<<55,0,0,1<<10,0,0,0,0,0,0,0));
    board.get()->printBoard();
    std::cout << "\n" << std::endl;


    return 0;
}
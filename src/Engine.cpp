#include "Engine.hpp"
#include "GameState.hpp"
#include <memory>
#include <vector>
void Engine::startCalculation(std::shared_ptr<GoParams>)
{
    this->isCalculating = true;
    while(!this->stopCalculation)
    {
       // auto move  = this->game->getMoveInfoVec()->at(0);
       // setMoveAsBest(move);
        this->stopCalculation = true;
    }
}

void Engine::setMoveAsBest(const Ply& move)
{
    //std::string move = getSquareName(move.sourceSquare) + getSquareName(move.targetSquare);
    //TODO:: Add promotion piece
    /*if (move.promotion_piece != NS_PieceType::NO_PIECE) {
        move += move.promotion_piece;
    }*/

    //this->bestMove = move;
}

std::string Engine::getSquareName(uint8_t square)
{
    std::string squareName = "";
    squareName += (char)('a' + (square % 8));
    squareName += (char)('1' + (square / 8));
    return squareName;
}
uint8_t Engine::getSquareFromName(std::string squareName)
{
    uint8_t square = 0;
    square += (squareName[0] - 'a');
    square += (squareName[1] - '1') * 8;
    return square;
}

void Engine::loadPositon(const std::string& fen,std::string moves)
{  
    //TODO:: This should stop calculations
    this->game = std::make_shared<GameState>(fen);
    std::vector<std::string> moveVec = Helper::split(moves,' ');
    //Remove the "moves" string
    if(moveVec.size() > 0)
    {
        moveVec.erase(moveVec.begin());
        for(auto move : moveVec)
        {
            uint8_t sourceSquare = getSquareFromName(move.substr(0,2));
            uint8_t targetSquare = getSquareFromName(move.substr(2,4));
            std::string promotion = move.substr(4,5);
//            this->game = this->game->playPly(sourceSquare,targetSquare);
            this->game->printGameState();
        }

    }
    
}

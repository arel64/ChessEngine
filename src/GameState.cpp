#include "GameState.hpp"
#include "Color.hpp"
#include "Board.hpp"
#include <cstdint>
#include <memory>


void GameState::printGameState()
{
    m_board->printBoard();
    std::cout << "Player to move: " << (m_playerToMove == WHITE ? "White" : "Black") << std::endl;
    std::cout << "Castle Info: " << (int)m_castleInfo << std::endl;
    std::cout << "En Passant Square: " << (int)m_enPassantSquare << std::endl;
}
GameState::GameState(std::string fen) : GameState()
{
    if(!fen.empty())
    {
        throw std::runtime_error("Not implemented");   
    }
}
GameState::GameState(): GameState(std::make_shared<Board>(),0,WHITE,0){}
GameState::GameState(std::shared_ptr<Board>board,uint8_t castleInfo,Color playerToMove,uint16_t enPassantSquare)
{
    m_castleInfo = castleInfo;
    m_playerToMove = playerToMove;
    m_enPassantSquare = enPassantSquare;
    m_board = board;    
}
GameState::GameState(const GameState& other,Ply move) : 
    GameState(std::make_shared<Board>( m_board,move),other.m_castleInfo,~other.m_playerToMove,other.m_enPassantSquare){}
GameState::~GameState()
{
}
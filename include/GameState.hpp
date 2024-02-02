#ifndef MACRO_GAMESTATE_HPP
#define MACRO_GAMESTATE_HPP
#include "Board.hpp"
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <memory>
#include <vector> 
#include <bit>

#define SHIFT_SIGN(color,pawns,num) (color == WHITE ? (pawns << num) : (pawns >> num))

#define IS_SLIDING(piece) (piece == BISHOP || piece == ROOK || piece == QUEEN)
class GameState
{
  public:
    std::shared_ptr<GameState> playPly(uint8_t sourceSquare,uint8_t targetSquare);
    auto getBoard(){return m_board;};
    Color getPlayerToMove(){return m_playerToMove;};
    void printGameState();
    GameState(std::string fen);
    GameState(std::shared_ptr<Board>, uint8_t castleInfo, Color playerToMove, uint16_t enPassantSquare);
    GameState(GameState const *gameState) : GameState(gameState->m_board, gameState->m_castleInfo, gameState->m_playerToMove, gameState->m_enPassantSquare) {}
    GameState(const GameState& other,ply move);
    GameState();        
    ~GameState();
  public:

  private:

    std::shared_ptr<Board> m_board;
    /*
    # 8 bit integer to store castle information that cannot be infered from the board
    # 0-3: (white) king moved, left rook moved, right rook moved, free bit
    # 4-7: (black) king moved, left rook moved, right rook moved, free bit
    */
    uint8_t m_castleInfo;

    Color m_playerToMove;
    uint16_t m_enPassantSquare;
    
};
#endif
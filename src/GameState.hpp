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
    auto getMoveInfoVec(){return m_moveInfoVec;};
    auto getBoard(){return m_board;};
    void printGameState();
    std::shared_ptr<std::vector<moveInfo>> generateMove(PieceType p);
    std::shared_ptr<std::vector<std::pair<uint64_t,uint8_t>>>generateMoveBitBoard(PieceType p);
    GameState(std::string fen);
    GameState(std::shared_ptr<Board>, uint8_t castleInfo, Color playerToMove, uint16_t enPassantSquare);
    GameState(GameState const *gameState) : GameState(gameState->m_board, gameState->m_castleInfo, gameState->m_playerToMove, gameState->m_enPassantSquare) {}
    GameState();        
    ~GameState();
  public:

  private:

    std::shared_ptr<Board> m_board;
    std::shared_ptr<std::vector<moveInfo>> m_moveInfoVec;
    /*
    # 8 bit integer to store castle information that cannot be infered from the board
    # 0-3: (white) king moved, left rook moved, right rook moved, free bit
    # 4-7: (black) king moved, left rook moved, right rook moved, free bit
    */
    uint8_t m_castleInfo;


    Color m_playerToMove;
    uint16_t m_enPassantSquare;

  private:
    std::shared_ptr<std::vector<moveInfo>> generateMoveInfoVec();
    uint64_t bishopMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square); 
    uint64_t rookMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square); 
    uint64_t pawnMove(uint64_t &piece, uint64_t &pieceClipFileA, uint64_t &pieceClipFileH);
    uint64_t knightMove(uint64_t &piece_moves, uint64_t pieceClipFileAB, uint64_t pieceClipFileA, uint64_t pieceClipFileH, uint64_t pieceClipFileGH);
    uint64_t kingMove(uint64_t pieceClipFileA, uint64_t piece, uint64_t pieceClipFileH);
    uint64_t directionSquareUpdate(uint64_t currentMoves, uint64_t blockingInc, uint64_t blockingExclude, uint64_t squareBitboard, bool directions[4]);

};
#endif
#ifndef MACRO_GAMESTATE_HPP
#define MACRO_GAMESTATE_HPP
#include "Board.hpp"
#include <cstdint>
#include <stdint.h>
#include <memory>
    class GameState
    {
      public:
        std::unique_ptr<Board>  generateMoveBoard();
        uint64_t generatePieceMoves(PieceType piece);
        GameState(Board*,uint8_t castleInfo,Color playerToMove,uint16_t enPassantSquare);
        GameState(GameState const *gameState) : GameState(gameState->m_board.get(), gameState->m_castleInfo, gameState->m_playerToMove, gameState->m_enPassantSquare) {}
        GameState();        
        ~GameState();
      private:

        std::unique_ptr<Board> m_board;
        /*
        # 8 bit integer to store castle information that cannot be infered from the board
        # 0-3: (white) king moved, left rook moved, right rook moved, free bit
        # 4-7: (black) king moved, left rook moved, right rook moved, free bit
        */
        uint8_t m_castleInfo;
        /*
            White is 1 and black is 0
        */
        Color m_playerToMove;
        
        /*
            TODO:: To be implemented
        */
        uint16_t m_enPassantSquare;
    
      private:
        uint64_t generateKingMoves(Color color);
        uint64_t generateQueenMoves(Color color);
        uint64_t generateRookMoves(Color color);
        uint64_t generateBishopMoves(Color color);
        uint64_t generateKnightMoves(Color color);
        uint64_t generatePawnMoves(Color color);
    };
#endif
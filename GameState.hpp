#ifndef MACRO_GAMESTATE_HPP
#define MACRO_GAMESTATE_HPP
#include "Board.hpp"
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <memory>
#include <vector> 
#include <bit>
    class GameState
    {
      public:
        std::shared_ptr<GameState> playPly(uint8_t sourceSquare,uint8_t targetSquare);
        
        std::shared_ptr<std::vector<moveInfo>> generatePieceMoves(PieceType piece);
        uint64_t getPositiveRayAttack(uint8_t square,Directions direction);
        uint64_t getNegativeRayAttack(uint8_t square,Directions direction);
        auto getMoveInfoVec(){return m_moveInfoVec;};
        void printGameState();
        GameState(std::shared_ptr<Board>,uint8_t castleInfo,Color playerToMove,uint16_t enPassantSquare);
        GameState(GameState const *gameState) : GameState(gameState->m_board, gameState->m_castleInfo, gameState->m_playerToMove, gameState->m_enPassantSquare) {}
        GameState() : GameState(std::make_shared<Board>(new Board()),0,WHITE,0) {};        
        ~GameState();
      public:
        inline static uint64_t RAY_ATTACKS[64][8];

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
        
        /*
            TODO:: To be implemented
        */
        uint16_t m_enPassantSquare;

        inline static bool isRayAttacksInitialized;

      private:
        std::shared_ptr<std::vector<moveInfo>> generateMoveInfoVec();
        std::shared_ptr<std::vector<moveInfo>> generateMove(PieceType p);
        uint64_t initRayAttacksForSquare(uint8_t square,int direction);
        uint64_t getRayAttack(bool positive,uint8_t square,Directions direction);
        void     initRayAttacks();
    };
#endif
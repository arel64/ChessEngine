#ifndef MACRO_PLY_HPP
#define MACRO_PLY_HPP
    #include "PieceColoredType.hpp"
    #include <cstdint>
    class Ply{
        public:
            uint8_t sourceSquare;
            uint8_t targetSquare;

            uint8_t castleInfo;
            bool passant_capture;

            PieceType promotion_piece;

            Ply(uint8_t sourceSquare, uint8_t targetSquare, uint8_t castleInfo, bool passant_capture, PieceType promotion_piece)
        {
            this->sourceSquare = sourceSquare;
            this->targetSquare = targetSquare;
            this->castleInfo = castleInfo;
            this->passant_capture = passant_capture;
            this->promotion_piece = promotion_piece;
        }
    };
#endif
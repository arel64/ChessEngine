#ifndef MACRO_PIECE_HPP
#define MACRO_PIECE_HPP
    #include "Color.hpp"
    #include <cstdint>
    class PieceColoredType
    {
        public:
            PieceColoredType(PlayerColors::Color color, uint64_t bitboard);
            PlayerColors::Color getColor();
            uint64_t getBitboard();
        private:
            PlayerColors::Color color;
            uint64_t bitboard;
    };
#endif
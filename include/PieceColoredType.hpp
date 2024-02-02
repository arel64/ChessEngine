#ifndef MACRO_PIECE_HPP
#define MACRO_PIECE_HPP
    #include "Color.hpp"
    #include <cstdint>
    #include <memory>
    enum PieceType
    {
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING,
        NO_PIECE
    };
    static const PieceType PIECE_TYPES []= {PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING};

    class PieceFamily
    {
        public:
            PieceFamily(PieceType type, PlayerColors::Color color)
            {
                this->type = type;
                this->color = color;
            }
            PlayerColors::Color getColor() {return color;}
            PieceType getType() {return type;}
        private:
            PlayerColors::Color color;
            PieceType type;
    };
    class PieceFamilyBitboard
    {
        public:
            PieceFamilyBitboard(PieceType type, PlayerColors::Color color, uint64_t bitboard) : 
                PieceFamilyBitboard(std::make_shared<PieceFamily>(type,color),bitboard) {}
            PieceFamilyBitboard(std::shared_ptr<PieceFamily> pieceFamily, uint64_t bitboard)
            {
                this->pieceFamily = pieceFamily;
                this->bitboard = bitboard;
            }
            uint64_t getBitboard() {return bitboard;}
            std::shared_ptr<PieceFamily> getPieceFamily() {return pieceFamily;} 
        private:
            uint64_t bitboard;
            std::shared_ptr<PieceFamily> pieceFamily;
    };
#endif
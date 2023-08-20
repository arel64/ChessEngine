#ifndef MACRO_BOARD_HPP
#define MACRO_BOARD_HPP
    #include <cstdint>
    #include <cstdio>
    #include <cassert>
    #include <memory>
    #include <utility>
    #include <vector>

    
    #define BOARD_DIM (8)
    #define BOARD_SIZE BOARD_DIM * BOARD_DIM
    #define NO_MOVE (0)
    #define ALL_WHITE_PIECES (m_whitePawns | m_whiteKnights | m_whiteBishops | m_whiteRooks | m_whiteQueens | m_whiteKing)
    #define ALL_BLACK_PIECES (m_blackPawns | m_blackKnights | m_blackBishops | m_blackRooks | m_blackQueens | m_blackKing)
    #define ALL_PIECES (ALL_WHITE_PIECES | ALL_BLACK_PIECES)

    #define A_FILE 0x0101010101010101
    #define FIRST_RANK 0x00000000000000FF
    #define CLEAR_FILE_MASK(file_num) (~(A_FILE << (file_num)))

    #define WHITE_PAWN_START (0x000000000000FF00)
    #define WHITE_KNIGHT_START (0x0000000000000042)
    #define WHITE_BISHOP_START (0x0000000000000024)
    #define WHITE_ROOK_START (0x0000000000000081)
    #define WHITE_QUEEN_START (0x0000000000000008)
    #define WHITE_KING_START (0x0000000000000010)

    #define BLACK_PAWN_START (0x00FF000000000000)
    #define BLACK_KNIGHT_START (0x4200000000000000)
    #define BLACK_BISHOP_START (0x2400000000000000)
    #define BLACK_ROOK_START (0x8100000000000000)
    #define BLACK_QUEEN_START (0x0800000000000000)
    #define BLACK_KING_START (0x1000000000000000)
    
    enum Directions
    {
        NORTH = 0,
        SOUTH,
        EAST,
        WEST,
        NORTH_EAST,
        NORTH_WEST,
        SOUTH_EAST,
        SOUTH_WEST
    };
    enum RANKS
    {
        RANK_1 = 0,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8
    };
    enum FILES
    {
        FILE_A = 0,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H
    };
    namespace NS_PieceType
    {
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
        static const PieceType AllPieces[] = {PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING};
        static const PieceType AllPiecesEnum[] = {PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING,NO_PIECE};
      
    }
    using namespace NS_PieceType;

    namespace NS_Color
    {
        enum Color
        {
            WHITE=0,
            BLACK,
            ALL,
            NONE
        };
        static const Color AllColors[] = {WHITE,BLACK};
        static const Color AllColorsEnum[] = {WHITE,BLACK,ALL,NONE};
    }
    
    using namespace NS_Color;
    
    struct moveInfo{
      uint8_t sourceSquare;
      uint8_t targetSquare;


      uint64_t moveBoard; 

      uint8_t castleInfo;
      bool passant_capture;   
      uint16_t enPassantSquare;

      bool promotion;
      PieceType promotion_piece;

    }typedef moveInfo;
    Color operator~(Color color);
    class Board
    {
        public:
            Board();
            Board(Board const *other) :
                Board(other->m_whitePawns, other->m_whiteKnights, other->m_whiteBishops, other->m_whiteRooks, other->m_whiteQueens, other->m_whiteKing,
                         other->m_blackPawns, other->m_blackKnights, other->m_blackBishops, other->m_blackRooks, other->m_blackQueens, other->m_blackKing){};
            Board(uint64_t whitePawns, uint64_t whiteKnights, uint64_t whiteBishops, uint64_t whiteRooks, uint64_t whiteQueens, uint64_t whiteKing,
                uint64_t blackPawns, uint64_t blackKnights, uint64_t blackBishops, uint64_t blackRooks, uint64_t blackQueens, uint64_t blackKing);
            Board(std::shared_ptr<Board>board,moveInfo move);
            void printBoard();
            uint64_t getPieceBitBoard(PieceType pieceType, Color color);
            std::pair<PieceType, Color> getPieceOnSquare(uint8_t square);
            inline static uint8_t getSquare(uint64_t occ) { return occ==0 ?  0: std::__countr_zero(occ);}
            constexpr uint64_t getPiecesByColor(Color color){{
                if(color == WHITE)
                {
                    return ALL_WHITE_PIECES;
                }
                else if(color == BLACK)
                {
                    return ALL_BLACK_PIECES;
                }
                else{
                    return ALL_PIECES;
                }
            }}
        private:


        /*
            This is a bitboard representation of the board
            0 means empty square and 1 means occupied square of the respective color and piece
            0th bit is a1, 1st bit is b1, 2nd bit is c1 wheras 8th bit is a2 and so on
        */
            uint64_t m_whitePawns;
            uint64_t m_whiteKnights;
            uint64_t m_whiteBishops;
            uint64_t m_whiteRooks;
            uint64_t m_whiteQueens;
            uint64_t m_whiteKing;
            uint64_t m_blackPawns;
            uint64_t m_blackKnights;
            uint64_t m_blackBishops;
            uint64_t m_blackRooks;
            uint64_t m_blackQueens;
            uint64_t m_blackKing;
            //std::vector<uint64_t> m_allPieces;
        private:
            void setPieceBitBoard(PieceType pieceType, Color color,int64_t bitBoard);

    };
#endif
#include "Board.hpp"
#include <cstdint>


Board::Board()
{
    /*
        Starting Position
    */
    m_whitePawns   = WHITE_PAWN_START;
    m_whiteKnights = WHITE_KNIGHT_START;
    m_whiteBishops = WHITE_BISHOP_START;
    m_whiteRooks   = WHITE_ROOK_START;
    m_whiteQueens  = WHITE_QUEEN_START;
    m_whiteKing    = WHITE_KING_START;
    m_blackPawns   = BLACK_PAWN_START;
    m_blackKnights = BLACK_KNIGHT_START;
    m_blackBishops = BLACK_BISHOP_START;
    m_blackRooks   = BLACK_ROOK_START;
    m_blackQueens  = BLACK_QUEEN_START;
    m_blackKing    = BLACK_KING_START;
}
Board::Board(uint64_t whitePawns, uint64_t whiteKnights, uint64_t whiteBishops, uint64_t whiteRooks, uint64_t whiteQueens, uint64_t whiteKing,
                uint64_t blackPawns, uint64_t blackKnights, uint64_t blackBishops, uint64_t blackRooks, uint64_t blackQueens, uint64_t blackKing)
{
    m_whitePawns   = whitePawns;
    m_whiteKnights = whiteKnights;
    m_whiteBishops = whiteBishops;
    m_whiteRooks   = whiteRooks;
    m_whiteQueens  = whiteQueens;
    m_whiteKing    = whiteKing;
    m_blackPawns   = blackPawns;
    m_blackKnights = blackKnights;
    m_blackBishops = blackBishops;
    m_blackRooks   = blackRooks;
    m_blackQueens  = blackQueens;
    m_blackKing    = blackKing;
}

uint64_t Board::getPieceBitBoard(PieceType pieceType, Color color)
{
    switch(pieceType)
    {
        case PAWN:
            if(color == WHITE)
            {
                return m_whitePawns;
            }
            else
            {
                return m_blackPawns;
            }
        case KNIGHT:
            if(color == WHITE)
            {
                return m_whiteKnights;
            }
            else
            {
                return m_blackKnights;
            }
        case BISHOP:
            if(color == WHITE)
            {
                return m_whiteBishops;
            }
            else
            {
                return m_blackBishops;
            }
        case ROOK:
            if(color == WHITE)
            {
                return m_whiteRooks;
            }
            else
            {
                return m_blackRooks;
            }
        case QUEEN:
            if(color == WHITE)
            {
                return m_whiteQueens;
            }
            else
            {
                return m_blackQueens;
            }
        case KING:
            if(color == WHITE)
            {
                return m_whiteKing;
            }
            else
            {
                return m_blackKing;
            }
        default:
            assert(false);
    }
}
Color operator~(Color color)
{
    return static_cast<Color>(~static_cast<int>(color));
}
void Board::printBoard()
{
    /*
        TODO:: REDUCE CODE DUPLICATION USING ENUMS
    */
    uint64_t white = ALL_WHITE_PIECES;
    uint64_t black = ALL_BLACK_PIECES;
    constexpr uint64_t one = 1;
    for(uint64_t i = 0; i < BOARD_SIZE; i++)
    {
        uint64_t shiftingOne = one << i;
        if((white & shiftingOne))
        {
            if(m_whitePawns & shiftingOne)
            {
                printf("P  ");
            }
            else if(m_whiteKnights & shiftingOne)
            {
                printf("N  ");
            }
            else if( m_whiteBishops & shiftingOne)
            {
                printf("B  ");
            }
            else if(m_whiteRooks & shiftingOne)
            {
                printf("R  ");
            } 
            else if(m_whiteQueens & shiftingOne)
            {
                printf("Q  ");
            }
            else if(m_whiteKing & shiftingOne)
            {
                printf("K  ");
            }
            else
            {
                assert(false);
            }
            
        }
        else if(black & shiftingOne)
        {
            if(m_blackPawns & shiftingOne)
            {
                printf("BP ");
            }
            else if(m_blackKnights & shiftingOne)
            {
                printf("BN ");
            }
            else if( m_blackBishops & shiftingOne)
            {
                printf("BB ");
            }
            else if(m_blackRooks & shiftingOne)
            {
                printf("BR ");
            } 
            else if(m_blackQueens & shiftingOne)
            {
                printf("BQ ");
            }
            else if(m_blackKing & shiftingOne)
            {
                printf("BK ");
            }
            else
            {
                assert(false);
            }
        }
        else
        {
            printf("L  ");
        }
        if(i % BOARD_DIM == 7)
        {
            printf("\n");
        }
    }
}
Board::Board(std::shared_ptr<Board>board,moveInfo move)
{
    
}

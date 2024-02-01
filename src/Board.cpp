#include "Board.hpp"
#include <cstdint>
using namespace PlayerColors;

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
    
    /*m_allPieces = std::vector<uint64_t>{m_whitePawns,m_whiteKnights,m_whiteBishops,m_whiteRooks,m_whiteQueens,m_whiteKing,
                                        m_blackPawns,m_blackKnights,m_blackBishops,m_blackRooks,m_blackQueens,m_blackKing};*/

}
void Board::setPieceBitBoard(PieceType pieceType, Color color,int64_t bitBoard)
{
    switch(pieceType)
    {
        case PAWN:
            if(color == WHITE)
            {
                m_whitePawns = bitBoard;
            }
            else
            {
                m_blackPawns = bitBoard;
            }
            break;
        case KNIGHT:
            if(color == WHITE)
            {
                m_whiteKnights = bitBoard;
            }
            else
            {
                m_blackKnights = bitBoard;
            }
            break;
        case BISHOP:
            if(color == WHITE)
            {
                m_whiteBishops = bitBoard;
            }
            else
            {
                m_blackBishops = bitBoard;
            }
            break;
        case ROOK:
            if(color == WHITE)
            {
                m_whiteRooks = bitBoard;
            }
            else
            {
                m_blackRooks = bitBoard;
            }
            break;
        case QUEEN:
            if(color == WHITE)
            {
                m_whiteQueens = bitBoard;
            }
            else
            {
                m_blackQueens = bitBoard;
            }
            break;
        case KING:
            if(color == WHITE)
            {
                m_whiteKing = bitBoard;
            }
            else
            {
                m_blackKing = bitBoard;
            }
            break;
    }
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
std::pair<PieceType, Color> Board::getPieceOnSquare(uint8_t square)
{
    for (auto pieceType : NS_PieceType::AllPieces)
    {
        for (auto color : PlayerColors::PLAYER_COLORS)
        {
            if (getPieceBitBoard(pieceType, color) & (1ull << square))
            {
                return {pieceType, color};
            }
        }
    }
    return {PieceType::NO_PIECE,Color::NONE};
}

Board::Board(std::shared_ptr<Board>board,moveInfo move)
{
    m_whitePawns   = board->getPieceBitBoard(PAWN,WHITE);
    m_whiteKnights = board->getPieceBitBoard(KNIGHT,WHITE);
    m_whiteBishops = board->getPieceBitBoard(BISHOP,WHITE);
    m_whiteRooks   = board->getPieceBitBoard(ROOK,WHITE);
    m_whiteQueens  = board->getPieceBitBoard(QUEEN,WHITE);
    m_whiteKing    = board->getPieceBitBoard(KING,WHITE);
    m_blackPawns   = board->getPieceBitBoard(PAWN,BLACK);
    m_blackKnights = board->getPieceBitBoard(KNIGHT,BLACK);
    m_blackBishops = board->getPieceBitBoard(BISHOP,BLACK);
    m_blackRooks   = board->getPieceBitBoard(ROOK,BLACK);
    m_blackQueens  = board->getPieceBitBoard(QUEEN,BLACK);
    m_blackKing    = board->getPieceBitBoard(KING,BLACK);


    auto sourcePiece = getPieceOnSquare(move.sourceSquare);
    auto targetPiece = getPieceOnSquare(move.targetSquare);
    if(targetPiece.first != NO_PIECE)
    {
        int64_t modifiedTarget = board->getPieceBitBoard(targetPiece.first,targetPiece.second);
        modifiedTarget &= ~(1ull << move.targetSquare);
        setPieceBitBoard(targetPiece.first,targetPiece.second,modifiedTarget);

    }
    int64_t modifiedSource = board->getPieceBitBoard(sourcePiece.first,sourcePiece.second);
    modifiedSource &= ~(1ull << move.sourceSquare);
    modifiedSource |= (1ull << move.targetSquare);    
    setPieceBitBoard(sourcePiece.first,sourcePiece.second,modifiedSource);
    /*_allPieces = std::vector<uint64_t>{m_whitePawns,m_whiteKnights,m_whiteBishops,m_whiteRooks,m_whiteQueens,m_whiteKing,
                                        m_blackPawns,m_blackKnights,m_blackBishops,m_blackRooks,m_blackQueens,m_blackKing};*/

}
bool Board::isSquareWithinBoard(uint8_t square)
{
    if(square >= BOARD_DIM*BOARD_DIM || square < 0)
    {
        return false;
    }
    return true;
}
bool Board::isSquaresWithinSameRow(uint8_t square1,uint8_t square2)
{
    if(square1 / BOARD_DIM == square2 / BOARD_DIM)
    {
        return true;
    }
    return false;
}
bool Board::isSquareOnRank(uint8_t square,BoardRank rank)
{
    if(square / BOARD_DIM == rank)
    {
        return true;
    }
    return false;
}
bool Board::isOnSamePositiveDiagonal(uint8_t square,uint8_t squareInDirection)
{
    uint8_t squareRank = square / BOARD_DIM;
    uint8_t squareFile = square % BOARD_DIM;
    uint8_t squareInDirectionRank = squareInDirection / BOARD_DIM;
    uint8_t squareInDirectionFile = squareInDirection % BOARD_DIM;
    if(squareRank - squareInDirectionRank == squareFile - squareInDirectionFile)
    {
        return true;
    }
    return false;
}
bool Board::isOnSameNegativeDiagonal(uint8_t square,uint8_t squareInDirection)
{
    uint8_t squareRank = square / BOARD_DIM;
    uint8_t squareFile = square % BOARD_DIM;
    uint8_t squareInDirectionRank = squareInDirection / BOARD_DIM;
    uint8_t squareInDirectionFile = squareInDirection % BOARD_DIM;
    
    if(squareRank - squareInDirectionRank ==  -(squareFile - squareInDirectionFile))
    {
        return true;
    }
    return false;
}

bool Board::isSquareWithinDirection(uint8_t square, uint8_t squareInDirection, Direction direction)
{
    switch(direction)
    {
        case NORTH:
        case SOUTH:
            return isSquareWithinBoard(square);
            break;
        case EAST:
        case WEST:
            return Board::isSquaresWithinSameRow(square,squareInDirection);
            break;
        case NORTH_EAST:
        case SOUTH_WEST:
            return Board::isOnSamePositiveDiagonal(square,squareInDirection);
        case NORTH_WEST:
        case SOUTH_EAST:
            return Board::isOnSameNegativeDiagonal(square,squareInDirection);
        break;
         
    }
    return false;
}
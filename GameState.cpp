#include "GameState.hpp"
#include "Board.hpp"
#include <cstdint>
#include <memory>

#define SHIFT_SIGN(color,pawns,num) (color == WHITE ? (pawns << num) : (pawns >> num))


std::unique_ptr<Board> GameState::generateMoveBoard()
{
    
    /*
        King Moves
    */
    uint64_t kingLegal      = generatePieceMoves(KING);
    uint64_t queenLegal     = generatePieceMoves(QUEEN);
    uint64_t rookLeagal     = generatePieceMoves(ROOK);
    uint64_t bishopLegal    = generatePieceMoves(BISHOP);
    uint64_t knightLegal    = generatePieceMoves(KNIGHT);
    uint64_t pawnLegal      = generatePieceMoves(PAWN);

    switch (m_playerToMove) {
        case WHITE:
               return std::make_unique<Board>(pawnLegal,knightLegal,bishopLegal,rookLeagal,queenLegal,kingLegal,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE);
            break;
        case BLACK:
                return std::make_unique<Board>(NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE,pawnLegal,knightLegal,bishopLegal,rookLeagal,queenLegal,kingLegal);
            break;
        default: assert(false);
    }
    return nullptr;
}
uint64_t GameState::generatePieceMoves(PieceType piece)
{
    switch(piece)
    {
        case KING:
            return generateKingMoves();
            break;
        case QUEEN:
            return generateQueenMoves();
            break;
        case ROOK:
            return generateRookMoves();
            break;
        case BISHOP:
            return generateBishopMoves();
            break;
        case KNIGHT:
            return generateKnightMoves();
            break;
        case PAWN:
            return generatePawnMoves();
            break;
        default:
            return 0;
    }
}

uint64_t GameState::generateKingMoves ()
{
    uint64_t king = m_board->getPieceBitBoard(KING, m_playerToMove);
    uint64_t kingClipFileA = king & CLEAR_FILE_MASK(FILE_A);
    uint64_t kingClipFileH = king & CLEAR_FILE_MASK(FILE_H);
    uint64_t king_moves = (
            (kingClipFileA << 7) | (king << 8)          | (kingClipFileH << 9) | (kingClipFileA >> 1) |
            (kingClipFileH << 1) | (kingClipFileA >> 9) | (king >> 8)          | (kingClipFileH >> 7)
        );
    uint64_t king_moves_legal = king_moves & ~m_board->getPiecesByColor(m_playerToMove);
    return king_moves_legal;
}
uint64_t GameState::generateKnightMoves()
{
    uint64_t knights = m_board->getPieceBitBoard(KNIGHT, m_playerToMove);
    uint64_t knightClipFileA = knights & CLEAR_FILE_MASK(FILE_A);
    uint64_t knightClipFileAB = knightClipFileA & CLEAR_FILE_MASK(FILE_B);
    uint64_t knightClipFileH = knights & CLEAR_FILE_MASK(FILE_H);
    uint64_t knightClipFileGH = knightClipFileH & CLEAR_FILE_MASK(FILE_G);

    uint64_t knight_moves = (
            (knightClipFileAB << 6) | (knightClipFileA << 15) | (knightClipFileH << 17) | (knightClipFileGH << 10) |
            (knightClipFileGH >> 6) | (knightClipFileH >> 15) | (knightClipFileA >> 17) | (knightClipFileAB >> 10)
        );
    uint64_t knight_moves_legal = knight_moves & ~m_board->getPiecesByColor(m_playerToMove);
    return knight_moves_legal;
}
uint64_t GameState::generateQueenMoves()
{
    return 0;
}
uint64_t GameState::generateRookMoves()
{
    return 0;
}
uint64_t GameState::generateBishopMoves()
{
    return 0;
}
uint64_t GameState::generatePawnMoves()
{
    uint64_t pawns = m_board->getPieceBitBoard(PAWN, m_playerToMove);
    uint64_t pawnClipFileA = pawns & CLEAR_FILE_MASK(FILE_A);
    uint64_t pawnClipFileH = pawns & CLEAR_FILE_MASK(FILE_H);
    uint64_t pawnOneStep = (SHIFT_SIGN(m_playerToMove,pawns,8));
    uint64_t pawnTwoStep = (SHIFT_SIGN(m_playerToMove,pawns,16));
    
    uint64_t validMoves = (pawnOneStep & ~m_board->getPiecesByColor(ALL)) | (pawnTwoStep & ~m_board->getPiecesByColor(ALL));
    
    uint64_t pawnAttackLeft  = SHIFT_SIGN(m_playerToMove,(((m_playerToMove==WHITE) ? pawnClipFileA:pawnClipFileH)),7);
    uint64_t pawnAttackRight = SHIFT_SIGN(m_playerToMove,(((m_playerToMove==WHITE) ? pawnClipFileH:pawnClipFileA)),9);

    uint64_t validAttacks = (pawnAttackLeft | pawnAttackRight ) & m_board->getPiecesByColor(~m_playerToMove);

    return validAttacks | validMoves;
}

GameState::GameState()
{
    m_castleInfo = 0;
    m_playerToMove = WHITE;
    m_enPassantSquare = 0; /* TODO:: FIX*/

    m_board = std::make_unique<Board>();

}
GameState::GameState(Board* board,uint8_t castleInfo,Color playerToMove,uint16_t enPassantSquare)
{
    m_castleInfo = castleInfo;
    m_playerToMove = playerToMove;
    m_enPassantSquare = enPassantSquare;
    m_board = std::make_unique<Board>(new Board(board));
}

GameState::~GameState()
{
    
}
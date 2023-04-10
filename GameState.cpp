#include "GameState.hpp"
#include "Board.hpp"
#include <cstdint>
#include <memory>

#define SHIFT_SIGN(color,pawns,num) (color == WHITE ? (pawns << num) : (pawns >> num))
uint64_t generateKingMoves(Board *,Color color);
uint64_t generateQueenMoves(Board *,Color color);
uint64_t generateRookMoves(Board *,Color color);
uint64_t generateBishopMoves(Board *,Color color);
uint64_t generateKnightMoves(Board *,Color color);
uint64_t generatePawnMoves(Board *,Color color);


std::unique_ptr<Board> GameState::generateMoveBoard()
{
    //auto moveBoard = std::make_unique<Board>(new Board(m_board.get()));
    
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
            return generateKingMoves(m_board.get(),m_playerToMove);
            break;
        case QUEEN:
            return generateQueenMoves(m_board.get(),m_playerToMove);
            break;
        case ROOK:
            return generateRookMoves(m_board.get(),m_playerToMove);
            break;
        case BISHOP:
            return generateBishopMoves(m_board.get(),m_playerToMove);
            break;
        case KNIGHT:
            return generateKnightMoves(m_board.get(),m_playerToMove);
            break;
        case PAWN:
            return generatePawnMoves(m_board.get(),m_playerToMove);
            break;
        default:
            return 0;
    }
}

uint64_t generateKingMoves (Board *board,Color color)
{
    uint64_t king = board->getPieceBitBoard(KING, color);
    uint64_t kingClipFileA = king & CLEAR_FILE_MASK(FILE_A);
    uint64_t kingClipFileH = king & CLEAR_FILE_MASK(FILE_H);
    uint64_t king_moves = (
            (kingClipFileA << 7) | (king << 8) | (kingClipFileH << 9) | (kingClipFileA >> 1) |
            (kingClipFileH << 1) | (kingClipFileA >> 9) | (king >> 8) | (kingClipFileH >> 7)
        );
    uint64_t king_moves_legal = king_moves & ~board->getPiecesByColor(color);
    return king_moves_legal;
}
uint64_t generateKnightMoves(Board *board,Color color)
{
    uint64_t knights = board->getPieceBitBoard(KNIGHT, color);
    uint64_t knightClipFileA = knights & CLEAR_FILE_MASK(FILE_A);
    uint64_t knightClipFileAB = knightClipFileA & CLEAR_FILE_MASK(FILE_B);
    uint64_t knightClipFileH = knights & CLEAR_FILE_MASK(FILE_H);
    uint64_t knightClipFileGH = knightClipFileH & CLEAR_FILE_MASK(FILE_G);

    uint64_t knight_moves = (
            (knightClipFileAB << 6) | (knightClipFileA << 15) | (knightClipFileH << 17) | (knightClipFileGH << 10) |
            (knightClipFileGH >> 6) | (knightClipFileH >> 15) | (knightClipFileA >> 17) | (knightClipFileAB >> 10)
        );
    uint64_t knight_moves_legal = knight_moves & ~board->getPiecesByColor(color);
    return knight_moves_legal;
}
uint64_t generateQueenMoves(Board *,Color color)
{
    return 0;
}
uint64_t generateRookMoves(Board *,Color color)
{
    return 0;
}
uint64_t generateBishopMoves(Board *,Color color)
{
    return 0;
}
uint64_t generatePawnMoves(Board *board,Color color)
{
    uint64_t pawns = board->getPieceBitBoard(PAWN, color);
    uint64_t pawnClipFileA = pawns & CLEAR_FILE_MASK(FILE_A);
    uint64_t pawnClipFileH = pawns & CLEAR_FILE_MASK(FILE_H);
    uint64_t pawnOneStep = (SHIFT_SIGN(color,pawns,8));
    uint64_t pawnTwoStep = (SHIFT_SIGN(color,pawns,16));
    
    uint64_t validMoves = (pawnOneStep & ~board->getPiecesByColor(ALL)) | (pawnTwoStep & ~board->getPiecesByColor(ALL));
    
    uint64_t pawnAttackLeft  = SHIFT_SIGN(color,(((color==WHITE) ? pawnClipFileA:pawnClipFileH)),7);
    uint64_t pawnAttackRight = SHIFT_SIGN(color,(((color==WHITE) ? pawnClipFileH:pawnClipFileA)),9);

    uint64_t validAttacks = (pawnAttackLeft | pawnAttackRight ) & board->getPiecesByColor(~color);

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
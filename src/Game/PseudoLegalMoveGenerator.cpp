#include "PseudoLegalMoveGenerator.hpp"
#include "Ply.hpp"
std::shared_ptr<std::vector<std::shared_ptr<Ply>>> PseudoLegalMoveGenerator::generatePseudoLegalMoves (GameState& gameState)
{
    std::shared_ptr<std::vector<std::shared_ptr<Ply>>> pseduolegalMoves = std::make_shared<std::vector<std::shared_ptr<Ply>>>();
    
    for(auto pieceType : PIECE_TYPES)
    {
        auto pieceMoves = generateMove(gameState,pieceType);
        pseduolegalMoves->insert(pseduolegalMoves->end(),pieceMoves->begin(),pieceMoves->end());
    }
    return pseduolegalMoves;
}
std::shared_ptr<std::vector<std::shared_ptr<Ply>>> PseudoLegalMoveGenerator::generateMoves (GameState& gameState)
{
    auto pseduolegalMoves = generatePseudoLegalMoves(gameState);
    removeIllegalMoves(gameState,pseduolegalMoves);

    return pseduolegalMoves;
}
void PseudoLegalMoveGenerator::removeIllegalMoves(GameState& gameState,std::shared_ptr<std::vector<std::shared_ptr<Ply>>> pseduolegalMoves)
{
    for(auto it = pseduolegalMoves->begin(); it != pseduolegalMoves->end();)
    {
        auto ply = it->get();
        GameState appliedMoveState = GameState(gameState,*ply);
        if(isEnemyKingThreatened(appliedMoveState))
        {
            it = pseduolegalMoves->erase(it);
        }
        else
        {
            it++;
        }
    }
}
bool PseudoLegalMoveGenerator::isEnemyKingThreatened(GameState& gameState)
{
    auto pseduolegalMoves = generatePseudoLegalMoves(gameState);
    for(auto& ply : *pseduolegalMoves)
    {
        PieceFamily enemyKing = PieceFamily(KING,~gameState.getPlayerToMove());
        if(ply->targetSquare == gameState.getBoard()->getPieceFamilyBitBoard(enemyKing))
        {
            return true;
        }
    }
    return false;
}
uint64_t PseudoLegalMoveGenerator::bishopMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square) 
{ 
    std::vector<std::shared_ptr<DirectionIteratorBase>> directionIterators = {
        std::make_shared<DirectionIterator<NORTH_EAST>>(square),
        std::make_shared<DirectionIterator<NORTH_WEST>>(square),
        std::make_shared<DirectionIterator<SOUTH_EAST>>(square),
        std::make_shared<DirectionIterator<SOUTH_WEST>>(square)
    };
    return slidingPieceMovesByPotentialDirections(directionIterators,blockingInc, blockingExclude);
}
uint64_t PseudoLegalMoveGenerator::rookMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square) 
{ 
    std::vector<std::shared_ptr<DirectionIteratorBase>> directionIterators = {
        std::make_shared<DirectionIterator<NORTH>>(square),
        std::make_shared<DirectionIterator<SOUTH>>(square),
        std::make_shared<DirectionIterator<EAST>>(square),
        std::make_shared<DirectionIterator<WEST>>(square)
    };
    return slidingPieceMovesByPotentialDirections(directionIterators,blockingInc, blockingExclude);
}
uint64_t PseudoLegalMoveGenerator::slidingPieceMovesByPotentialDirections(std::vector<std::shared_ptr<DirectionIteratorBase>> directionIterators, uint64_t blockingInclusive, uint64_t blockingExclusive)
{
    uint64_t movesBitboard = 0;
    for (auto &directionIterator : directionIterators)
    {
        int8_t square = directionIterator->getNextSquare();
        while (square != -1)
        {
            uint64_t squareInDirectionBitboard = SINGLE_SQUARE_BITBOARD(square);
            if(squareInDirectionBitboard & blockingInclusive)
            {
                break;
            }
            movesBitboard |= squareInDirectionBitboard;
            if(squareInDirectionBitboard & blockingExclusive)
            {
                break;
            }
            square = directionIterator->getNextSquare();
        }
    }
    return movesBitboard;
}
/**
 * TODO:: Refactor
 */
std::shared_ptr<std::vector<std::pair<uint64_t,uint8_t>>>PseudoLegalMoveGenerator::getMoveBitboardSquareCollection(GameState& gameState,PieceType p)
{
    uint64_t piece_moves = 0;
    PieceFamily pieceFamily = PieceFamily(p,gameState.getPlayerToMove());
    uint64_t pieces = gameState.getBoard()->getPieceFamilyBitBoard(pieceFamily);
    std::shared_ptr<std::vector<std::pair<uint64_t,uint8_t>>> allMoveBoards =
        std::make_shared<std::vector<std::pair<uint64_t,uint8_t>>>();
    
    while(pieces != 0)
    {             
        uint8_t square = std::__countr_zero(pieces);
        uint64_t piece = 1ull << square;
        uint64_t pieceClipFileA = 0, pieceClipFileAB = 0, pieceClipFileH = 0, pieceClipFileGH = 0;
        if(!IS_SLIDING(p))
        {
            pieceClipFileA = piece & CLEAR_FILE_MASK(FILE_A);
            pieceClipFileAB = pieceClipFileA & CLEAR_FILE_MASK(FILE_B);
            pieceClipFileH = piece & CLEAR_FILE_MASK(FILE_H);
            pieceClipFileGH = pieceClipFileH & CLEAR_FILE_MASK(FILE_G);
        }
        uint64_t playerToMoveColoredPiecesBitboard = gameState.getBoard()->getAllPiecesBitboardsByColor(gameState.getPlayerToMove());
        uint64_t otherPlayerColoredPiecesBitboard = gameState.getBoard()->getAllPiecesBitboardsByColor(~gameState.getPlayerToMove());
        switch (p) {
            case BISHOP:
                piece_moves |= bishopMove(otherPlayerColoredPiecesBitboard,playerToMoveColoredPiecesBitboard,square);
                break;
            case ROOK:
                piece_moves |= rookMove(otherPlayerColoredPiecesBitboard,playerToMoveColoredPiecesBitboard,square);
                break;
            case QUEEN:
                piece_moves |= bishopMove(otherPlayerColoredPiecesBitboard,playerToMoveColoredPiecesBitboard,square);
                piece_moves |= rookMove(otherPlayerColoredPiecesBitboard,playerToMoveColoredPiecesBitboard,square);
                break;
            case PAWN:
                piece_moves |= pawnMove(gameState,piece, pieceClipFileA, pieceClipFileH);
                break;
            case KNIGHT:
                piece_moves |= knightMove(gameState,piece_moves, pieceClipFileAB, pieceClipFileA, pieceClipFileH, pieceClipFileGH);
                break;
            case KING:
            {
                piece_moves |= kingMove(gameState,pieceClipFileA, piece, pieceClipFileH);
                break;
            }
            default:
                assert(false);
                break;
        }
        pieces &= ~piece;

        if(!(piece_moves == 0))
        {
            allMoveBoards->push_back(std::pair<uint64_t,uint8_t>(piece_moves,square));
        }
    }
    return allMoveBoards;
}

uint64_t PseudoLegalMoveGenerator::kingMove(GameState& gamestate, uint64_t pieceClipFileA, uint64_t piece, uint64_t pieceClipFileH)
{
    uint64_t king_moves = ((pieceClipFileA << 7) | (piece << 8) | (pieceClipFileH << 9) | (pieceClipFileA >> 1) |
                           (pieceClipFileH << 1) | (pieceClipFileA >> 9) | (piece >> 8) | (pieceClipFileH >> 7));
    return king_moves & ~gamestate.getBoard()->getAllPiecesBitboardsByColor(gamestate.getPlayerToMove());
}

uint64_t PseudoLegalMoveGenerator::knightMove(GameState& gamestate,uint64_t &piece_moves, uint64_t pieceClipFileAB, uint64_t pieceClipFileA, uint64_t pieceClipFileH, uint64_t pieceClipFileGH)
{
    piece_moves = ((pieceClipFileAB << 6) | (pieceClipFileA << 15) | (pieceClipFileH << 17) | (pieceClipFileGH << 10) |
                    (pieceClipFileGH >> 6) | (pieceClipFileH >> 15) | (pieceClipFileA >> 17) | (pieceClipFileAB >> 10));
    return  piece_moves & ~gamestate.getBoard()->getAllPiecesBitboardsByColor(gamestate.getPlayerToMove());
}

uint64_t PseudoLegalMoveGenerator::pawnMove(GameState& gamestate,uint64_t &piece, uint64_t &pieceClipFileA, uint64_t &pieceClipFileH)
{
    Color playerToMove = gamestate.getPlayerToMove();
    uint64_t pawnOneStep = (SHIFT_SIGN(playerToMove, piece, 8));
    uint64_t pawnTwoStep = 0;
    if ((piece & (playerToMove == WHITE ? GET_K_RANK(2) : GET_K_RANK(7))) != 0)
    {
        // Piece is on the second rank if white and 7th if black
        pawnTwoStep = (SHIFT_SIGN(playerToMove, piece, 16));
    }

    uint64_t validMoves = (pawnOneStep & ~gamestate.getBoard()->getAllPiecesBitboardsByColor(ALL)) | (pawnTwoStep & ~gamestate.getBoard()->getAllPiecesBitboardsByColor(ALL));

    uint64_t pawnAttackLeft = SHIFT_SIGN(playerToMove, (((playerToMove == WHITE) ? pieceClipFileA : pieceClipFileH)), 7);
    uint64_t pawnAttackRight = SHIFT_SIGN(playerToMove, (((playerToMove == WHITE) ? pieceClipFileH : pieceClipFileA)), 9);

    uint64_t validAttacks = (pawnAttackLeft | pawnAttackRight) & gamestate.getBoard()->getAllPiecesBitboardsByColor(~playerToMove);

    return validAttacks | validMoves;
}

std::shared_ptr<std::vector<std::shared_ptr<Ply>>> PseudoLegalMoveGenerator::generateMove(GameState& gameState, PieceType pieceType)
{    
    auto allMoveBoards = getMoveBitboardSquareCollection(gameState,pieceType);

    auto pieceMovesLegalVec = std::make_shared<std::vector<std::shared_ptr<Ply>>>();
    for(auto& [board,square] : *allMoveBoards)
    {
        while(board != 0 )
        {
            uint8_t targetSquare = std::__countr_zero(board);
            std::shared_ptr<Ply> singleMove = std::make_shared<Ply>(square,targetSquare,0,false,PieceType::NO_PIECE);

            board &= ~SINGLE_SQUARE_BITBOARD(targetSquare);
            pieceMovesLegalVec->push_back(singleMove);
        }
        
    }
    return pieceMovesLegalVec;
}

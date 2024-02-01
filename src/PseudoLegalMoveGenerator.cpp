#include "PseudoLegalMoveGenerator.hpp"

std::shared_ptr<std::vector<move>> PseudoLegalMoveGenerator::generateMoves (std::shared_ptr<GameState> gameState)
{
    std::shared_ptr<std::vector<move>> kingLegal      = generateMove(gameState,KING);
    std::shared_ptr<std::vector<move>> queenLegal     = generateMove(gameState,QUEEN);
    std::shared_ptr<std::vector<move>> rookLegal      = generateMove(gameState,ROOK);
    std::shared_ptr<std::vector<move>> bishopLegal    = generateMove(gameState,BISHOP);
    std::shared_ptr<std::vector<move>> knightLegal    = generateMove(gameState,KNIGHT);
    std::shared_ptr<std::vector<move>> pawnLegal      = generateMove(gameState,PAWN);
    
    std::shared_ptr<std::vector<move>> legalMoves = std::make_shared<std::vector<move>>();
    legalMoves->insert(legalMoves->end(),kingLegal->begin(),kingLegal->end());
    legalMoves->insert(legalMoves->end(),queenLegal->begin(),queenLegal->end());
    legalMoves->insert(legalMoves->end(),rookLegal->begin(),rookLegal->end());
    legalMoves->insert(legalMoves->end(),bishopLegal->begin(),bishopLegal->end());
    legalMoves->insert(legalMoves->end(),knightLegal->begin(),knightLegal->end());
    legalMoves->insert(legalMoves->end(),pawnLegal->begin(),pawnLegal->end());

    return legalMoves;
}
uint64_t PseudoLegalMoveGenerator::bishopMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square) 
{ 
    Direction PotentialDirections[4] = {NORTH_EAST,NORTH_WEST,SOUTH_EAST,SOUTH_WEST};
    return slidingPieceMovesByPotentialDirections(PotentialDirections, square,blockingInc, blockingExclude);
}
uint64_t PseudoLegalMoveGenerator::getMoveBitboardAndUpdateDirection(uint64_t blockingInc, uint64_t blockingExclude, uint64_t squareBitboard, bool& directions)
{
    if ((squareBitboard & blockingExclude) != 0)
    {
        directions = true;
        return 0;
    }
    if ((squareBitboard & blockingInc) != 0)
    {
        directions = true;
    }
    return squareBitboard;
}
uint8_t PseudoLegalMoveGenerator::getSquareInDirection(uint8_t square, Direction direction, uint8_t distance)
{
    return square + direction * distance;
}
uint64_t PseudoLegalMoveGenerator::rookMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square) 
{ 
    Direction PotentialDirections[4] = {NORTH,SOUTH,EAST,WEST};
    return slidingPieceMovesByPotentialDirections(PotentialDirections, square,blockingInc, blockingExclude);
}
uint64_t PseudoLegalMoveGenerator::slidingPieceMovesByPotentialDirections(Direction potentialDirections[4], uint8_t square, uint64_t blockingInc, uint64_t blockingExclude)
{
    uint64_t movesBitboard = 0;
    std::pair<Direction, bool> directionsStatusPairs[4] =
     {{potentialDirections[0], false}, {potentialDirections[1], false}, {potentialDirections[2], false}, {potentialDirections[3], false}};
    for (int i = 1; i < BOARD_DIM; i++)
    {
        for (auto &[direction, direction_status] : directionsStatusPairs)
        {
            if (direction_status)
            {
                continue;
            }
            uint8_t squareInDirection = getSquareInDirection(square, direction, i);
            direction_status = !Board::isSquareWithinDirection(square, squareInDirection, direction);
            if (direction_status)
            {
                continue;
            }
            uint64_t squareInDirectionBitboard = SINGLE_SQUARE_BITBOARD(squareInDirection);
            movesBitboard |= getMoveBitboardAndUpdateDirection(blockingInc, blockingExclude, squareInDirectionBitboard, direction_status);
        }
    }
    return movesBitboard;
}
std::shared_ptr<std::vector<std::pair<uint64_t,uint8_t>>>PseudoLegalMoveGenerator::getMoveBitboardSquareCollection(GameState& gameState,PieceType p)
{
    uint64_t piece_moves = 0;
    uint64_t pieces = gameState.getBoard()->getPieceBitBoard(p, gameState.getPlayerToMove());
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
        uint64_t playerToMoveColoredPiecesBitboard = gameState.getBoard()->getPiecesByColor(~gameState.getPlayerToMove());
        uint64_t otherPlayerColoredPiecesBitboard = gameState.getBoard()->getPiecesByColor(gameState.getPlayerToMove());
        switch (p) {
            case BISHOP:
                piece_moves |= bishopMove(playerToMoveColoredPiecesBitboard,otherPlayerColoredPiecesBitboard,square);
                break;
            case ROOK:
                piece_moves |= rookMove(playerToMoveColoredPiecesBitboard,otherPlayerColoredPiecesBitboard,square);
                break;
            case QUEEN:
                piece_moves |= bishopMove(playerToMoveColoredPiecesBitboard,otherPlayerColoredPiecesBitboard,square);
                piece_moves |= rookMove(playerToMoveColoredPiecesBitboard,otherPlayerColoredPiecesBitboard,square);
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
    return king_moves & ~gamestate.getBoard()->getPiecesByColor(gamestate.getPlayerToMove());
}

uint64_t PseudoLegalMoveGenerator::knightMove(GameState& gamestate,uint64_t &piece_moves, uint64_t pieceClipFileAB, uint64_t pieceClipFileA, uint64_t pieceClipFileH, uint64_t pieceClipFileGH)
{
    piece_moves = ((pieceClipFileAB << 6) | (pieceClipFileA << 15) | (pieceClipFileH << 17) | (pieceClipFileGH << 10) |
                    (pieceClipFileGH >> 6) | (pieceClipFileH >> 15) | (pieceClipFileA >> 17) | (pieceClipFileAB >> 10));
    return  piece_moves & ~gamestate.getBoard()->getPiecesByColor(gamestate.getPlayerToMove());
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

    uint64_t validMoves = (pawnOneStep & ~gamestate.getBoard()->getPiecesByColor(ALL)) | (pawnTwoStep & ~gamestate.getBoard()->getPiecesByColor(ALL));

    uint64_t pawnAttackLeft = SHIFT_SIGN(playerToMove, (((playerToMove == WHITE) ? pieceClipFileA : pieceClipFileH)), 7);
    uint64_t pawnAttackRight = SHIFT_SIGN(playerToMove, (((playerToMove == WHITE) ? pieceClipFileH : pieceClipFileA)), 9);

    uint64_t validAttacks = (pawnAttackLeft | pawnAttackRight) & gamestate.getBoard()->getPiecesByColor(~playerToMove);

    return validAttacks | validMoves;
}

std::shared_ptr<std::vector<move>> PseudoLegalMoveGenerator::generateMove(GameState& gameState, PieceType pieceType)
{
    auto pieceMovesLegalVec = std::make_shared<std::vector<move>>();
    auto allMoveBoards = getMoveBitboardSquareCollection(gameState,pieceType);
    for(auto& [board,square] : *allMoveBoards)
    {
        while(board !=0 )
        {
            uint8_t targetSquare = std::__countr_zero(board);

            move move = {0,0,0,0,0};
            move.sourceSquare = square;
            move.moveBoard = board & (1ull << targetSquare);
            move.targetSquare = targetSquare;
            //TODO add promotion, castle implications
            move.castleInfo = 0;
            move.enPassantSquare = 0;
            board &= ~(1ull << targetSquare);
            pieceMovesLegalVec->push_back(move);
        }
        
    }
    return pieceMovesLegalVec;
}
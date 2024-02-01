#include "GameState.hpp"
#include "Color.hpp"
#include "Board.hpp"
#include <cstdint>
#include <memory>

std::shared_ptr<std::vector<moveInfo>> GameState::generateMoveInfoVec()
{
    
   
    std::shared_ptr<std::vector<moveInfo>> kingLegal      = generateMove(KING);
    std::shared_ptr<std::vector<moveInfo>> queenLegal     = generateMove(QUEEN);
    std::shared_ptr<std::vector<moveInfo>> rookLegal      = generateMove(ROOK);
    std::shared_ptr<std::vector<moveInfo>> bishopLegal    = generateMove(BISHOP);
    std::shared_ptr<std::vector<moveInfo>> knightLegal    = generateMove(KNIGHT);
    std::shared_ptr<std::vector<moveInfo>> pawnLegal      = generateMove(PAWN);
    
    std::shared_ptr<std::vector<moveInfo>> legalMoves = std::make_shared<std::vector<moveInfo>>();
    legalMoves->insert(legalMoves->end(),kingLegal->begin(),kingLegal->end());
    legalMoves->insert(legalMoves->end(),queenLegal->begin(),queenLegal->end());
    legalMoves->insert(legalMoves->end(),rookLegal->begin(),rookLegal->end());
    legalMoves->insert(legalMoves->end(),bishopLegal->begin(),bishopLegal->end());
    legalMoves->insert(legalMoves->end(),knightLegal->begin(),knightLegal->end());
    legalMoves->insert(legalMoves->end(),pawnLegal->begin(),pawnLegal->end());

    return legalMoves;
}
std::shared_ptr<std::vector<std::pair<uint64_t,uint8_t>>>GameState::getMoveBitboardSquareCollection(PieceType p)
{
    uint64_t piece_moves = 0;
    uint64_t pieces = m_board->getPieceBitBoard(p, m_playerToMove);
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
        uint64_t playerToMoveColoredPiecesBitboard = this->getBoard()->getPiecesByColor(~m_playerToMove);
        uint64_t otherPlayerColoredPiecesBitboard = this->getBoard()->getPiecesByColor(m_playerToMove);
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
                piece_moves |= pawnMove(piece, pieceClipFileA, pieceClipFileH);
                break;
            case KNIGHT:
                piece_moves |= knightMove(piece_moves, pieceClipFileAB, pieceClipFileA, pieceClipFileH, pieceClipFileGH);
                break;
            case KING:
            {
                piece_moves |= kingMove(pieceClipFileA, piece, pieceClipFileH);
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

uint64_t GameState::kingMove(uint64_t pieceClipFileA, uint64_t piece, uint64_t pieceClipFileH)
{
    uint64_t king_moves = ((pieceClipFileA << 7) | (piece << 8) | (pieceClipFileH << 9) | (pieceClipFileA >> 1) |
                           (pieceClipFileH << 1) | (pieceClipFileA >> 9) | (piece >> 8) | (pieceClipFileH >> 7));
    return king_moves & ~m_board->getPiecesByColor(m_playerToMove);
}

uint64_t GameState::knightMove(uint64_t &piece_moves, uint64_t pieceClipFileAB, uint64_t pieceClipFileA, uint64_t pieceClipFileH, uint64_t pieceClipFileGH)
{
    piece_moves = ((pieceClipFileAB << 6) | (pieceClipFileA << 15) | (pieceClipFileH << 17) | (pieceClipFileGH << 10) |
                    (pieceClipFileGH >> 6) | (pieceClipFileH >> 15) | (pieceClipFileA >> 17) | (pieceClipFileAB >> 10));
    return  piece_moves & ~m_board->getPiecesByColor(m_playerToMove);
}

uint64_t GameState::pawnMove(uint64_t &piece, uint64_t &pieceClipFileA, uint64_t &pieceClipFileH)
{
    uint64_t pawnOneStep = (SHIFT_SIGN(m_playerToMove, piece, 8));
    uint64_t pawnTwoStep = 0;
    if ((piece & (m_playerToMove == WHITE ? GET_K_RANK(2) : GET_K_RANK(7))) != 0)
    {
        // Piece is on the second rank if white and 7th if black
        pawnTwoStep = (SHIFT_SIGN(m_playerToMove, piece, 16));
    }

    uint64_t validMoves = (pawnOneStep & ~m_board->getPiecesByColor(ALL)) | (pawnTwoStep & ~m_board->getPiecesByColor(ALL));

    uint64_t pawnAttackLeft = SHIFT_SIGN(m_playerToMove, (((m_playerToMove == WHITE) ? pieceClipFileA : pieceClipFileH)), 7);
    uint64_t pawnAttackRight = SHIFT_SIGN(m_playerToMove, (((m_playerToMove == WHITE) ? pieceClipFileH : pieceClipFileA)), 9);

    uint64_t validAttacks = (pawnAttackLeft | pawnAttackRight) & m_board->getPiecesByColor(~m_playerToMove);

    return validAttacks | validMoves;
}

std::shared_ptr<std::vector<moveInfo>> GameState::generateMove(PieceType pieceType)
{
    auto pieceMovesLegalVec = std::make_shared<std::vector<moveInfo>>();
    auto allMoveBoards = getMoveBitboardSquareCollection(pieceType);
    for(auto& [board,square] : *allMoveBoards)
    {
        while(board !=0 )
        {
            uint8_t targetSquare = std::__countr_zero(board);

            moveInfo move = {0,0,0,0,0};
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
GameState::GameState(): GameState(std::make_shared<Board>(),0,WHITE,0)
{}
GameState::GameState(std::shared_ptr<Board>board,uint8_t castleInfo,Color playerToMove,uint16_t enPassantSquare)
{

    m_castleInfo = castleInfo;
    m_playerToMove = playerToMove;
    m_enPassantSquare = enPassantSquare;
    m_board = board;
    m_moveInfoVec = generateMoveInfoVec();
    
}
std::shared_ptr<GameState> GameState::playPly(uint8_t sourceSquare,uint8_t targetSquare)
{
    auto moveBoard = getMoveInfoVec();
    uint64_t sourceSquareMask = (1ull) << sourceSquare;

    if((m_board->getPiecesByColor(m_playerToMove) & sourceSquareMask) == 0)
    {
        /*
            If source has no current turn piece, no move is possible
        */
        return nullptr;
    }
    for(auto move : *moveBoard.get())
    {
        if(move.sourceSquare != sourceSquare)
        {
            continue;
        }
        if((move.moveBoard & (1ull << targetSquare)) == 0)
        {
            /*
                Piece in location is not able to move to target location
            */
            continue;
        }
        m_castleInfo |= move.castleInfo;
        move.moveBoard &= (1ull << targetSquare);
        move.targetSquare = targetSquare;
        std::shared_ptr<Board>board = std::make_shared<Board>( m_board,move);
        return std::make_shared<GameState>(board,m_castleInfo,~m_playerToMove,move.enPassantSquare);
    }
    return nullptr;
}

void GameState::printGameState()
{
    m_board->printBoard();
    std::cout << "Player to move: " << (m_playerToMove == WHITE ? "White" : "Black") << std::endl;
    std::cout << "Castle Info: " << (int)m_castleInfo << std::endl;
    std::cout << "En Passant Square: " << (int)m_enPassantSquare << std::endl;
}
GameState::GameState(std::string fen) : GameState()
{
    if(!fen.empty())
    {
        throw std::runtime_error("Not implemented");   
    }
}
uint64_t GameState::bishopMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square) 
{ 
    Direction PotentialDirections[4] = {NORTH_EAST,NORTH_WEST,SOUTH_EAST,SOUTH_WEST};
    return slidingPieceMovesByPotentialDirections(PotentialDirections, square,blockingInc, blockingExclude);
}
uint64_t GameState::getMoveBitboardAndUpdateDirection(uint64_t blockingInc, uint64_t blockingExclude, uint64_t squareBitboard, bool& directions)
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
uint8_t GameState::getSquareInDirection(uint8_t square, Direction direction, uint8_t distance)
{
    return square + direction * distance;
}
uint64_t GameState::rookMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square) 
{ 
    Direction PotentialDirections[4] = {NORTH,SOUTH,EAST,WEST};
    return slidingPieceMovesByPotentialDirections(PotentialDirections, square,blockingInc, blockingExclude);
}
uint64_t GameState::slidingPieceMovesByPotentialDirections(Direction potentialDirections[4], uint8_t square, uint64_t blockingInc, uint64_t blockingExclude)
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
GameState::~GameState()
{
    
}
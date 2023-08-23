#include "GameState.hpp"
#include "Board.hpp"
#include <cstdint>

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

std::shared_ptr<std::vector<moveInfo>> GameState::generateMove(PieceType p)
{
    uint64_t piece_moves = 0;
    uint64_t pieces = m_board->getPieceBitBoard(p, m_playerToMove);
    std::vector<std::pair<uint64_t,uint8_t>> allMoveBoards;
    auto pieceMovesLegalVec = std::make_shared<std::vector<moveInfo>>();
    
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
        
        switch (p) {
            case BISHOP:
                piece_moves |= bishopMove(this->getBoard()->getPiecesByColor(~m_playerToMove),this->getBoard()->getPiecesByColor(m_playerToMove),square);
                break;
            case ROOK:
                piece_moves |= rookMove(this->getBoard()->getPiecesByColor(~m_playerToMove),this->getBoard()->getPiecesByColor(m_playerToMove),square);
                break;
            case QUEEN:
                piece_moves |= bishopMove(this->getBoard()->getPiecesByColor(~m_playerToMove),this->getBoard()->getPiecesByColor(m_playerToMove),square);
                piece_moves |= rookMove(this->getBoard()->getPiecesByColor(~m_playerToMove),this->getBoard()->getPiecesByColor(m_playerToMove),square);
                break;
            case PAWN:{
                uint64_t pawnOneStep = (SHIFT_SIGN(m_playerToMove,piece,8));
                uint64_t pawnTwoStep = 0;
                if((piece & (m_playerToMove == WHITE ? GET_K_RANK(2):GET_K_RANK(7))) != 0)
                {
                    //Piece is on the second rank if white and 7th if black
                    pawnTwoStep = (SHIFT_SIGN(m_playerToMove,piece,16));
                }
                

                uint64_t validMoves = (pawnOneStep & ~m_board->getPiecesByColor(ALL)) | (pawnTwoStep & ~m_board->getPiecesByColor(ALL));

                uint64_t pawnAttackLeft  = SHIFT_SIGN(m_playerToMove,(((m_playerToMove==WHITE) ? pieceClipFileA:pieceClipFileH)),7);
                uint64_t pawnAttackRight = SHIFT_SIGN(m_playerToMove,(((m_playerToMove==WHITE) ? pieceClipFileH:pieceClipFileA)),9);

                uint64_t validAttacks = (pawnAttackLeft | pawnAttackRight ) & m_board->getPiecesByColor(~m_playerToMove);

                piece_moves =  validAttacks | validMoves;
                break;
            }
            case KNIGHT:
            {
                piece_moves= (
                    (pieceClipFileAB << 6) | (pieceClipFileA << 15) | (pieceClipFileH << 17) | (pieceClipFileGH << 10) |
                    (pieceClipFileGH >> 6) | (pieceClipFileH >> 15) | (pieceClipFileA >> 17) | (pieceClipFileAB >> 10)
                );
                piece_moves = piece_moves & ~m_board->getPiecesByColor(m_playerToMove);;
                break;
            }
            case KING:
            {
                uint64_t king_moves = (
                    (pieceClipFileA << 7) | (piece << 8)          | (pieceClipFileH << 9) | (pieceClipFileA >> 1) |
                    (pieceClipFileH << 1) | (pieceClipFileA >> 9) | (piece >> 8)          | (pieceClipFileH >> 7)
                );
                piece_moves = king_moves & ~m_board->getPiecesByColor(m_playerToMove);
                break;
            }
            default:
                assert(false);
                break;
        }
        pieces &= ~piece;

        if(!(piece_moves == 0))
        {
            allMoveBoards.push_back(std::pair<uint64_t,uint8_t>(piece_moves,square));
        }
    }
    for(auto& [board,square] : allMoveBoards)
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
    if(!GameState::isRayAttacksInitialized)
    {
        GameState::isRayAttacksInitialized = true;
        initRayAttacks();
    }
    m_castleInfo = castleInfo;
    m_playerToMove = playerToMove;
    m_enPassantSquare = enPassantSquare;
    m_board = board;
    m_moveInfoVec = generateMoveInfoVec();
    
}
void GameState::initRayAttacks()
{
    /*
        This whole thing can happen offline, and then just be loaded into memory when the program starts.
        It has no real impact on preformace so I will not worry about it for now.
    */
    for (int i = 0; i < BOARD_SIZE; ++i) {
            GameState::RAY_ATTACKS[i][NORTH] = generateNorthRay(i);
            GameState::RAY_ATTACKS[i][SOUTH] = generateSouthRay(i);
            GameState::RAY_ATTACKS[i][EAST] = generateEastRay(i);
            GameState::RAY_ATTACKS[i][WEST] = generateWestRay(i);
            GameState::RAY_ATTACKS[i][DIAGONAL] = generateDiagonalRay(i);
            GameState::RAY_ATTACKS[i][ANTI_DIAGONAL] = generateAntiDiagonalRay(i);

    }
  
}

uint64_t GameState::getNegativeRayAttack(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square, Directions direction)
{
    return getRayAttack( blockingInc, blockingExclude, square,direction,false);
}

uint64_t GameState::getPositiveRayAttack(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square, Directions direction)
{
    return getRayAttack( blockingInc, blockingExclude, square,direction,true);
}
uint64_t GameState::getDiagonalAttack(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square, Directions direction)
{
    uint64_t rayValue = GameState::RAY_ATTACKS[square][direction] ^ (1ull << square);
    uint64_t blocking = (rayValue & (blockingInc | blockingExclude));
    if(blocking)
    {
        uint8_t blockingSquare;
        if(direction == DIAGONAL)
        {
            blockingSquare = std::__countr_zero(blocking);
        }
        else
        {
            blockingSquare = std::__countr_zero(blocking);
            //blockingSquare= std::__countl_zero(blocking);
        }
        rayValue ^= (GameState::RAY_ATTACKS[blockingSquare][direction] ^ (1ull << blockingSquare));
        rayValue = rayValue ^ (rayValue & blockingExclude);
    }
    return rayValue;
}
uint64_t GameState::getRayAttack(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square, Directions direction,bool positive)
{
    uint64_t rayValue = GameState::RAY_ATTACKS[square][direction];
    uint64_t blocking = rayValue & (blockingInc | blockingExclude);
    if(blocking)
    {
        if(positive)
        {
            square = std::__countr_zero(blocking);
        }
        else
        {
            ///square = std::__countl_zero(blocking);
            square = std::__countr_zero(blocking);

        }
        rayValue ^= GameState::RAY_ATTACKS[square][direction];
        rayValue = rayValue ^ (rayValue & blockingExclude);
    }
    return rayValue;
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
    uint64_t bishopMoves = 0; 
    bishopMoves |= getDiagonalAttack(blockingInc,blockingExclude,square, DIAGONAL); 
    bishopMoves |= getDiagonalAttack(blockingInc,blockingExclude,square, ANTI_DIAGONAL); 

    return bishopMoves; 
}
uint64_t GameState::rookMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square) \
{ 
    uint64_t rookMoves = 0; \
    rookMoves |= getPositiveRayAttack(blockingInc,blockingExclude,square, NORTH); 
    rookMoves |= getPositiveRayAttack(blockingInc,blockingExclude,square, EAST); 
    rookMoves |= getNegativeRayAttack(blockingInc,blockingExclude,square, SOUTH); 
    rookMoves |= getNegativeRayAttack(blockingInc,blockingExclude,square, WEST); 
    return rookMoves; 
}
uint64_t GameState::generateNorthRay(int sq) {
    return 0x0101010101010100ULL << sq;
}

uint64_t GameState::generateSouthRay(int sq) {
    return 0x0080808080808080ULL >> (63 - sq);
}

uint64_t GameState::generateEastRay(int sq) {
    const uint64_t one = 1;
    return 2*( (one << (sq|7)) - (one << sq) );
}

uint64_t GameState::generateWestRay(int sq) {
    const uint64_t one = 1;
    return (one << sq) - (one << (sq&56));
}

uint64_t GameState::generateDiagonalRay(int sq) {
   const uint64_t maindia = (0x8040201008040201);
   int diag  = (sq&7) - (sq>>3);
   return diag >= 0 ? maindia >> diag*8 : maindia << -diag*8;
}

uint64_t GameState::generateAntiDiagonalRay(int sq) {
   const uint64_t maindia = (0x0102040810204080);
   int diag  = 7 - (sq&7) - (sq>>3);
   return diag >= 0 ? maindia >> diag*8 : maindia << -diag*8;
}
GameState::~GameState()
{
    
}
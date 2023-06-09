#include "GameState.hpp"
#include "Board.hpp"


#define SHIFT_SIGN(color,pawns,num) (color == WHITE ? (pawns << num) : (pawns >> num))
#define BISHOP_MOVE(square) \
    ({ \
        uint64_t bishop_moves = 0; \
        bishop_moves |= getPositiveRayAttack(square, NORTH_WEST); \
        bishop_moves |= getPositiveRayAttack(square, NORTH_EAST); \
        bishop_moves |= getNegativeRayAttack(square, SOUTH_EAST); \
        bishop_moves |= getNegativeRayAttack(square, SOUTH_WEST); \
        bishop_moves; \
    })
#define ROOK_MOVE(square) \
    ({ \
        uint64_t rook_moves = 0; \
        rook_moves |= getPositiveRayAttack(square, NORTH); \
        rook_moves |= getPositiveRayAttack(square, EAST); \
        rook_moves |= getNegativeRayAttack(square, SOUTH); \
        rook_moves |= getNegativeRayAttack(square, WEST); \
        rook_moves; \
    })
#define IS_SLIDING(piece) (piece == BISHOP || piece == ROOK || piece == QUEEN)
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
    auto pieceMovesLegalVec = std::make_shared<std::vector<moveInfo>>(8);
    
    while(pieces != 0)
    {             
        uint8_t square = std::__countr_zero(pieces);
        uint64_t piece = 1ull << square;
        moveInfo move = {square,0,0,0,0,0};                           
        uint64_t pieceClipFileA = 0, pieceClipFileAB = 0, pieceClipFileH = 0, pieceClipFileGH = 0;
        if(IS_SLIDING(p))
        {
            pieceClipFileA = piece & CLEAR_FILE_MASK(FILE_A);
            pieceClipFileAB = pieceClipFileA & CLEAR_FILE_MASK(FILE_B);
            pieceClipFileH = piece & CLEAR_FILE_MASK(FILE_H);
            pieceClipFileGH = pieceClipFileH & CLEAR_FILE_MASK(FILE_G);
        }
        
        switch (p) {
            case BISHOP:
                piece_moves |= BISHOP_MOVE(square);
                break;
            case ROOK:
                piece_moves |= ROOK_MOVE(square);
                break;
            case QUEEN:
                piece_moves |= BISHOP_MOVE(square);
                piece_moves |= ROOK_MOVE(square);
                break;
            case PAWN:{
                uint64_t pawnOneStep = (SHIFT_SIGN(m_playerToMove,piece,8));
                uint64_t pawnTwoStep = (SHIFT_SIGN(m_playerToMove,piece,16));

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
                uint64_t kingMovesLegal = king_moves & ~m_board->getPiecesByColor(m_playerToMove);
                move.castleInfo = m_playerToMove == WHITE ? 1: 1<<4; // Signefies that this is a king moved
                break;
            }
            default:
                assert(false);
                break;
        }
        move.moveBoard = piece_moves;
        pieceMovesLegalVec->push_back(move);
        pieces &= ~piece;
    }
    return pieceMovesLegalVec;
}

GameState::GameState(std::shared_ptr<Board>board,uint8_t castleInfo,Color playerToMove,uint16_t enPassantSquare)
{
    m_castleInfo = castleInfo;
    m_playerToMove = playerToMove;
    m_enPassantSquare = enPassantSquare;
    m_board = board;
    m_moveInfoVec = generateMoveInfoVec();
    if(!GameState::isRayAttacksInitialized)
    {
        GameState::isRayAttacksInitialized = true;
        initRayAttacks();
    }
}
void GameState::initRayAttacks()
{
    /*
        This whole thing can happen offline, and then just be loaded into memory when the program starts.
        It has no real impact on preformace so I will not worry about it for now.
    */
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for(int k = 0 ; k<8 ; ++k)
        {
            uint64_t rayValue = initRayAttacksForSquare(i,k);
            GameState::RAY_ATTACKS[i][k] = rayValue;
        }       
        
        
    }
}
uint64_t GameState::initRayAttacksForSquare(uint8_t square,int direction)
{
    // Calculate the row and column offsets for the current direction
    const uint64_t one = 1;
    uint64_t rayValue = 0;
    int rowOffset = 0;
    int colOffset = 0;
    switch (direction) {
        case NORTH_WEST:    rowOffset = -1; colOffset = -1; break; // NW
        case NORTH:         rowOffset = -1; colOffset =  0; break; // N
        case NORTH_EAST:    rowOffset = -1; colOffset =  1; break; // NE
        case WEST:          rowOffset =  0; colOffset = -1; break; // W
        case EAST:          rowOffset =  0; colOffset =  1; break; // E
        case SOUTH_WEST:    rowOffset =  1; colOffset = -1; break; // SW
        case SOUTH:         rowOffset =  1; colOffset =  0; break; // S
        case SOUTH_EAST:    rowOffset =  1; colOffset =  1; break; // SE
    }

    int currentRow = square/BOARD_DIM + rowOffset;
    int currentCol = square%BOARD_DIM + colOffset;
    while (currentRow >= 0 && currentRow < BOARD_DIM && currentCol >= 0 && currentCol < BOARD_DIM) {
        // Set the bit for the current square in the ray value
        rayValue |= (one << (currentRow * BOARD_DIM + currentCol));
        
        // Move to the next square in the current direction
        currentRow += rowOffset;
        currentCol += colOffset;
    }

    return rayValue;
}
uint64_t GameState::getNegativeRayAttack(uint8_t square, Directions direction)
{
    return getRayAttack(false, square,direction);
}

uint64_t GameState::getPositiveRayAttack(uint8_t square, Directions direction)
{
    return getRayAttack(true, square,direction);
}
uint64_t GameState::getRayAttack(bool positive,uint8_t square, Directions direction)
{
    uint64_t rayValue = GameState::RAY_ATTACKS[square][direction];
    uint64_t blocking = rayValue & m_board.get()->getPiecesByColor(ALL);
    if(blocking)
    {
        if(positive)
        {
            square = std::__countr_zero(blocking);
        }
        else
        {
            square = std::__countl_zero(blocking);
        }
        rayValue ^= GameState::RAY_ATTACKS[square][direction];
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
            return nullptr;
        }
        m_castleInfo |= move.castleInfo;
        move.moveBoard &= (1ull << targetSquare);
        move.targetSquare = targetSquare;
        std::shared_ptr<Board>board = std::make_shared<Board>( new Board(m_board,move));
         return std::make_shared<GameState>(new GameState(board,m_castleInfo,~m_playerToMove,move.enPassantSquare));
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
GameState::~GameState()
{
    
}
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include "Board.hpp"
#include "GameState.hpp"
#include <memory>
#include <iostream>
bool get_bitboard_before_and_after_ply(
    PieceType piece,Color color,uint8_t source_square,uint8_t target_square,GameState gameState
        ,std::pair<uint64_t,uint64_t>& bitboards)
{
    auto sourcePieceBitboard = gameState.getBoard()->getPieceBitBoard(piece, color); 
    if(sourcePieceBitboard == 0)
    {
        return false;    
    }
    auto state =  gameState.playPly(source_square,target_square); 
    if(state==nullptr)
    {
        return false;
    }
    auto sourcePieceBitboardAfterPly = state->getBoard()->getPieceBitBoard(piece, color); 
    bitboards = std::make_pair(sourcePieceBitboard,sourcePieceBitboardAfterPly);
    return true;
}

bool single_piece_move_test(PieceType piece,Color color,uint8_t source_square,uint8_t target_square,GameState gameState)
{
    std::pair<uint64_t,uint64_t> before_and_after_bitboard;
    if(!get_bitboard_before_and_after_ply(piece,color,source_square,target_square,gameState,before_and_after_bitboard))
    {
        return false;
    }
    return  (before_and_after_bitboard.first == SINGLE_SQUARE_BITBOARD(source_square)) && (before_and_after_bitboard.second == SINGLE_SQUARE_BITBOARD(target_square) );
} 

bool single_piece_move_test_capture(PieceType attacking_piece,Color attacking_piece_color,uint8_t source_square,uint8_t target_square,GameState gameState)
{
    if(!single_piece_move_test(attacking_piece,attacking_piece_color,source_square,target_square,gameState))
    {
        return false;
    }
    std::pair<uint64_t,uint64_t> before_and_after_bitboards;
    if(!get_bitboard_before_and_after_ply(attacking_piece,attacking_piece_color,source_square,target_square,gameState,before_and_after_bitboards))
    {
        return false;
    }
    return (before_and_after_bitboards.second) == (SINGLE_SQUARE_BITBOARD(target_square)) && (before_and_after_bitboards.first == (SINGLE_SQUARE_BITBOARD(source_square))) ;   
}
TEST_CASE("Board Init","[unit]")
{
    GameState gameState = GameState();
    auto board = gameState.getBoard();
    SECTION( "board initializtion" ) {
        CHECK( board->getPieceBitBoard(PAWN,WHITE) == WHITE_PAWN_START );
        CHECK( board->getPieceBitBoard(PAWN,BLACK) == BLACK_PAWN_START );
        CHECK( board->getPieceBitBoard(KNIGHT,WHITE) == WHITE_KNIGHT_START );
        CHECK( board->getPieceBitBoard(KNIGHT,BLACK) == BLACK_KNIGHT_START );
        CHECK( board->getPieceBitBoard(BISHOP,WHITE) == WHITE_BISHOP_START );
        CHECK( board->getPieceBitBoard(BISHOP,BLACK) == BLACK_BISHOP_START );
        CHECK( board->getPieceBitBoard(ROOK,WHITE) == WHITE_ROOK_START );
        CHECK( board->getPieceBitBoard(ROOK,BLACK) == BLACK_ROOK_START );
        CHECK( board->getPieceBitBoard(QUEEN,WHITE) == WHITE_QUEEN_START );
        CHECK( board->getPieceBitBoard(QUEEN,BLACK) == BLACK_QUEEN_START );
        CHECK( board->getPieceBitBoard(KING,WHITE) == WHITE_KING_START );
        CHECK( board->getPieceBitBoard(KING,BLACK) == BLACK_KING_START );
    }
}
TEST_CASE( "Non Sliding Pieces Movement", "[unit]" ) {
    SECTION( "pawn movement" ) {
        SECTION( "white pawn movement and capture" )
        {
            for(int square = 0 ; square < BOARD_DIM*BOARD_DIM - BOARD_DIM; square ++)
            {
                Color playerToMove = WHITE;
                uint64_t singlePawnBitboard = SINGLE_SQUARE_BITBOARD(square);
                uint64_t flankPawnsForCaptureBitboard;
                int8_t leftFlankPawnSquare;
                int8_t rightFlankPawnSquare;
                u_int8_t onePawnMoveForwardSquare = square + BOARD_DIM;
                u_int8_t twoPawnMoveForwardSquare = square + 2*BOARD_DIM;
                bool currentPawnValidSquaredToMove[BOARD_DIM*BOARD_DIM];

                if(square % BOARD_DIM == 0)
                {
                    rightFlankPawnSquare = square + BOARD_DIM + 1;
                    leftFlankPawnSquare = -1;
                    flankPawnsForCaptureBitboard = (SINGLE_SQUARE_BITBOARD(rightFlankPawnSquare));
                }
                else if(square % BOARD_DIM == BOARD_DIM - 1)
                {
                    leftFlankPawnSquare = square + BOARD_DIM -  1;
                    rightFlankPawnSquare = -1;
                    flankPawnsForCaptureBitboard = (SINGLE_SQUARE_BITBOARD(leftFlankPawnSquare));
                }
                else
                {
                    leftFlankPawnSquare = square + BOARD_DIM + 1;
                    rightFlankPawnSquare = square +BOARD_DIM +- 1;
                    flankPawnsForCaptureBitboard = (SINGLE_SQUARE_BITBOARD(leftFlankPawnSquare)) + (SINGLE_SQUARE_BITBOARD(rightFlankPawnSquare));
                }
                
                std::shared_ptr board = std::make_shared<Board>(singlePawnBitboard ,0,0,0,0,0,flankPawnsForCaptureBitboard,0,0,0,0,0);
                INFO("Checking white pawn movement for board square" << square  );
                if(Board::isSquareWithinBoard(onePawnMoveForwardSquare))
                {
                    currentPawnValidSquaredToMove[onePawnMoveForwardSquare] = true;
                    GameState gameState = GameState(board,0,playerToMove,0);
                    CHECK(single_piece_move_test(PAWN,WHITE, square, onePawnMoveForwardSquare,gameState));
                }
                if(Board::isSquareOnRank(square,BoardRank::RANK_2))
                {
                    currentPawnValidSquaredToMove[twoPawnMoveForwardSquare] = true;
                    GameState gameState  = GameState(board,0,WHITE,0);
                    CHECK(single_piece_move_test(PAWN,WHITE, square, twoPawnMoveForwardSquare,gameState));
                }
                if(Board::isSquareWithinBoard(leftFlankPawnSquare))
                {
                    GameState gameState  = GameState(board,0,WHITE,0);
                    currentPawnValidSquaredToMove[leftFlankPawnSquare] = true;
                    CHECK(single_piece_move_test_capture(PAWN,WHITE,square,leftFlankPawnSquare,gameState));
                }
                if(Board::isSquareWithinBoard(rightFlankPawnSquare))
                {
                    GameState gameState  = GameState(board,0,WHITE,0);
                    currentPawnValidSquaredToMove[rightFlankPawnSquare] = true;
                    CHECK(single_piece_move_test_capture(PAWN,WHITE,square,rightFlankPawnSquare,gameState));
                }
                for(int i = 0 ; i < BOARD_DIM*BOARD_DIM ; i++)
                {
                    if(!currentPawnValidSquaredToMove[i])
                    {
                        GameState gameState  = GameState(board,0,WHITE,0);
                        CHECK(!single_piece_move_test(PAWN,WHITE,square,i,gameState));
                    }
                }
            }
            
        }
        SECTION( "black pawn movement and capture" )
        {
            for(int square = BOARD_DIM*BOARD_DIM - 1 ; square >= BOARD_DIM  ; square --)
            {
                Color playerToMove = BLACK;
                uint64_t singlePawnBitboard = SINGLE_SQUARE_BITBOARD(square);
                uint64_t flankPawnsForCaptureBitboard;
                int8_t leftFlankPawnSquare;
                int8_t rightFlankPawnSquare;
                u_int8_t onePawnMoveForwardSquare = square - BOARD_DIM;
                u_int8_t twoPawnMoveForwardSquare = square - 2*BOARD_DIM;
                bool currentPawnValidSquaredToMove[BOARD_DIM*BOARD_DIM];

                if(square % BOARD_DIM == 0)
                {
                    rightFlankPawnSquare = square - BOARD_DIM + 1;
                    leftFlankPawnSquare = -1;
                    flankPawnsForCaptureBitboard = (SINGLE_SQUARE_BITBOARD(rightFlankPawnSquare));
                }
                else if(square % BOARD_DIM == BOARD_DIM - 1)
                {
                    leftFlankPawnSquare = square - BOARD_DIM -  1;
                    rightFlankPawnSquare = -1;
                    flankPawnsForCaptureBitboard = (SINGLE_SQUARE_BITBOARD(leftFlankPawnSquare));
                }
                else
                {
                    leftFlankPawnSquare = square - BOARD_DIM - 1;
                    rightFlankPawnSquare = square - BOARD_DIM + 1;
                    flankPawnsForCaptureBitboard = (SINGLE_SQUARE_BITBOARD(leftFlankPawnSquare)) + (SINGLE_SQUARE_BITBOARD(rightFlankPawnSquare));
                }
                
                std::shared_ptr board = std::make_shared<Board>(flankPawnsForCaptureBitboard,0,0,0,0,0,singlePawnBitboard,0,0,0,0,0);
                INFO("Checking black pawn movement for board square" << square  );
                if(Board::isSquareWithinBoard(onePawnMoveForwardSquare))
                {
                    currentPawnValidSquaredToMove[onePawnMoveForwardSquare] = true;
                    GameState gameState = GameState(board,0,playerToMove,0);
                    CHECK(single_piece_move_test(PAWN,playerToMove, square, onePawnMoveForwardSquare,gameState));
                }
                if(Board::isSquareOnRank(square,BoardRank::RANK_7))
                {
                    currentPawnValidSquaredToMove[twoPawnMoveForwardSquare] = true;
                    GameState gameState  = GameState(board,0,playerToMove,0);
                    CHECK(single_piece_move_test(PAWN,playerToMove, square, twoPawnMoveForwardSquare,gameState));
                }
                if(Board::isSquareWithinBoard(leftFlankPawnSquare))
                {
                    GameState gameState  = GameState(board,0,playerToMove,0);
                    currentPawnValidSquaredToMove[leftFlankPawnSquare] = true;
                    CHECK(single_piece_move_test_capture(PAWN,playerToMove,square,leftFlankPawnSquare,gameState));
                }
                if(Board::isSquareWithinBoard(rightFlankPawnSquare))
                {
                    GameState gameState  = GameState(board,0,playerToMove,0);
                    currentPawnValidSquaredToMove[rightFlankPawnSquare] = true;
                    CHECK(single_piece_move_test_capture(PAWN,playerToMove,square,rightFlankPawnSquare,gameState));
                }
                for(int i = 0 ; i < BOARD_DIM*BOARD_DIM ; i++)
                {
                    if(!currentPawnValidSquaredToMove[i])
                    {
                        GameState gameState  = GameState(board,0,playerToMove,0);
                        CHECK(!single_piece_move_test(PAWN,playerToMove,square,i,gameState));
                    }
                }
            }
        }
    }
    SECTION( "knight movement INCOMPLETE" ) {
        SECTION( "white knight movement and capture  INCOMPLETE" )
        {
            
            for(auto square : {A2,B1,D1,E2,E4,D5,B5,A4})
            {
                GameState gameState = GameState(std::make_shared<Board>( 0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                CHECK(single_piece_move_test(KNIGHT,WHITE, C3,square,gameState));
                gameState  = GameState(std::make_shared<Board>( 0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,0,SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0),0,WHITE,0); 
                CHECK(single_piece_move_test_capture(KNIGHT,WHITE, C3,square,gameState));
            }
        }
        SECTION( "black knight movement and capture INCOMPLETE" )
        {
            for(auto square : {A2,B1,D1,E2,E4,D5,B5,A4})
            {
                GameState gameState = GameState(std::make_shared<Board>( 0,0,0,0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,0),0,BLACK,0); 
                CHECK(single_piece_move_test(KNIGHT,BLACK, C3,square,gameState));
                gameState  = GameState(std::make_shared<Board>( SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,0),0,BLACK,0); 
                CHECK(single_piece_move_test_capture(KNIGHT,BLACK, C3,square,gameState));
            }
        }
    }
    SECTION( "king movement trivial" ) {
        SECTION( "white king movement and capture" )
        {
            for(auto square : {C3,D3,E3,C5,E5,C4,D5,E4})
            {
                GameState gameState = GameState(std::make_shared<Board>( 0,0,0,0,0,1ull << D4,0,0,0,0,0,0),0,WHITE,0); 
                CHECK(single_piece_move_test(KING,WHITE, D4,square,gameState));
                gameState  = GameState(std::make_shared<Board>( 0,0,0,0,0,1ull << D4,SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0),0,WHITE,0); 
                CHECK(single_piece_move_test_capture(KING,WHITE, D4,square,gameState));
            }
        }
        SECTION( "black king movement and capture" )
        {
            for(auto square : {C3,D3,E3,C5,E5,C4,D5,E4})
            {
                GameState gameState = GameState(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,0,0,1ull << D4),0,BLACK,0); 
                CHECK(single_piece_move_test(KING,BLACK, D4,square,gameState));
                gameState  = GameState(std::make_shared<Board>( SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0,0,0,0,0,0,1ull << D4),0,BLACK,0); 
                CHECK(single_piece_move_test_capture(KING,BLACK, D4,square,gameState));
            }
        }
    }
}
TEST_CASE( "Sliding Pieces Movement", "[unit]" ) {
    SECTION( "sliding" ) {
        SECTION( "rook movement" ) {
            SECTION( "white rook movement and capture" )
            {

                for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8})
                {
                    GameState gameState = GameState(std::make_shared<Board>( 0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,0,0,0,0,0),0,WHITE,0); 
                    INFO("ROOK MC for direction " << square  );
                    CHECK(single_piece_move_test(ROOK,WHITE, C3,square,gameState));
                    
                    gameState  = GameState(std::make_shared<Board>( 0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0),0,WHITE,0); 
                    INFO("ROOK MC CAPTURE for direction " << square );
                    CHECK(single_piece_move_test_capture(ROOK,WHITE, C3,square,gameState));
                }
                uint64_t square = (1ull<<D4);
                INFO("Exact white rook capture open board" <<  square);
                GameState gameState  = GameState(std::make_shared<Board>( 0,0,0,square,0,0,0,0,0,0,0,0),0,WHITE,0); 
                auto moves = gameState.getMoveBitboardSquareCollection(ROOK);
                CHECK(moves->size() == 1);
                CHECK(moves->at(0).first == 0x8080808f7080808);
                
            }
            SECTION( "black rook movement and capture" )
            {
                GameState gameState;
                for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8})
                {
                    gameState = GameState(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0),0,BLACK,0); 
                    CHECK(single_piece_move_test(ROOK,BLACK, C3,square,gameState));
                    gameState  = GameState(std::make_shared<Board>( SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0),0,BLACK,0); 
                    CHECK(single_piece_move_test_capture(ROOK,BLACK, C3,square,gameState));
                }
            }                
        }
        SECTION( "bishop movement" ) {
            SECTION( "white bishop movement and capture" )
            {
                for(auto square : {A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                {
                    GameState gameState = GameState(std::make_shared<Board>( 0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                    INFO("BISHOP MC for direction " << square  );
                    CHECK(single_piece_move_test(BISHOP,WHITE, C3,square,gameState));
                    gameState  = GameState(std::make_shared<Board>( 0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0),0,WHITE,0); 
                    INFO("BISHOP MC CAPTURE for direction " << square );
                    CHECK(single_piece_move_test_capture(BISHOP,WHITE,C3,square,gameState));
                    
                }
                GameState gameState = GameState(std::make_shared<Board>( 0,0,SINGLE_SQUARE_BITBOARD(D4),0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                auto moves = gameState.getMoveBitboardSquareCollection(BISHOP);
                CHECK(moves->size() == 1);
                CHECK(moves->at(0).first == 0x8041221400142241);
            }
            SECTION( "black bishop movement and capture" )
            {
                for(auto square : {A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                {
                    GameState gameState = GameState(std::make_shared<Board>( 0,0,0,0,0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0),0,BLACK,0); 
                    CHECK(single_piece_move_test(BISHOP,BLACK, C3,square,gameState));
                    gameState  = GameState(std::make_shared<Board>( SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0),0,BLACK,0); 
                    CHECK(single_piece_move_test_capture(BISHOP,BLACK,C3,square,gameState));
                }
            }                
        }
        
        SECTION( "queen movement" ) {
            SECTION( "white queen movement and capture" )
            {
                for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8,A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                {
                    GameState gameState = GameState(std::make_shared<Board>( 0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,0,0,0,0),0,WHITE,0); 
                    CHECK(single_piece_move_test(QUEEN,WHITE, C3,square,gameState));
                    gameState  = GameState(std::make_shared<Board>( 0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0),0,WHITE,0); 
                    CHECK(single_piece_move_test_capture(QUEEN,WHITE,C3,square,gameState));
                }
            }
            SECTION( "black queen movement and capture" )
            {
                std::shared_ptr<GameState>gameState;
                for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8,A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                {
                    GameState gameState = GameState(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0),0,BLACK,0); 
                    CHECK(single_piece_move_test(QUEEN,BLACK, C3,square,gameState));
                    gameState  = GameState(std::make_shared<Board>( SINGLE_SQUARE_BITBOARD(square),0,0,0,0,0,0,0,0,0,SINGLE_SQUARE_BITBOARD(C3),0),0,BLACK,0); 
                    CHECK(single_piece_move_test_capture(QUEEN,BLACK,C3,square,gameState));
                }
            }                
        }
    }
}
TEST_CASE( "Blocks", "[unit]" ) {
    SECTION( "Blocks" )
    {
        SECTION( "Blocks ROOK INC" )
        {

            for(auto square : {C5, C1, F3,A3})
            {
                uint64_t blockingInc = SINGLE_SQUARE_BITBOARD(square);
                uint64_t blockingExclude = 0;
                GameState gameState = GameState(std::make_shared<Board>
                ( 0,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,blockingInc,0,0,0,0,0),0,WHITE,0); 

                auto moves = gameState.generateMove(ROOK);
                bool passed = false;
                for(auto move : *moves)
                {
                    if(move.targetSquare == square)
                    {
                        passed = true;
                        break;
                    }
                }
                INFO("ROOK INC for direction " << square << " passed " << passed );
                CHECK(passed);
            }
        }
        SECTION( "Blocks ROOK EXCLUDE" )
        {


            for(auto square : {C5, C1, F3,A3})
            {
                uint64_t blockingInc = 0;
                uint64_t blockingExclude = SINGLE_SQUARE_BITBOARD(square);
                GameState gameState = GameState(std::make_shared<Board>
                ( blockingExclude,0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,0,0,0,0,0),0,WHITE,0); 

                auto moves = gameState.generateMove(ROOK);
                bool passed = true;
                for(auto move : *moves)
                {
                    if(move.targetSquare == square)
                    {
                        passed = false;
                        break;
                    }
                }
                INFO("ROOK INC for direction " << square << " passed " << passed);
                CHECK(passed);
            }
        }
        /*SECTION( "Blocks BISHOP INC" )
        {
            std::shared_ptr<GameState>gameState;

            for(auto square : {A5, B2, E1,G7})
            {
                uint64_t blockingInc = SINGLE_SQUARE_BITBOARD(square);
                uint64_t blockingExclude = 0;
                gameState = GameState(std::make_shared<Board>( 0,0,SINGLE_SQUARE_BITBOARD(C3),0,0,0,blockingInc,0,0,0,0,0),0,WHITE,0); 

                auto moves = gameState.generateMove(BISHOP);
                bool passed = false;
                for(auto move : *moves)
                {
                    if(move.targetSquare == square)
                    {
                        passed = true;
                        break;
                    }
                }
                CHECK(passed);
            }
        }*/
        
    }
}
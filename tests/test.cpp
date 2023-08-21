
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include "../src/Board.hpp"
#include "../src/GameState.hpp"
#include <memory>
#include <iostream>
#define SINGLE_PIECE_MOVE_TEST_UNWRAPPED(piece,color,source_square,target_square,gameState)\
    auto piecebbs = gameState->getBoard()->getPieceBitBoard(piece, color); \
    auto state =  gameState->playPly(source_square,target_square); \
    if(state==nullptr) {CHECK(false);}\
    auto piecebbe = state->getBoard()->getPieceBitBoard(piece, color); \
    CHECK( piecebbs == 1ull << source_square); \
    CHECK( piecebbe == 1ull << target_square ); 
#define SINGLE_PIECE_MOVE_TEST(piece,color,source_square,target_square,gameState) \
{\
    SINGLE_PIECE_MOVE_TEST_UNWRAPPED(piece,color,source_square,target_square,gameState)}
#define SINGLE_PIECE_MOVE_TEST_CAPTURE(attacking_piece,attacking_piece_color,captured_piece,captured_piece_color,source_square,target_square,gameState)\
    {\
    auto captured_piece_bbs = gameState->getBoard()->getPieceBitBoard(captured_piece, captured_piece_color); \
    SINGLE_PIECE_MOVE_TEST_UNWRAPPED(attacking_piece, attacking_piece_color, source_square, target_square, gameState);\
    auto captured_piece_bbe = state->getBoard()->getPieceBitBoard(captured_piece, captured_piece_color); \
    CHECK( captured_piece_bbs == 1ull << target_square ); \
    CHECK( captured_piece_bbe == 0 );}
TEST_CASE( "generalized several ply game", "[unit]" ) {

    std::unique_ptr<GameState> gameState = std::make_unique<GameState>();
    auto board = gameState->getBoard();
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
    SECTION( "piece movement" ) {
        SECTION( "non sliding" ) {
            SECTION( "pawn movement" ) {
                SECTION( "white pawn movement and capture" )
                {
                    auto gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << D2 ,0,0,0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                    SINGLE_PIECE_MOVE_TEST(PAWN,WHITE, D2, D4,gameState);
                    gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << D2 ,0,0,0,0,0,0,0,0,0,0,0),0,WHITE,0);
                    SINGLE_PIECE_MOVE_TEST(PAWN,WHITE, D2, D3,gameState);
                    gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << D5 ,0,0,0,0,0,0,0,0,0,0,0),0,WHITE,0);
                    SINGLE_PIECE_MOVE_TEST(PAWN,WHITE, D5, D6,gameState);
                    gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << D5 ,0,0,0,0,0,1ull << 42,0,0,0,0,0),0,WHITE,0);
                    SINGLE_PIECE_MOVE_TEST_CAPTURE(PAWN,WHITE,PAWN,BLACK, D5,C6,gameState);
                    gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << D5 ,0,0,0,0,0,1ull << 44,0,0,0,0,0),0,WHITE,0); 
                    SINGLE_PIECE_MOVE_TEST_CAPTURE(PAWN,WHITE,PAWN,BLACK, D5,E6,gameState);
                }
                SECTION( "black pawn movement and capture" )
                {
                    auto gameState  = std::make_unique<GameState>(std::make_shared<Board>(0,0,0,0,0,0,1ull << D4,0,0,0,0,0),0,BLACK,0); 
                    SINGLE_PIECE_MOVE_TEST(PAWN,BLACK, D4, D3,gameState);
                    gameState  = std::make_unique<GameState>(std::make_shared<Board>(0 ,0,0,0,0,0,1ull << D7,0,0,0,0,0),0,BLACK,0);
                    SINGLE_PIECE_MOVE_TEST(PAWN,BLACK, D7, D5,gameState);
                    gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << C4 ,0,0,0,0,0,1ull << D5,0,0,0,0,0),0,BLACK,0);
                    SINGLE_PIECE_MOVE_TEST_CAPTURE(PAWN,BLACK,PAWN,WHITE, D5,C4,gameState);
                    gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << E4 ,0,0,0,0,0,1ull << D5,0,0,0,0,0),0,BLACK,0); 
                    SINGLE_PIECE_MOVE_TEST_CAPTURE(PAWN,BLACK,PAWN,WHITE, D5,E4,gameState);
                }
            }
            SECTION( "knight movement" ) {
                SECTION( "white knight movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {A2,B1,D1,E2,E4,D5,B5,A4})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,1ull << C3,0,0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST(KNIGHT,WHITE, C3,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 0,1ull << C3,0,0,0,0,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(KNIGHT,WHITE,PAWN,BLACK, C3,square,gameState);
                    }
                }
                SECTION( "black knight movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {A2,B1,D1,E2,E4,D5,B5,A4})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,1ull << C3,0,0,0,0),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST(KNIGHT,BLACK, C3,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,1ull << C3,0,0,0,0),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(KNIGHT,BLACK,PAWN,WHITE, C3,square,gameState);
                    }
                }
            }
            SECTION( "king movement" ) {
                SECTION( "white king movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {C3,D3,E3,C5,E5,C4,D5,E4})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,0,1ull << D4,0,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST(KING,WHITE, D4,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,0,1ull << D4,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(KING,WHITE,PAWN,BLACK, D4,square,gameState);
                    }
                }
                SECTION( "black king movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {C3,D3,E3,C5,E5,C4,D5,E4})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,0,0,1ull << D4),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST(KING,BLACK, D4,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,0,0,0,0,1ull << D4),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(KING,BLACK,PAWN,WHITE, D4,square,gameState);
                    }
                }
            }
        }
        SECTION( "sliding" ) {
             SECTION( "rook movement" ) {
                SECTION( "white rook movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,1ull << C3,0,0,0,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST(ROOK,WHITE, C3,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,1ull << C3,0,0,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(ROOK,WHITE,PAWN,BLACK, C3,square,gameState);
                    }
                }
                SECTION( "black rook movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,1ull << C3,0,0),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST(ROOK,BLACK, C3,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,0,0,1ull << C3,0,0),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(ROOK,BLACK,PAWN,WHITE, C3,square,gameState);
                    }
                }                
            }
            SECTION( "bishop movement" ) {
                SECTION( "white bishop movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,1ull << C3,0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST(BISHOP,WHITE, C3,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 0,0,1ull << C3,0,0,0,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(BISHOP,WHITE,PAWN,BLACK, C3,square,gameState);
                    }
                }
                SECTION( "black bishop movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,0,1ull << C3,0,0,0),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST(BISHOP,BLACK, C3,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,0,1ull << C3,0,0,0),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(BISHOP,BLACK,PAWN,WHITE, C3,square,gameState);
                    }
                }                
            }
            SECTION( "queen movement" ) {
                SECTION( "white queen movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8,A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,1ull << C3,0,0,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST(QUEEN,WHITE, C3,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,1ull << C3,0,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(QUEEN,WHITE,PAWN,BLACK, C3,square,gameState);
                    }
                }
                SECTION( "black queen movement and capture" )
                {
                    std::unique_ptr<GameState>gameState;
                    for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8,A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                    {
                        gameState = std::make_unique<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,0,1ull << C3,0),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST(QUEEN,BLACK, C3,square,gameState);
                        gameState  = std::make_unique<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,0,0,0,1ull << C3,0),0,BLACK,0); 
                        SINGLE_PIECE_MOVE_TEST_CAPTURE(QUEEN,BLACK,PAWN,WHITE, C3,square,gameState);
                    }
                }                
            }
        }

        /*
        auto board_single_white_bishop = std::make_shared<Board>(0,0,1ull << 35,0,0,0,0,0,0,0,0,0);
        auto board_single_black_bishop = std::make_shared<Board>(0,0,0,0,0,0,0,0,1ull << 35,0,0,0);
        auto board_single_white_rook = std::make_shared<Board>(0,0,0,1ull << 35,0,0,0,0,0,0,0,0);
        auto board_single_black_rook = std::make_shared<Board>(0,0,0,0,0,0,0,0,0,1ull << 35,0,0);
        auto board_single_white_queen = std::make_shared<Board>(0,0,0,0,1ull << 35,0,0,0,0,0,0,0);
        auto board_single_black_queen = std::make_shared<Board>(0,0,0,0,0,0,0,0,0,0,1ull << 35,0);
       */
        



        
        
        // auto pawnbb1 = gameState->getBoard()->getPieceBitBoard(PAWN, WHITE);
        // auto state =  gameState->playPly(A2,A4);
        // auto pawnbb2 = state->getBoard()->getPieceBitBoard(PAWN, WHITE);
        // auto board = state->getBoard();
        // CHECK( pawnbb2 == 0x100FE00 );

    }
    
}
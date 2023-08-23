#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include "../src/Board.hpp"
#include "../src/GameState.hpp"
#include <memory>
#include <iostream>
bool single_piece_move_test(PieceType piece,Color color,uint8_t source_square,uint8_t target_square,std::shared_ptr<GameState> gameState)
{
    auto piecebbs = gameState->getBoard()->getPieceBitBoard(piece, color); 
    if(piecebbs == 0) {return false;}
    auto state =  gameState->playPly(source_square,target_square); 
    if(state==nullptr) {return false;}
    auto piecebbe = state->getBoard()->getPieceBitBoard(piece, color); 
    return  (piecebbs == 1ull << source_square) &&(piecebbe == 1ull << target_square );
} 

bool single_piece_move_test_capture(PieceType attacking_piece,Color attacking_piece_color,PieceType captured_piece,Color captured_piece_color,uint8_t source_square,uint8_t target_square,std::shared_ptr<GameState> gameState)
{
    if(!single_piece_move_test(attacking_piece,attacking_piece_color,source_square,target_square,gameState)) {return false;}

    auto captured_piece_bbs = gameState->getBoard()->getPieceBitBoard(captured_piece, captured_piece_color); 
    if(captured_piece_bbs == 0) {return false;}
    auto state =  gameState->playPly(source_square,target_square);
    auto captured_piece_bbe = state->getBoard()->getPieceBitBoard(captured_piece, captured_piece_color); 
    return (captured_piece_bbs == 1ull << target_square ) && (captured_piece_bbe == 0);
    
}
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
                    auto gameState  = std::make_shared<GameState>(std::make_shared<Board>(1ull << D2 ,0,0,0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                    CHECK(single_piece_move_test(PAWN,WHITE, D2, D4,gameState));
                    gameState  = std::make_shared<GameState>(std::make_shared<Board>(1ull << D2 ,0,0,0,0,0,0,0,0,0,0,0),0,WHITE,0);
                    CHECK(single_piece_move_test(PAWN,WHITE, D2, D3,gameState));
                    gameState  = std::make_shared<GameState>(std::make_shared<Board>(1ull << D5 ,0,0,0,0,0,0,0,0,0,0,0),0,WHITE,0);
                    CHECK(single_piece_move_test(PAWN,WHITE, D5, D6,gameState));
                    gameState  = std::make_shared<GameState>(std::make_shared<Board>(1ull << D5 ,0,0,0,0,0,1ull << 42,0,0,0,0,0),0,WHITE,0);
                    CHECK(single_piece_move_test_capture(PAWN,WHITE,PAWN,BLACK, D5,C6,gameState));
                    gameState  = std::make_shared<GameState>(std::make_shared<Board>(1ull << D5 ,0,0,0,0,0,1ull << 44,0,0,0,0,0),0,WHITE,0); 
                    CHECK(single_piece_move_test_capture(PAWN,WHITE,PAWN,BLACK, D5,E6,gameState));
                }
                SECTION( "black pawn movement and capture" )
                {
                    auto gameState  = std::make_shared<GameState>(std::make_shared<Board>(0,0,0,0,0,0,1ull << D4,0,0,0,0,0),0,BLACK,0); 
                    CHECK(single_piece_move_test(PAWN,BLACK, D4, D3,gameState));
                    gameState  = std::make_shared<GameState>(std::make_shared<Board>(0 ,0,0,0,0,0,1ull << D7,0,0,0,0,0),0,BLACK,0);
                    CHECK(single_piece_move_test(PAWN,BLACK, D7, D5,gameState));
                    gameState  = std::make_shared<GameState>(std::make_shared<Board>(1ull << C4 ,0,0,0,0,0,1ull << D5,0,0,0,0,0),0,BLACK,0);
                    CHECK(single_piece_move_test_capture(PAWN,BLACK,PAWN,WHITE, D5,C4,gameState));
                    gameState  = std::make_shared<GameState>(std::make_shared<Board>(1ull << E4 ,0,0,0,0,0,1ull << D5,0,0,0,0,0),0,BLACK,0); 
                    CHECK(single_piece_move_test_capture(PAWN,BLACK,PAWN,WHITE, D5,E4,gameState));
                }
            }
            SECTION( "knight movement" ) {
                SECTION( "white knight movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {A2,B1,D1,E2,E4,D5,B5,A4})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,1ull << C3,0,0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test(KNIGHT,WHITE, C3,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 0,1ull << C3,0,0,0,0,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test_capture(KNIGHT,WHITE,PAWN,BLACK, C3,square,gameState));
                    }
                }
                SECTION( "black knight movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {A2,B1,D1,E2,E4,D5,B5,A4})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,1ull << C3,0,0,0,0),0,BLACK,0); 
                        CHECK(single_piece_move_test(KNIGHT,BLACK, C3,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,1ull << C3,0,0,0,0),0,BLACK,0); 
                        CHECK(single_piece_move_test_capture(KNIGHT,BLACK,PAWN,WHITE, C3,square,gameState));
                    }
                }
            }
            SECTION( "king movement" ) {
                SECTION( "white king movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {C3,D3,E3,C5,E5,C4,D5,E4})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,0,1ull << D4,0,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test(KING,WHITE, D4,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,0,1ull << D4,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test_capture(KING,WHITE,PAWN,BLACK, D4,square,gameState));
                    }
                }
                SECTION( "black king movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {C3,D3,E3,C5,E5,C4,D5,E4})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,0,0,1ull << D4),0,BLACK,0); 
                        CHECK(single_piece_move_test(KING,BLACK, D4,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,0,0,0,0,1ull << D4),0,BLACK,0); 
                        CHECK(single_piece_move_test_capture(KING,BLACK,PAWN,WHITE, D4,square,gameState));
                    }
                }
            }
        }
        SECTION( "sliding" ) {
             SECTION( "rook movement" ) {
                SECTION( "white rook movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,1ull << C3,0,0,0,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test(ROOK,WHITE, C3,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,1ull << C3,0,0,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test_capture(ROOK,WHITE,PAWN,BLACK, C3,square,gameState));
                    }
                }
                SECTION( "black rook movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,1ull << C3,0,0),0,BLACK,0); 
                        CHECK(single_piece_move_test(ROOK,BLACK, C3,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,0,0,1ull << C3,0,0),0,BLACK,0); 
                        CHECK(single_piece_move_test_capture(ROOK,BLACK,PAWN,WHITE, C3,square,gameState));
                    }
                }                
            }
            SECTION( "bishop movement" ) {
                SECTION( "white bishop movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,1ull << C3,0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test(BISHOP,WHITE, C3,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 0,0,1ull << C3,0,0,0,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test_capture(BISHOP,WHITE,PAWN,BLACK, C3,square,gameState));
                    }
                }
                SECTION( "black bishop movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,0,1ull << C3,0,0,0),0,BLACK,0); 
                        CHECK(single_piece_move_test(BISHOP,BLACK, C3,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,0,1ull << C3,0,0,0),0,BLACK,0); 
                        CHECK(single_piece_move_test_capture(BISHOP,BLACK,PAWN,WHITE, C3,square,gameState));
                    }
                }                
            }
            SECTION( "queen movement" ) {
                SECTION( "white queen movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8,A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,1ull << C3,0,0,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test(QUEEN,WHITE, C3,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,1ull << C3,0,1ull << square,0,0,0,0,0),0,WHITE,0); 
                        CHECK(single_piece_move_test_capture(QUEEN,WHITE,PAWN,BLACK, C3,square,gameState));
                    }
                }
                SECTION( "black queen movement and capture" )
                {
                    std::shared_ptr<GameState>gameState;
                    for(auto square : {B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8,A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1})
                    {
                        gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,0,0,0,0,0,0,0,0,1ull << C3,0),0,BLACK,0); 
                        CHECK(single_piece_move_test(QUEEN,BLACK, C3,square,gameState));
                        gameState  = std::make_shared<GameState>(std::make_shared<Board>( 1ull << square,0,0,0,0,0,0,0,0,0,1ull << C3,0),0,BLACK,0); 
                        CHECK(single_piece_move_test_capture(QUEEN,BLACK,PAWN,WHITE, C3,square,gameState));
                    }
                }                
            }
        }
        SECTION( "RAY CHECKING" )
        {
            SECTION( "Blocks ROOK INC" )
            {
                std::shared_ptr<GameState>gameState;

                for(auto square : {C5, C1, F3,A3})
                {
                    uint64_t blockingInc = 1ull << square;
                    uint64_t blockingExclude = 0;
                    gameState = std::make_shared<GameState>(std::make_shared<Board>
                    ( 0,0,0,1ull << C3,0,0,blockingInc,0,0,0,0,0),0,WHITE,0); 

                    auto moves = gameState->generateMove(ROOK);
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
            }
            SECTION( "Blocks ROOK EXCLUDE" )
            {
                std::shared_ptr<GameState>gameState;

                for(auto square : {C5, C1, F3,A3})
                {
                    uint64_t blockingInc = 0;
                    uint64_t blockingExclude = 1ull << square;
                    gameState = std::make_shared<GameState>(std::make_shared<Board>
                    ( blockingExclude,0,0,1ull << C3,0,0,0,0,0,0,0,0),0,WHITE,0); 

                    auto moves = gameState->generateMove(ROOK);
                    bool passed = true;
                    for(auto move : *moves)
                    {
                        if(move.targetSquare == square)
                        {
                            passed = false;
                            break;
                        }
                    }
                    CHECK(passed);
                }
            }
            SECTION( "Blocks BISHOP INC" )
            {
                std::shared_ptr<GameState>gameState;

                for(auto square : {A5, B2, E1,G7})
                {
                    uint64_t blockingInc = 1ull << square;
                    uint64_t blockingExclude = 0;
                    gameState = std::make_shared<GameState>(std::make_shared<Board>( 0,0,1ull << C3,0,0,0,blockingInc,0,0,0,0,0),0,WHITE,0); 

                    auto moves = gameState->generateMove(BISHOP);
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
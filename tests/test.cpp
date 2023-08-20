
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include "../src/Board.hpp"
#include "../src/GameState.hpp"
#include <memory>
#include <iostream>

#define SINGLE_PIECE_MOVE_TEST(piece,color,source_square,target_square,gameState) \
{\
    auto piecebbs = gameState->getBoard()->getPieceBitBoard(piece, color); \
    auto state =  gameState->playPly(source_square,target_square); \
    if(state==nullptr) {REQUIRE(false);}\
    auto piecebbe = state->getBoard()->getPieceBitBoard(piece, color); \
    REQUIRE( piecebbs == 1ull << source_square ); \
    REQUIRE( piecebbe == 1ull << target_square );}
#define SINGLE_PIECE_MOVE_TEST_CAPTURE(attacking_piece,attacking_piece_color,captured_piece,captured_piece_color,source_square,target_square,gameState)\
    {\
    auto captured_piece_bbs = gameState->getBoard()->getPieceBitBoard(captured_piece, captured_piece_color); \
    SINGLE_PIECE_MOVE_TEST(attacking_piece, attacking_piece_color, source_square, target_square, gameState);\
    auto captured_piece_bbe = gameState->getBoard()->getPieceBitBoard(captured_piece, captured_piece_color); \
    REQUIRE( captured_piece_bbs == 1ull << target_square ); \
    REQUIRE( captured_piece_bbe == 0 );}
TEST_CASE( "generalized several ply game", "[unit]" ) {

    std::unique_ptr<GameState> gameState = std::make_unique<GameState>();
    auto board = gameState->getBoard();
    SECTION( "board initializtion" ) {
        REQUIRE( board->getPieceBitBoard(PAWN,WHITE) == WHITE_PAWN_START );
        REQUIRE( board->getPieceBitBoard(PAWN,BLACK) == BLACK_PAWN_START );
        REQUIRE( board->getPieceBitBoard(KNIGHT,WHITE) == WHITE_KNIGHT_START );
        REQUIRE( board->getPieceBitBoard(KNIGHT,BLACK) == BLACK_KNIGHT_START );
        REQUIRE( board->getPieceBitBoard(BISHOP,WHITE) == WHITE_BISHOP_START );
        REQUIRE( board->getPieceBitBoard(BISHOP,BLACK) == BLACK_BISHOP_START );
        REQUIRE( board->getPieceBitBoard(ROOK,WHITE) == WHITE_ROOK_START );
        REQUIRE( board->getPieceBitBoard(ROOK,BLACK) == BLACK_ROOK_START );
        REQUIRE( board->getPieceBitBoard(QUEEN,WHITE) == WHITE_QUEEN_START );
        REQUIRE( board->getPieceBitBoard(QUEEN,BLACK) == BLACK_QUEEN_START );
        REQUIRE( board->getPieceBitBoard(KING,WHITE) == WHITE_KING_START );
        REQUIRE( board->getPieceBitBoard(KING,BLACK) == BLACK_KING_START );
    }
    SECTION( "piece movement" ) {
        SECTION( "pawn movement" ) {
            SECTION( "white pawn movement" )
            {
                auto gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << 35 ,0,0,0,0,0,0,0,0,0,0,0),0,WHITE,0); 
                SINGLE_PIECE_MOVE_TEST(PAWN,WHITE, D5, D7,gameState);
                gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << 35 ,0,0,0,0,0,0,0,0,0,0,0),0,WHITE,0);
                SINGLE_PIECE_MOVE_TEST(PAWN,WHITE, D5, D6,gameState);
                gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << 35 ,0,0,0,0,0,1ull << 42,0,0,0,0,0),0,WHITE,0);
                {
                  auto captured_piece_bbs =
                      gameState->getBoard()->getPieceBitBoard(PAWN, BLACK);
                  {
                    auto piecebbs =
                        gameState->getBoard()->getPieceBitBoard(PAWN, WHITE);
                    auto state = gameState->playPly(D5, C6);
                    if (state == nullptr) {
                      do {
                        (void)__builtin_constant_p(false);
                        Catch ::AssertionHandler catchAssertionHandler(
                            "REQUIRE"_catch_sr,
                            ::Catch ::SourceLineInfo(
                                "/home/arels/ChessEngine/tests/test.cpp",
                                static_cast<std ::size_t>(51)),
                            "false", Catch ::ResultDisposition ::Normal);
                        try {
                          catchAssertionHandler.handleExpr(
                              Catch ::Decomposer() <= false);
                        } catch (...) {
                          catchAssertionHandler
                              .handleUnexpectedInflightException();
                        }
                        catchAssertionHandler.complete();
                      } while ((void)0,
                               (false) && static_cast<const bool &>(!!(false)));
                    }
                    auto piecebbe =
                        state->getBoard()->getPieceBitBoard(PAWN, WHITE);
                    do {
                      (void)__builtin_constant_p(piecebbs == 1ull << D5);
                      Catch ::AssertionHandler catchAssertionHandler(
                          "REQUIRE"_catch_sr,
                          ::Catch ::SourceLineInfo(
                              "/home/arels/ChessEngine/tests/test.cpp",
                              static_cast<std ::size_t>(51)),
                          "piecebbs == 1ull << D5",
                          Catch ::ResultDisposition ::Normal);
                      try {
                        catchAssertionHandler.handleExpr(
                            Catch ::Decomposer() <= piecebbs == 1ull << D5);
                      } catch (...) {
                        catchAssertionHandler
                            .handleUnexpectedInflightException();
                      }
                      catchAssertionHandler.complete();
                    } while ((void)0, (false) && static_cast<const bool &>(!!(
                                                     piecebbs == 1ull << D5)));
                    do {
                      (void)__builtin_constant_p(piecebbe == 1ull << C6);
                      Catch ::AssertionHandler catchAssertionHandler(
                          "REQUIRE"_catch_sr,
                          ::Catch ::SourceLineInfo(
                              "/home/arels/ChessEngine/tests/test.cpp",
                              static_cast<std ::size_t>(51)),
                          "piecebbe == 1ull << C6",
                          Catch ::ResultDisposition ::Normal);
                      try {
                        catchAssertionHandler.handleExpr(
                            Catch ::Decomposer() <= piecebbe == 1ull << C6);
                      } catch (...) {
                        catchAssertionHandler
                            .handleUnexpectedInflightException();
                      }
                      catchAssertionHandler.complete();
                    } while ((void)0, (false) && static_cast<const bool &>(!!(
                                                     piecebbe == 1ull << C6)));
                  };
                  auto captured_piece_bbe =
                      gameState->getBoard()->getPieceBitBoard(PAWN, BLACK);
                  do {
                    (void)__builtin_constant_p(captured_piece_bbs == 1ull
                                                                         << C6);
                    Catch ::AssertionHandler catchAssertionHandler(
                        "REQUIRE"_catch_sr,
                        ::Catch ::SourceLineInfo(
                            "/home/arels/ChessEngine/tests/test.cpp",
                            static_cast<std ::size_t>(51)),
                        "captured_piece_bbs == 1ull << C6",
                        Catch ::ResultDisposition ::Normal);
                    try {
                      catchAssertionHandler.handleExpr(Catch ::Decomposer() <=
                                                           captured_piece_bbs ==
                                                       1ull << C6);
                    } catch (...) {
                      catchAssertionHandler.handleUnexpectedInflightException();
                    }
                    catchAssertionHandler.complete();
                  } while ((void)0,
                           (false) && static_cast<const bool &>(!!(
                                          captured_piece_bbs == 1ull << C6)));
                  do {
                    (void)__builtin_constant_p(captured_piece_bbe == 0);
                    Catch ::AssertionHandler catchAssertionHandler(
                        "REQUIRE"_catch_sr,
                        ::Catch ::SourceLineInfo(
                            "/home/arels/ChessEngine/tests/test.cpp",
                            static_cast<std ::size_t>(51)),
                        "captured_piece_bbe == 0",
                        Catch ::ResultDisposition ::Normal);
                    try {
                      catchAssertionHandler.handleExpr(
                          Catch ::Decomposer() <= captured_piece_bbe == 0);
                    } catch (...) {
                      catchAssertionHandler.handleUnexpectedInflightException();
                    }
                    catchAssertionHandler.complete();
                  } while ((void)0, (false) && static_cast<const bool &>(!!(
                                                   captured_piece_bbe == 0)));
                };
                gameState  = std::make_unique<GameState>(std::make_shared<Board>(1ull << 35 ,0,0,0,0,0,1ull << 44,0,0,0,0,0),0,WHITE,0); 
                SINGLE_PIECE_MOVE_TEST_CAPTURE(PAWN,WHITE,PAWN,BLACK, D5,E6,gameState);
            }
            
        }
        /*auto board_single_black_pawn = std::make_shared<Board>(0,0,0,0,0,0,1ull << 35,0,0,0,0,0);
        auto board_single_white_knight = std::make_shared<Board>(0,1ull << 35,0,0,0,0,0,0,0,0,0,0);
        auto board_single_black_knight = std::make_shared<Board>(0,0,0,0,0,0,0,1ull << 35,0,0,0,0);
        auto board_single_white_bishop = std::make_shared<Board>(0,0,1ull << 35,0,0,0,0,0,0,0,0,0);
        auto board_single_black_bishop = std::make_shared<Board>(0,0,0,0,0,0,0,0,1ull << 35,0,0,0);
        auto board_single_white_rook = std::make_shared<Board>(0,0,0,1ull << 35,0,0,0,0,0,0,0,0);
        auto board_single_black_rook = std::make_shared<Board>(0,0,0,0,0,0,0,0,0,1ull << 35,0,0);
        auto board_single_white_queen = std::make_shared<Board>(0,0,0,0,1ull << 35,0,0,0,0,0,0,0);
        auto board_single_black_queen = std::make_shared<Board>(0,0,0,0,0,0,0,0,0,0,1ull << 35,0);
        auto board_single_white_king = std::make_shared<Board>(0,0,0,0,0,1ull << 35,0,0,0,0,0,0);
        auto board_single_black_king = std::make_shared<Board>(0,0,0,0,0,0,0,0,0,0,0,1ull << 35);*/
        



        
        
        // auto pawnbb1 = gameState->getBoard()->getPieceBitBoard(PAWN, WHITE);
        // auto state =  gameState->playPly(A2,A4);
        // auto pawnbb2 = state->getBoard()->getPieceBitBoard(PAWN, WHITE);
        // auto board = state->getBoard();
        // REQUIRE( pawnbb2 == 0x100FE00 );

    }
    
}
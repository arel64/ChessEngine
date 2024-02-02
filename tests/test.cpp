#include <catch2/catch_test_macros.hpp>
#include "PseudoLegalMoveGenerator.hpp"
#include <cstdint>
#include <memory>
#include <iostream>

bool checkTargetSquareContainedContainedInMoves(std::shared_ptr<std::vector<std::shared_ptr<Ply>>> moves, uint8_t targetSquare)
{
    for(auto move : *moves)
    {
        bool isSameSquare = move->targetSquare == targetSquare;
        CHECK(isSameSquare);
        if(isSameSquare)
        {
            return true;
        }
    }
    return false;
}
bool checkTargetSquaresContainedInMoves(std::shared_ptr<std::vector<std::shared_ptr<Ply>>> moves,std::vector<u_int8_t> squares)
{
    for(auto square : squares)
    {
        if(!checkTargetSquareContainedContainedInMoves(moves,square))
        {
            return false;
        }
    }
    return true;
}
bool checkTargetSquaresContainedInMovesExactly(std::shared_ptr<std::vector<std::shared_ptr<Ply>>> moves,std::vector<u_int8_t> squares)
{
    bool isSameSize = moves->size() == squares.size();
    CHECK(isSameSize);
    if(isSameSize)
    {
        return false;
    }
    return checkTargetSquaresContainedInMoves(moves,squares);
}
std::shared_ptr<std::vector<std::shared_ptr<Ply>>> generateSinglePieceFamilyBoardAndMoves(std::shared_ptr<MoveGenerator> generator,PieceFamilyBitboard pieceFamily)
{
    std::shared_ptr<std::vector<PieceFamilyBitboard>> pieceFamilyBitboards = std::make_shared<std::vector<PieceFamilyBitboard>>();
    pieceFamilyBitboards->push_back(pieceFamily);
    std::shared_ptr<Board> singlePawnBitboard = std::make_shared<Board>(pieceFamilyBitboards);
    GameState gameState = GameState(singlePawnBitboard,0,pieceFamily.getPieceFamily()->getColor(),0);
    return generator->generateMoves(gameState);
}

bool checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly
    (std::shared_ptr<MoveGenerator> generator,PieceFamilyBitboard pieceFamilyBitboard,std::vector<u_int8_t> squares)
{
    auto potentialPlys =  generateSinglePieceFamilyBoardAndMoves(generator,pieceFamilyBitboard);
    return checkTargetSquaresContainedInMovesExactly(potentialPlys,squares);
}

TEST_CASE("Board Init","[unit]")
{
    GameState gameState = GameState();
    auto board = gameState.getBoard();
    SECTION( "board initializtion" ) {
        PieceFamily whitePawn = PieceFamily(PAWN,WHITE);
        PieceFamily blackPawn = PieceFamily(PAWN,BLACK);
        PieceFamily whiteKnight = PieceFamily(KNIGHT,WHITE);
        PieceFamily blackKnight = PieceFamily(KNIGHT,BLACK);
        PieceFamily whiteBishop = PieceFamily(BISHOP,WHITE);
        PieceFamily blackBishop = PieceFamily(BISHOP,BLACK);
        PieceFamily whiteRook = PieceFamily(ROOK,WHITE);
        PieceFamily blackRook = PieceFamily(ROOK,BLACK);
        PieceFamily whiteQueen = PieceFamily(QUEEN,WHITE);
        PieceFamily blackQueen = PieceFamily(QUEEN,BLACK);
        PieceFamily whiteKing = PieceFamily(KING,WHITE);
        PieceFamily blackKing = PieceFamily(KING,BLACK);

        CHECK( board->getPieceFamilyBitBoard(whitePawn ) == WHITE_PAWN_START) ;
        CHECK( board->getPieceFamilyBitBoard(blackPawn ) == BLACK_PAWN_START) ;
        CHECK( board->getPieceFamilyBitBoard( whiteKnight) == WHITE_KNIGHT_START) ;
        CHECK( board->getPieceFamilyBitBoard( blackKnight) == BLACK_KNIGHT_START) ;
        CHECK( board->getPieceFamilyBitBoard(whiteBishop ) == WHITE_BISHOP_START) ;
        CHECK( board->getPieceFamilyBitBoard( blackBishop) == BLACK_BISHOP_START) ;
        CHECK( board->getPieceFamilyBitBoard(whiteRook ) == WHITE_ROOK_START) ;
        CHECK( board->getPieceFamilyBitBoard( blackRook) == BLACK_ROOK_START) ;
        CHECK( board->getPieceFamilyBitBoard(whiteQueen ) == WHITE_QUEEN_START) ;
        CHECK( board->getPieceFamilyBitBoard(blackQueen ) == BLACK_QUEEN_START) ;
        CHECK( board->getPieceFamilyBitBoard(whiteKing ) == WHITE_KING_START) ;
        CHECK( board->getPieceFamilyBitBoard(blackKing) == BLACK_KING_START) ;
    }
}
TEST_CASE( "Non Sliding Pieces Movement", "[unit]" ) {
    std::shared_ptr<MoveGenerator> moveGenerator = std::make_shared<PseudoLegalMoveGenerator>();
    SECTION( "Pawn Movement" ) {
        SECTION("White")
        {
            PieceFamily pieceFamily =  PieceFamily(PAWN,WHITE);
            PieceFamily pieceFamilyCapture =  PieceFamily(PAWN,BLACK);
            SECTION( "Single Central Row Pawn" )
            {
                INFO("Checking pawn movement for central row square");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{C4}
                );
                INFO("Checking  pawn movement for central row flank files");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(A3)),{A4}
                );
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(H3)),{H4}
                );
            }
            SECTION( "Single  Second Rank Pawn")
            {
                INFO("Checking  pawn movement for second row square");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C2)),{C3,C4}
                );
                INFO("Checking  pawn movement for second row flank files");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(A2)),{A3,A4}
                );
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(H2)),{H3,H4}
                );
                INFO("Checking  pawn movement for seventh row flank files");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(H7)),{H8}
                );
            }
            SECTION( "Pawn Captures")
            {
                INFO("Checking  pawn movement for central row square");
                auto pieceFamilyBitboardVec = std::make_shared<std::vector<PieceFamilyBitboard>>();
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)));
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamilyCapture,SINGLE_SQUARE_BITBOARD(D4) + SINGLE_SQUARE_BITBOARD(B4)));
                GameState gameState = GameState(std::make_shared<Board>(pieceFamilyBitboardVec),0,WHITE,0);
                auto moves = moveGenerator->generateMoves(gameState);
                checkTargetSquaresContainedInMovesExactly(moves,{D4,C4,B4});

                INFO("Checking pawn capture for flank row square");
                pieceFamilyBitboardVec = std::make_shared<std::vector<PieceFamilyBitboard>>();
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(A2)));
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamilyCapture,SINGLE_SQUARE_BITBOARD(B3)));
                gameState = GameState(std::make_shared<Board>(pieceFamilyBitboardVec),0,WHITE,0);
                moves = moveGenerator->generateMoves(gameState);
                checkTargetSquaresContainedInMovesExactly(moves,{B3,A3,A4});

                pieceFamilyBitboardVec = std::make_shared<std::vector<PieceFamilyBitboard>>();
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(H2)));
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamilyCapture,SINGLE_SQUARE_BITBOARD(G3)));
                gameState = GameState(std::make_shared<Board>(pieceFamilyBitboardVec),0,WHITE,0);
                moves = moveGenerator->generateMoves(gameState);
                checkTargetSquaresContainedInMovesExactly(moves,{H3,H4,G3});

            }

        }
        SECTION("Black")
        {
            PieceFamily pieceFamily =  PieceFamily(PAWN,BLACK);
            PieceFamily pieceFamilyCapture =  PieceFamily(PAWN,WHITE);

            SECTION( "Single Central Row  Pawn" )
            {
                INFO("Checking  pawn movement for central row square");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{C2}
                );
                INFO("Checking  pawn movement for central row flank files");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(A3)),{A2}
                );
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(H3)),{H2}
                );
            }
            SECTION( "Single  Seventh Rank Pawn")
            {
                INFO("Checking  pawn movement for Seventh row flank files");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(A7)),{A6,A5}
                );
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(H7)),{H5,H6}
                );
                INFO("Checking  pawn movement for second row square");
                checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                    moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C2)),{C1}
                );
            }
            SECTION( "Pawn Captures")
            {
                INFO("Checking  pawn movement for central row square");
                auto pieceFamilyBitboardVec = std::make_shared<std::vector<PieceFamilyBitboard>>();
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)));
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamilyCapture,SINGLE_SQUARE_BITBOARD(B2) + SINGLE_SQUARE_BITBOARD(D2)));
                GameState gameState = GameState(std::make_shared<Board>(pieceFamilyBitboardVec),0,BLACK,0);
                auto moves = moveGenerator->generateMoves(gameState);
                checkTargetSquaresContainedInMovesExactly(moves,{D2,C2,B2});

                INFO("Checking pawn capture for flank row square");
                pieceFamilyBitboardVec = std::make_shared<std::vector<PieceFamilyBitboard>>();
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(A2)));
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamilyCapture,SINGLE_SQUARE_BITBOARD(B1)));
                gameState = GameState(std::make_shared<Board>(pieceFamilyBitboardVec),0,BLACK,0);
                moves = moveGenerator->generateMoves(gameState);
                checkTargetSquaresContainedInMovesExactly(moves,{B1,A1});

                pieceFamilyBitboardVec = std::make_shared<std::vector<PieceFamilyBitboard>>();
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(H2)));
                pieceFamilyBitboardVec->push_back(PieceFamilyBitboard(pieceFamilyCapture,SINGLE_SQUARE_BITBOARD(G1)));
                gameState = GameState(std::make_shared<Board>(pieceFamilyBitboardVec),0,BLACK,0);
                moves = moveGenerator->generateMoves(gameState);
                checkTargetSquaresContainedInMovesExactly(moves,{H1,G1});

            }
        }
    }
}
    /*
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
                auto moves = gameState.getMoveBitboardSquareCollection(gameState,ROOK);
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
                auto moves = gameState.getMoveBitboardSquareCollection(gameState,BISHOP);
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

                auto moves = gameState.generateMove(gameState,ROOK);
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

                auto moves = gameState.generateMove(gameState,ROOK);
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
        
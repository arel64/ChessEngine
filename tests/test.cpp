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
TEST_CASE( "Pawn Movement" ) {
    std::shared_ptr<MoveGenerator> moveGenerator = std::make_shared<PseudoLegalMoveGenerator>();
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
TEST_CASE( "Knight movement" )
{
    std::shared_ptr<MoveGenerator> moveGenerator = std::make_shared<PseudoLegalMoveGenerator>();
    SECTION( "white knight movement and capture  INCOMPLETE" )
    {
        PieceFamily pieceFamily =  PieceFamily(KNIGHT,WHITE);
        INFO("Checking knight movement for central knight");
        checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
            moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{A2,B1,D1,E2,E4,D5,B5,A4}
        );
    }
    SECTION( "black knight movement and capture INCOMPLETE" )
    {
        PieceFamily pieceFamily =  PieceFamily(KNIGHT,BLACK);
        INFO("Checking knight movement for central knight");
        checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
            moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{A2,B1,D1,E2,E4,D5,B5,A4}
        );
    }
}
TEST_CASE( "Sliding Pieces Movement", "[unit]" ) {
    std::shared_ptr<MoveGenerator> moveGenerator = std::make_shared<PseudoLegalMoveGenerator>();
    
    SECTION( "Rook movement" ) {
        SECTION( "white rook movement" )
        {
            PieceFamily pieceFamily =  PieceFamily(ROOK,WHITE);
            
            INFO("Checking rook movement for central placement square empty board");
            checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8}
            );
            
        }
        SECTION( "black rook movement" )
        {
            PieceFamily pieceFamily =  PieceFamily(ROOK,BLACK);
            
            INFO("Checking rook movement for central placement square empty board");
            checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8}
            );
        }                
    }

    SECTION( "Bishop movement" ) {
        SECTION( "white bishop movement and capture" )
        {
            PieceFamily pieceFamily =  PieceFamily(BISHOP,WHITE);
            INFO("Checking bishop movement for central placement square empty board");
            checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1}
            );
        }
        SECTION( "black bishop movement and capture" )
        {
            PieceFamily pieceFamily =  PieceFamily(BISHOP,BLACK);
            INFO("Checking bishop movement for central placement square empty board");
            checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1}
            );
        }                
    }
    SECTION( "Queen movement" ) {
        SECTION( "white queen movement" )
        {
            PieceFamily pieceFamily =  PieceFamily(QUEEN,WHITE);
            INFO("Checking bishop movement for central placement square empty board");
            checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8,A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1}
            );
        }
        SECTION( "black queen movement and capture" )
        {
            PieceFamily pieceFamily =  PieceFamily(QUEEN,BLACK);
            INFO("Checking bishop movement for central placement square empty board");
            checkPieceFamilySingleSquareBitboardTargetSquaresCollectionExactly(
                moveGenerator,PieceFamilyBitboard(pieceFamily,SINGLE_SQUARE_BITBOARD(C3)),{B3,A3,D3,E3,F3,G3,H3,C1,C2,C4,C5,C6,C7,C8,A1,B2,D4,E5,F6,G7,H8,B4,A5,D2,E1}
            );
        }                
    }
}
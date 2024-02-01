#ifndef MACRO_PSEUDOLEGALMOVEGENERATOR_HPP
#define MACRO_PSEUDOLEGALMOVEGENERATOR_HPP
#include "MoveGenerator.hpp"
class PseudoLegalMoveGenerator : MoveGenerator {
    public:
        std::shared_ptr<std::vector<move>> generateMoves (std::shared_ptr<GameState> gameState);
    private:
        std::shared_ptr<std::vector<move>> PseudoLegalMoveGenerator::generateMove(GameState& gameState,PieceType pieceType);
        std::shared_ptr<std::vector<std::pair<uint64_t,uint8_t>>>PseudoLegalMoveGenerator::getMoveBitboardSquareCollection(GameState& gameState,PieceType p);
        uint64_t bishopMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square); 
        uint64_t rookMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square);
        uint64_t slidingPieceMovesByPotentialDirections(Direction PotentialDirections[4], uint8_t square, uint64_t blockingInc, uint64_t blockingExclude);
        uint64_t pawnMove(GameState& gameState,uint64_t &piece, uint64_t &pieceClipFileA, uint64_t &pieceClipFileH);
        uint64_t knightMove(GameState& gameState,uint64_t &piece_moves, uint64_t pieceClipFileAB, uint64_t pieceClipFileA, uint64_t pieceClipFileH, uint64_t pieceClipFileGH);
        uint64_t kingMove(GameState& gameState,uint64_t pieceClipFileA, uint64_t piece, uint64_t pieceClipFileH);
        uint64_t getMoveBitboardAndUpdateDirection(uint64_t blockingInc, uint64_t blockingExclude, uint64_t squareBitboard, bool& directions);
        uint8_t getSquareInDirection(uint8_t square, Direction direction, uint8_t distance);

};
#endif
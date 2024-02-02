#ifndef MACRO_PSEUDOLEGALMOVEGENERATOR_HPP
#define MACRO_PSEUDOLEGALMOVEGENERATOR_HPP
#include "MoveGenerator.hpp"
#include "DirectionIterator.hpp"
class PseudoLegalMoveGenerator : public MoveGenerator {
    public:
        std::shared_ptr<std::vector<std::shared_ptr<Ply>>> generateMoves(GameState& gameState) override;
    private:
        std::shared_ptr<std::vector<std::shared_ptr<Ply>>> generateMove(GameState& gameState,PieceType pieceType);
        std::shared_ptr<std::vector<std::pair<uint64_t,uint8_t>>>getMoveBitboardSquareCollection(GameState& gameState,PieceType p);
        uint64_t bishopMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square); 
        uint64_t rookMove(uint64_t blockingInc,uint64_t blockingExclude,uint8_t square);
        uint64_t pawnMove(GameState& gameState,uint64_t &piece, uint64_t &pieceClipFileA, uint64_t &pieceClipFileH);
        uint64_t knightMove(GameState& gameState,uint64_t &piece_moves, uint64_t pieceClipFileAB, uint64_t pieceClipFileA, uint64_t pieceClipFileH, uint64_t pieceClipFileGH);
        uint64_t kingMove(GameState& gameState,uint64_t pieceClipFileA, uint64_t piece, uint64_t pieceClipFileH);
        uint64_t getMoveBitboardAndUpdateDirection(uint64_t blockingInc, uint64_t blockingExclude, uint64_t squareBitboard, bool& directions);
        uint64_t slidingPieceMovesByPotentialDirections(std::vector<std::shared_ptr<DirectionIteratorBase>> directionIterators, uint64_t blockingInclusive, uint64_t blockingExclusive);
        std::shared_ptr<std::vector<std::shared_ptr<Ply>>> generatePseudoLegalMoves (GameState& gameState);
        bool isEnemyKingThreatened(GameState& gameState);
        void removeIllegalMoves(GameState& gameState,std::shared_ptr<std::vector<std::shared_ptr<Ply>>> pseduolegalMoves);

};
#endif
#ifndef MACRO_ENGINE_HPP
#define MACRO_ENGINE_HPP
#include "GameState.hpp"
#include <atomic>
#include <cstdint>
#include <memory>
#include "MoveGenerator.hpp"
#define ENGINE_NAME "FAF Engine"
#define ENGINE_AUTHOR "Arel Sharon"
struct GoParams
{
    uint32_t wtime;
    uint32_t btime;
    uint32_t winc;
    uint32_t binc;
    uint32_t movestogo;
    uint32_t depth;
    uint32_t nodes;
    uint32_t mate;
    uint32_t movetime;
    bool infinite;
    bool ponder;

    std::vector<std::string> moves;
};
struct CalculationState
{
    std::shared_ptr<Ply> bestPly;
    std::atomic<bool> isCalculating;
    pid_t pid;
    int communicationfd;

    void reset()
    {
        bestPly = nullptr;
        isCalculating = false;
        pid = 0;
        communicationfd = 0;
    }
};
class Engine{
    public:
        Engine(std::shared_ptr<MoveGenerator> moveGenerator);
        void loadPositon(const std::string& fen,std::string moves);
        void startCalculation(std::shared_ptr<GoParams>);
        void setPlyAsBest(std::shared_ptr<Ply> ply);
        void reset();
        
        std::string GetName(){return ENGINE_NAME;};
        std::string GetAuthor(){return ENGINE_AUTHOR;};
        
    private:
        CalculationState calculationState;
        std::shared_ptr<GameState> game;
        std::shared_ptr<MoveGenerator> moveGenerator;
    private:
        std::string getSquareName(uint8_t square);
        uint8_t getSquareFromName(std::string squareName);
};
#endif
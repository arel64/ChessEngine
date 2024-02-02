#ifndef MACRO_ENGINE_HPP
#define MACRO_ENGINE_HPP
#include "GameState.hpp"
#include "Helper.hpp"
#include <atomic>
#include <cstdint>
#include <memory>

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

class Engine{
    public:
        void loadPositon(const std::string& fen,std::string moves);
        void startCalculation(std::shared_ptr<GoParams>);
        void interruptCalculation() { this->stopCalculation = true; }
        void setMoveAsBest(const Ply& move);
        Engine() : isCalculating(false), stopCalculation(false), game(nullptr) {}
        std::string bestMove;
    private:
        std::atomic<bool> isCalculating;
        std::atomic<bool> stopCalculation;
        std::shared_ptr<GameState> game;
    private:
        std::string getSquareName(uint8_t square);
        uint8_t getSquareFromName(std::string squareName);
};
#endif
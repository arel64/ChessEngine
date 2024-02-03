#ifndef MACRO_UCIHandeling_HPP
#define MACRO_UCIHandeling_HPP
#include "Engine.hpp"
#include <memory>
#include <vector>
#include <mutex>
#include <queue>
#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>
#include <unordered_map>
#include <iostream>
#include <atomic>
#include <iterator>
#include <sstream>


class UCIHandeling
{
    public:
        int startUciHandler();
        UCIHandeling(std::shared_ptr<Engine>);
    public:
        bool isRunning;
        bool isDebugMode;
        const std::string START_POSITION = "startpos";
        const std::string FEN_POSITION = "fen";
        std::shared_ptr<Engine> engine;
    private:
        void startCommandHandlerLoop();
        int getCommandResponse(std::shared_ptr<std::vector<std::string>> arglist);


};
#endif
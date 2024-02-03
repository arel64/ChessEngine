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

#define ENGINE_NAME "FAF Engine"
#define ENGINE_AUTHOR "Arel Sharon"
namespace NS_UCICommands{
    class UCICommand;
};
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
        int getCommandResponse(std::vector<std::string>& arglist);


};

namespace NS_UCICommands {
    enum class UCICommandName
    {
        UCI = 0,
        Debug,
        IsReady,
        SetOption,
        //Register,
        UciNewGame,
        Position,
        Go,
        Stop,
        //PonderHit,
        Quit,
        Unknown
    };
    const std::unordered_map<std::string,UCICommandName> UCICommandStrings = {
        {"uci",UCICommandName::UCI},
        {"debug",UCICommandName::Debug},
        {"isready",UCICommandName::IsReady},
        {"setoption",UCICommandName::SetOption},
        //{"register",UCICommand::Register},
        {"ucinewgame",UCICommandName::UciNewGame},
        {"position",UCICommandName::Position},
        {"go",UCICommandName::Go},
        {"stop",UCICommandName::Stop},
        //{"ponderhit",UCICommand::PonderHit},
        {"quit",UCICommandName::Quit},
        {"unknown",UCICommandName::Unknown}
    };
    const std::unordered_map<UCICommandName,uint32_t> UCIInteruptingCommandsPriority = {
        {UCICommandName::Quit,9},
        {UCICommandName::Stop,7},
        {UCICommandName::IsReady,8},
        {UCICommandName::Debug,10},
    };

    class UCICommand
    {
        public:
            void execute();
            const UCICommandName command;
            std::string commandParams;
            UCICommand(UCIHandeling* instance,UCICommandName command,std::string params,uint32_t commandTime) :
             instance(instance),command(command),commandParams(params),commandTime(commandTime){};

            static bool isInteruptingCommand(UCICommandName command);
            uint32_t getCommandTime(){return this->commandTime;}
        private:
            uint32_t commandTime = 0;
            UCIHandeling* instance;
            void handleUCICommand();
            void handleIsReadyCommand();
            void handleDebugCommand();
            void handleStopCommand();
            void handleQuitCommand();
            void handleSetOptionCommand();
            void handleUciNewGameCommand();
            void handlePositionCommand();
            void handleGoCommand();
            void handleUnknownCommand();
            
    };


    
}
#endif
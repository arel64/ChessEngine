#ifndef MACRO_UCIHandeling_HPP
#define MACRO_UCIHandeling_HPP
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
#define ENGINE_NAME ("FAF Engine")
#define ENGINE_AUTHOR ("Arel Sharon")
namespace NS_UCICommands{
    class UCICommand;
};
class UCIHandeling
{
    public:
        int startUciHandler();
        void bufferCommands(std::string& commandStrings);
        bool isRunning;
        bool isDebugMode;
        uint32_t getCommandsCounterAndAdvance(){ return commandsCounter++;}
        UCIHandeling();
    private:
        typedef struct UCICommandCompare
        {
            bool operator()(const std::shared_ptr<NS_UCICommands::UCICommand>& lhs, const std::shared_ptr<NS_UCICommands::UCICommand>& rhs) const;
        }UCICommandCompare;
        std::priority_queue<std::shared_ptr<NS_UCICommands::UCICommand>,std::vector<std::shared_ptr<NS_UCICommands::UCICommand>>,UCICommandCompare> currentlyExecutingCommands;
        std::vector<std::string> commandBuffer;
        std::unique_ptr<std::mutex> currentlyExecutingCommandsMutex;
        uint32_t commandsCounter;
        


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
            std::atomic<bool> isCommandExectuing;
            std::atomic<bool> isCommandStarted;
            std::shared_ptr<std::thread> worker;
            UCICommand(UCIHandeling* instance,UCICommandName command,uint32_t commandTime) :
             instance(instance),command(command),isCommandExectuing(false),isCommandStarted(false),worker(nullptr),commandTime(commandTime){};

            static bool isInteruptingCommand(UCICommandName command);
            uint32_t getCommandTime(){return this->commandTime;}
        private:
            uint32_t commandTime = 0;
            UCIHandeling* instance;
            void handleUCICommand();
            void handleIsReadyCommand();
            void handleSetOptionCommand();
            void handleUciNewGameCommand();
            void handlePositionCommand();
            void handleGoCommand();
            void handleUnknownCommand();
    };


    
}
#endif
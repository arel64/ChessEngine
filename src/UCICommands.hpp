#ifndef MACRO_UCICOMMANDS_HPP
#define MACRO_UCICOMMANDS_HPP

#include <string>
#include <thread>
#include <unordered_map>
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
    class UCICommand
    {
        public:
            const UCICommandName command;
            std::thread execute();
            UCICommand(UCICommandName command) : command(command){};
        private:
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
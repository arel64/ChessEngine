#ifndef MACRO_UCICommandGenerator_HPP
#define MACRO_UCICommandGenerator_HPP
    #include "UCICommand.hpp"
    #include "Engine.hpp"
    
    static const std::unordered_map<std::string,UCICommandType> UCICommandStrings = {
        {"uci",UCICommandType::UCI},
        {"debug",UCICommandType::Debug},
        {"isready",UCICommandType::IsReady},
        {"setoption",UCICommandType::SetOption},
        //{"register",UCICommandType::Register},
        {"ucinewgame",UCICommandType::UciNewGame},
        {"position",UCICommandType::Position},
        {"go",UCICommandType::Go},
        {"stop",UCICommandType::Stop},
        //{"ponderhit",UCICommandType::PonderHit},
        {"quit",UCICommandType::Quit},
        {"unknown",UCICommandType::Unknown}
    };
    class UCICommandGenerator
    {
        public:
            std::unique_ptr<UCICommand> generateUCICommand(
                std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine);
        private:
            UCICommandType parseCommand(std::shared_ptr<std::vector<std::string>> commandwordslist);
    };
#endif
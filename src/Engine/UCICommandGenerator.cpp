#include "UCICommandGenerator.hpp"
#include "Uci.hpp"
#include "IsReady.hpp"
#include "Quit.hpp"
#include "UciNewGame.hpp"
#include "Go.hpp"
UCICommandType UCICommandGenerator::parseCommand(std::shared_ptr<std::vector<std::string>>commandwordslist)
{
    for (auto &word : *commandwordslist)
    {
        if (UCICommandStrings.find(word) != UCICommandStrings.end())
        {
            return UCICommandStrings.at(word);
            break;
        }
    }
    return UCICommandType::Unknown;
}

std::unique_ptr<UCICommand> UCICommandGenerator::generateUCICommand(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine)
{
    UCICommandType commandType = parseCommand(arglist);
    switch (commandType)
    {
        case UCICommandType::UCI:
            return std::make_unique<Uci>(arglist,engine);
            break;
        case UCICommandType::IsReady:
            return std::make_unique<IsReady>(arglist,engine);
            break;
        case UCICommandType::Debug:
            throw std::invalid_argument("Debug command not implemented");
            break;
        case UCICommandType::Stop:
            throw std::invalid_argument("Stop command not implemented");
            break;
        case UCICommandType::Quit:
            return std::make_unique<Quit>(arglist,engine);
            break;
        case UCICommandType::SetOption:
            throw std::invalid_argument("SetOption command not implemented");
            break;
        case UCICommandType::UciNewGame:
            return std::make_unique<UciNewGame>(arglist,engine);
            break;
        case UCICommandType::Position:
            throw std::invalid_argument("Position command not implemented");
            break;
        case UCICommandType::Go:
            return std::make_unique<Go>(arglist,engine);
            break;
        case UCICommandType::Unknown:
        case UCICommandType::Ignore:
        default:
            throw std::invalid_argument("Invalid UCI Command");
            break;
    }
}
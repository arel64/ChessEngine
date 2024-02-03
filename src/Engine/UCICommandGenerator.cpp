#include "UCICommandGenerator.hpp"
#include "Uci.hpp"
#include "IsReady.hpp"
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
            
        case UCICommandType::Stop:
            
        case UCICommandType::Quit:
            
        case UCICommandType::SetOption:
            
        case UCICommandType::UciNewGame:
            
        case UCICommandType::Position:
            
        case UCICommandType::Go:
            throw std::invalid_argument("Go command not implemented");
        case UCICommandType::Unknown:
        case UCICommandType::Ignore:
        default:
            throw std::invalid_argument("Invalid UCI Command");
            break;
    }
}
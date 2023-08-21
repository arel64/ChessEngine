#include "UCICommands.hpp"
std::thread NS_UCICommands::UCICommand::execute()
{
    switch (command)
    {
        case UCICommandName::UCI:
            return std::thread(&NS_UCICommands::UCICommand::handleUCICommand,this);
            break;
        case UCICommandName::IsReady:
            //return std::thread(&NS_UCICommands::UCICommand::handleIsReadyCommand,this);
            break;
        case UCICommandName::Debug:
           // return std::thread(handleIsReadyCommand);
            break;
        case UCICommandName::Stop:
            //return std::thread(handleIsReadyCommand);
        break;
        case UCICommandName::Quit:
            //return std::thread(handleIsReadyCommand);
        break;
        case UCICommandName::SetOption:
            //return std::thread(handleSetOptionCommand);
            break;
        case UCICommandName::UciNewGame:
            //return std::thread(handleUciNewGameCommand);
            break;

        case UCICommandName::Position:
            //return std::thread(handlePositionCommand);
            break;
        case UCICommandName::Go:
            //return std::thread(handleGoCommand);
            break;
        case UCICommandName::Unknown:
        default:
            return std::thread();
            break;
    }
    return std::thread();
}
void NS_UCICommands::UCICommand::handleUCICommand()
{
    std::cout << "id name MySimpleEngine" << std::endl;
    std::cout << "id author YourName" << std::endl;
    // Other engine info commands here

    std::cout << "uciok" << std::endl;
}
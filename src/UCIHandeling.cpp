#include "UCIHandeling.hpp"
#include <iterator>

UCIHandeling::UCIHandeling()
{
    this->isRunning = true;
    this->isDebugMode = true;
    this->commandsCounter = 0;
    this->currentlyExecutingCommands = std::priority_queue<std::shared_ptr<NS_UCICommands::UCICommand>,std::vector<std::shared_ptr<NS_UCICommands::UCICommand>>,UCICommandCompare>();
    this->currentlyExecutingCommandsMutex = std::make_unique<std::mutex>();
    this->commandBuffer = std::vector<std::string>();
    this->startUciHandler();
}
bool UCIHandeling::UCICommandCompare::UCICommandCompare::operator()(const std::shared_ptr<NS_UCICommands::UCICommand>& lhs,
                                                                        const std::shared_ptr<NS_UCICommands::UCICommand>& rhs) const
{
    
    bool interputing_lhs = NS_UCICommands::UCICommand::isInteruptingCommand(lhs->command);
    bool interputing_rhs = NS_UCICommands::UCICommand::isInteruptingCommand(rhs->command);
    if(interputing_lhs && interputing_rhs)
    {
        return NS_UCICommands::UCIInteruptingCommandsPriority.at(lhs->command) < NS_UCICommands::UCIInteruptingCommandsPriority.at(rhs->command);
    }
    else if(interputing_lhs)
    {
        return false;
    }
    else if(interputing_rhs)
    {
        return true;
    }
    else
    {
        return lhs->getCommandTime() < rhs->getCommandTime();
    }
}
int UCIHandeling::startUciHandler()
{
    std::string commandsString;
    while (this->isRunning) {
        getline(std::cin, commandsString);
        if(!commandsString.empty())
        {
            this->commandBuffer.push_back(commandsString);
            continue;
        }
        /*
            Parse commands from buffer
        */
        for(std::vector<std::string>::iterator currentCommand = this->commandBuffer.begin(); currentCommand  != this->commandBuffer.end();)
        {
            auto seperator = currentCommand->find(" ");
            std::string commandNameStr;
            std::string commandParams;
            if(seperator == std::string::npos)
            {
                commandNameStr = currentCommand->substr(0);
                commandParams = "";
            }
            else
            {
                commandNameStr = currentCommand->substr(0,seperator );
                commandParams = currentCommand->substr(seperator );
            }

            currentCommand = this->commandBuffer.erase(currentCommand);
            NS_UCICommands::UCICommandName commandName = NS_UCICommands::UCICommandStrings.at(commandNameStr);
            if(commandName == NS_UCICommands::UCICommandName::Unknown)
            {
                continue;
            }            
            std::shared_ptr<NS_UCICommands::UCICommand> recievedCommand =
                std::make_shared<NS_UCICommands::UCICommand>(this,commandName,UCIHandeling::getCommandsCounterAndAdvance());

            this->currentlyExecutingCommandsMutex->lock();
            this->currentlyExecutingCommands.push(recievedCommand);
            this->currentlyExecutingCommandsMutex->unlock();
        }
        /*
            Remove Finished commands from queue and start highest priority commands
        */
        this->currentlyExecutingCommandsMutex->lock();
        if(this->currentlyExecutingCommands.empty())
        {
            this->currentlyExecutingCommandsMutex->unlock();
            continue;
        }
        auto command = this->currentlyExecutingCommands.top();
        if(command.get()->isCommandStarted)
        {
            if(!command.get()->isCommandExectuing)
            {
                command.get()->worker->join();
                this->currentlyExecutingCommands.pop();
            }
        }
        else
        {
            command.get()->isCommandStarted = true;
            command.get()->execute();
        }

        this->currentlyExecutingCommandsMutex->unlock();
    }
    return 0;
}

void NS_UCICommands::UCICommand::execute()
{
    this->isCommandExectuing = true;
    switch (command)
    {
        case UCICommandName::UCI:
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handleUCICommand,this);
            break;
        case UCICommandName::IsReady:
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handleIsReadyCommand,this);
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
            this->worker = nullptr;
            break;
    }
    
}
void NS_UCICommands::UCICommand::handleUCICommand()
{
    std::cout << "id name " << ENGINE_NAME <<std::endl;
    std::cout << "id author " << ENGINE_AUTHOR << std::endl;
    // Other engine info commands here

    std::cout << "uciok" << std::endl;
    this->isCommandExectuing = false;
}
void NS_UCICommands::UCICommand::handleIsReadyCommand()
{
    std::cout << "readyok" << ENGINE_NAME <<std::endl;
    this->isCommandExectuing = false;
}
bool NS_UCICommands::UCICommand::isInteruptingCommand(NS_UCICommands::UCICommandName command)
{
    return NS_UCICommands::UCIInteruptingCommandsPriority.find(command) != NS_UCICommands::UCIInteruptingCommandsPriority.end();
}

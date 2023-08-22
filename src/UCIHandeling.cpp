#include "UCIHandeling.hpp"

UCIHandeling::UCIHandeling(std::shared_ptr<Engine> engine)
{
    this->isRunning = true;
    this->isDebugMode = true;
    this->commandsCounter = 0;
    this->currentlyExecutingCommands = std::priority_queue<std::shared_ptr<NS_UCICommands::UCICommand>,std::vector<std::shared_ptr<NS_UCICommands::UCICommand>>,UCICommandCompare>();
    this->currentlyExecutingCommandsMutex = std::make_unique<std::mutex>();
    this->commandBuffer = std::vector<std::string>();
    this->outputBuffer = std::vector<std::string>();
    this->outputBufferMutex = std::make_unique<std::mutex>();
    this->engine = engine;
    this->outputBufferCounter = 0;
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
    std::thread printBufferThread(&UCIHandeling::printBuffer,this);
    std::atomic<bool> isInputGiven( false);

    std::mutex commandStringMutex;
    std::string commandsString = "";
    std::string temp = "";
    std::thread inputWaiter([&]{
        while(this->isRunning)
        {
            getline(std::cin, temp);
            if(!temp.empty())
            {
                commandStringMutex.lock();
                commandsString = std::string(temp);
                temp = "";
                isInputGiven = true;
                commandStringMutex.unlock();
            }
            while (isInputGiven) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        
    });
    while (this->isRunning) {
        commandStringMutex.lock();
        if(!commandsString.empty())
        {
            this->commandBuffer.push_back(commandsString);
            commandsString = "";
            isInputGiven = false;
            commandStringMutex.unlock();
            continue;
        }
        commandStringMutex.unlock();

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
                commandParams = currentCommand->substr(seperator + 1 );
            }

            currentCommand = this->commandBuffer.erase(currentCommand);
            NS_UCICommands::UCICommandName commandName = NS_UCICommands::UCICommandStrings.at(commandNameStr);
            if(commandName == NS_UCICommands::UCICommandName::Unknown)
            {
                continue;
            }            
            std::shared_ptr<NS_UCICommands::UCICommand> recievedCommand =
                std::make_shared<NS_UCICommands::UCICommand>(this,commandName,commandParams,UCIHandeling::getCommandsCounterAndAdvance());

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
    printBufferThread.join();
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
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handleDebugCommand,this);
            break;
        case UCICommandName::Stop:
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handleStopCommand,this);
            break;
        break;
        case UCICommandName::Quit:
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handleQuitCommand,this);
            break;
        break;
        case UCICommandName::SetOption:
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handleSetOptionCommand,this);
            break;
        case UCICommandName::UciNewGame:
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handleUciNewGameCommand,this);
            break;

        case UCICommandName::Position:
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handlePositionCommand,this);
            break;
        case UCICommandName::Go:
            this->worker =  std::make_shared<std::thread>(&NS_UCICommands::UCICommand::handleGoCommand,this);
            break;
        case UCICommandName::Unknown:
        default:
            this->worker = nullptr;
            break;
    }
    
}
void NS_UCICommands::UCICommand::handleUCICommand()
{
    std::string ret1 = "id name ";
    std::string ret2 = "id author ";
    std::string ret3 = "uciok";
    this->instance->pushToOutputBuffer(ret1.append(ENGINE_NAME));
    this->instance->pushToOutputBuffer(ret2.append(ENGINE_AUTHOR));
    this->instance->pushToOutputBufferAndWait(ret3);

    this->isCommandExectuing = false;
}
void NS_UCICommands::UCICommand::handleIsReadyCommand()
{
    std::string ret = "readyok";
    this->instance->pushToOutputBufferAndWait(ret);
    this->isCommandExectuing = false;
}
void NS_UCICommands::UCICommand::handleDebugCommand()
{
    if(this->commandParams.compare("on") == 0)
    {
        this->instance->isDebugMode = true;
    }
    if(this->commandParams.compare("off")  == 0)
    {
        this->instance->isDebugMode = false;
    }
    this->isCommandExectuing = false;
}
void NS_UCICommands::UCICommand::handleQuitCommand()
{
    this->instance->isRunning = false;
    this->isCommandExectuing = false;

}
bool NS_UCICommands::UCICommand::isInteruptingCommand(NS_UCICommands::UCICommandName command)
{
    return NS_UCICommands::UCIInteruptingCommandsPriority.find(command) != NS_UCICommands::UCIInteruptingCommandsPriority.end();
}
void NS_UCICommands::UCICommand::handlePositionCommand()
{
    std::string position = this->commandParams;
    auto split = position.find(this->instance->START_POSITION);
    if( split != std::string::npos)
    {
        this->instance->engine->loadPositon("",position.substr(split+this->instance->START_POSITION.length() +1));
    }
    split = position.find(this->instance->FEN_POSITION);
    if(split != std::string::npos)
    {
        this->instance->engine->loadPositon(position,position.substr(split+this->instance->FEN_POSITION.length()+1));
    }
    this->isCommandExectuing = false;

}
void NS_UCICommands::UCICommand::handleStopCommand()
{
    this->instance->engine->interruptCalculation();
    this->isCommandExectuing = false;
}
void NS_UCICommands::UCICommand::handleUciNewGameCommand()
{
    //TODO:: This needs to be replaced by a reset command
    this->instance->engine = std::make_shared<Engine>();
    this->isCommandExectuing = false;

}


void NS_UCICommands::UCICommand::handleSetOptionCommand()
{
    this->isCommandExectuing = false;
}

void NS_UCICommands::UCICommand::handleGoCommand()
{
    std::shared_ptr<GoParams> params = std::make_shared<GoParams>();
    std::vector<std::string> paramsVector = Helper::split(this->commandParams,' ');
    for(int i = 0 ; i < paramsVector.size(); )
    {
        if (paramsVector[i].compare("wtime") == 0) {
            params->wtime = std::stoi(paramsVector[i+1]);
            i+=1;
        }
        else if (paramsVector[i].compare("btime") == 0) {
            params->btime = std::stoi(paramsVector[i+1]);
            i+=1;
        }
        else if (paramsVector[i].compare("winc") == 0) {
            params->winc = std::stoi(paramsVector[i+1]);
            i+=1;

        }
        else if (paramsVector[i].compare("binc") == 0) {
            params->binc = std::stoi(paramsVector[i+1]);
            i+=1;

        }
        else if (paramsVector[i].compare("movestogo") == 0) {
            params->movestogo = std::stoi(paramsVector[i+1]);
            i+=1;

        }
        else if (paramsVector[i].compare("depth") == 0) {
            params->depth = std::stoi(paramsVector[i+1]);
            i+=1;

        }
        else if (paramsVector[i].compare("nodes") == 0) {
            params->nodes = std::stoi(paramsVector[i+1]);
            i+=1;

        }
        else if (paramsVector[i].compare("mate") == 0) {
            params->mate = std::stoi(paramsVector[i+1]);
            i+=1;

        }
        else if (paramsVector[i].compare("movetime") == 0) {
            params->movetime = std::stoi(paramsVector[i+1]);
            i+=1;

        }
        else if (paramsVector[i].compare("infinite") == 0) {
            params->infinite = true;
          
        }
        else if (paramsVector[i].compare("searchmoves") == 0) {
            params->moves = std::vector<std::string>(paramsVector.begin()+i+1,paramsVector.end());
            break;
        }
        else if (paramsVector[i].compare("ponder") == 0) {
            params->ponder = true;
        }
        i+=1;
    }
    this->instance->engine->startCalculation(params);
    std::string ret = "bestmove ";
    this->instance->pushToOutputBufferAndWait(ret.append(this->instance->engine->bestMove));
    this->isCommandExectuing = false;

}
void UCIHandeling::pushToOutputBuffer(std::string& output)
{
    this->outputBufferMutex->lock();
    this->outputBuffer.push_back(output);
    this->outputBufferMutex->unlock();
}
void UCIHandeling::pushToOutputBufferAndWait(std::string& output)
{
    uint32_t temp;
    this->outputBufferMutex->lock();
    uint32_t messageBufferNumber = this->outputBufferCounter;
    this->outputBuffer.push_back(output);
    this->outputBufferMutex->unlock();
    bool isOutputSent = false;
    while(true)
    {
        this->outputBufferMutex->lock();
        temp = this->outputBufferCounter;
        this->outputBufferMutex->unlock();
        if(this->outputBufferCounter>messageBufferNumber)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void UCIHandeling::printBuffer()
{
    while (this->isRunning) {
        this->outputBufferMutex->lock();
        for(std::vector<std::string>::iterator it = this->outputBuffer.begin(); it != this->outputBuffer.end(); ++it) {
            std::cout << *it << std::endl;
            this->outputBufferCounter++;
        }
        this->outputBuffer.clear();
        this->outputBufferMutex->unlock();
    }
}

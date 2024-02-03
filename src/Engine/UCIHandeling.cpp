#include "UCIHandeling.hpp"
#include <string.h>

UCIHandeling::UCIHandeling(std::shared_ptr<Engine> engine)
{
    this->isRunning = true;
    this->isDebugMode = true;
    this->engine = engine;
}

void UCIHandeling::startCommandHandlerLoop()
{
    while(this->isRunning)
    {
        std::vector<std::string> arglist;
        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }
        char* token = strtok(const_cast<char*>(line.c_str()), " \t\n");
        while (token != nullptr) {
            arglist.push_back(token);
            token = strtok(nullptr, " \t\n");
        }
        if (!arglist.empty()) {
            if (!getCommandResponse(arglist)) {
                break;
            }
        }
    }
}
int UCIHandeling::getCommandResponse(std::vector<std::string>& arglist)
{

    return 0;
}
int UCIHandeling::startUciHandler()
{
    this->isRunning = true;
    startCommandHandlerLoop();
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

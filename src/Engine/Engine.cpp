#include "Engine.hpp"
#include "GameState.hpp"
#include <memory>
#include <unistd.h>
#include <vector>
#include <signal.h>

std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}
std::shared_ptr<Ply> Engine::getCurrentBestPly()
{
    return this->calculationState.bestPly;
}
sighandler_t interruptCalculation(int signum, sighandler_t handler)
{
        /*
        {
            std::shared_ptr<Ply> ply = getCurrentBestPly();
            std::""cout"" << "bestmove " << toLongAlgebraicNotation(ply) << std::endl;
            exit(0);
        });
        */
    
}
void Engine::startCalculation(std::shared_ptr<GoParams>)
{
    this->calculationState.isCalculating = true;
    int pid = fork();
    int pipefd[2] , writefd , readfd;
    readfd = pipefd[0];
    writefd = pipefd[1];
    if(pipe(pipefd) == -1)
    {
        std::cerr << "Failed to create pipe" << std::endl;
        throw std::runtime_error("Failed to create pipe");
    }
    if(pid < 0)
    {
        std::cerr << "Failed to fork" << std::endl;
        throw std::runtime_error("Failed to fork");
    }
    else if(pid == 0)
    {
        signal(SIGUSR1,interruptCalculation);
        //Temporary!!! This should be replaced with the actual calculation
        auto plys = this->moveGenerator->generateMoves(*this->game);
        this->setPlyAsBest(plys->at(0));
        exit(0);
    }
}

int Engine::getPid()
{
    if(!this->calculationState.isCalculating)
    {
        return 0;
    }
    return this->calculationState.pid;
}
void Engine::setPlyAsBest(std::shared_ptr<Ply> ply)
{
    this->calculationState.bestPly = ply;
}

std::string Engine::getSquareName(uint8_t square)
{
    std::string squareName = "";
    squareName += (char)('a' + (square % 8));
    squareName += (char)('1' + (square / 8));
    return squareName;
}
uint8_t Engine::getSquareFromName(std::string squareName)
{
    uint8_t square = 0;
    square += (squareName[0] - 'a');
    square += (squareName[1] - '1') * 8;
    return square;
}

void Engine::loadPositon(const std::string& fen,std::string moves)
{  
    //TODO:: This should stop calculations
    this->game = std::make_shared<GameState>(fen);
    std::vector<std::string> moveVec = split(moves,' ');
    //Remove the "moves" string
    if(moveVec.size() > 0)
    {
        moveVec.erase(moveVec.begin());
        for(auto move : moveVec)
        {
            uint8_t sourceSquare = getSquareFromName(move.substr(0,2));
            uint8_t targetSquare = getSquareFromName(move.substr(2,4));
            std::string promotion = move.substr(4,5);
//            this->game = this->game->playPly(sourceSquare,targetSquare);
            this->game->printGameState();
        }

    }
    
}
Engine::Engine (std::shared_ptr<MoveGenerator> moveGenerator)
{
    reset();
    this->moveGenerator = moveGenerator;
}
void Engine::reset()
{
    this->moveGenerator = nullptr;
    this->game = nullptr;
    this->calculationState.reset();
}

#include "UciNewGame.hpp"
int UciNewGame::execute()
{
    return 0;
}
UciNewGame::UciNewGame(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine) :
            UCICommand(arglist,engine)
{
    engine->reset();   
}
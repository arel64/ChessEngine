#include "Go.hpp"
int Go::execute()
{
    return 0;
}
Go::Go(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine) :
            UCICommand(arglist,engine)
{
    engine->startCalculation(nullptr);
}
#include "Stop.hpp"
#include "Go.hpp"
#include <signal.h>
int Go::execute()
{
    return 0;
}
Go::Go(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine) :
            UCICommand(arglist,engine)
{
    kill(engine->getPid(),SIGUSR1);
}
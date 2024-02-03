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
    pid_t pid = engine->getPid();
    if(pid == 0)
    {
        std::cerr << "Attempt to stop non existing calculation" << std::endl;
        return;
    }
    kill(pid,SIGUSR1);
}
#ifndef MACRO_STOP_HPP
#define MACRO_STOP_HPP
#include "UCICommand.hpp"
#include "Engine.hpp"
class Stop : public UCICommand
{
    public:
        Stop(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine) :
            UCICommand(arglist,engine) {}
        int execute() override;
};
#endif
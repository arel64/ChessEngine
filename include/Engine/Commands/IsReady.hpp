#ifndef MACRO_ISREADY_HPP
#define MACRO_ISREADY_HPP
#include "UCICommand.hpp"
class IsReady : public UCICommand
{
    public:
        IsReady(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine) :
            UCICommand(arglist,engine) {}
        int execute() override;
};

#endif
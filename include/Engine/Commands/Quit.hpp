#ifndef MACRO_QUIT_HPP
#define MACRO_QUIT_HPP
#include "UCICommand.hpp"
#include "Engine.hpp"
class Quit : public UCICommand
{
    public:
        Quit(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine) :
            UCICommand(arglist,engine) {}
        int execute() override;
};
#endif
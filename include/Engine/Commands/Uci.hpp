#ifndef MACRO_UCI_HPP
#define MACRO_UCI_HPP
#include "UCICommand.hpp"
class Uci : public UCICommand
{
    public:
        Uci(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine) :
            UCICommand(arglist,engine) {}
        int execute() override;
};

#endif
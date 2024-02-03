#ifndef MACRO_GO_HPP
#define MACRO_GO_HPP
#include "UCICommand.hpp"
#include "Engine.hpp"

class Go : public UCICommand
{
    public:
        Go(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine);
        int execute() override;
};
#endif
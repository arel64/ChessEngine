#ifndef MACRO_UCINEWGAME_HPP
#define MACRO_UCINEWGAME_HPP
#include "UCICommand.hpp"
class UciNewGame : public UCICommand
{
    public:
        UciNewGame(std::shared_ptr<std::vector<std::string>> arglist,std::shared_ptr<Engine> engine);
        int execute() override;
};
#endif
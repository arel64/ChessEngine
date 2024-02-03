#include "UCICommand.hpp"
UCICommand::UCICommand(std::shared_ptr<std::vector<std::string>> commandwordslist,std::shared_ptr<Engine> engine)
{
    this->engine = engine;
    this->arglist = commandwordslist;
}
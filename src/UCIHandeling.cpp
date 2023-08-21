#include "UCIHandeling.hpp"

int UCIHandeling::startUciHandler()
{
    std::string line;
    while (getline(std::cin, line)) {
        std::string commandstr = line.substr(0, line.find(" "));
        NS_UCICommands::UCICommandName command = NS_UCICommands::UCICommandStrings.at(commandstr);
        NS_UCICommands::UCICommand recievedCommand(command);
        recievedCommand.execute();
        /*if (line == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (line == "uci") {
            handleUCICommand();
        } else if (line.find("position") == 0) {
            handlePositionCommand(line);
        } else if (line.find("go") == 0) {
            handleGoCommand(line);
        } else if (line == "quit") {
            break;
        }*/
    }
    return 0;
}
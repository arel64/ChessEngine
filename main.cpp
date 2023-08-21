
#include "./src/Board.hpp"
#include "./src/GameState.hpp"
#include <memory>
#include <iostream>
#include <string>

using namespace std;
void handleUCICommand() {
    cout << "id name MySimpleEngine" << endl;
    cout << "id author YourName" << endl;
    // Other engine info commands here

    cout << "uciok" << endl;
}
void handlePositionCommand(const std::string& command) {
    // Extract position information from the command
    // Set up the chess board based on the provided FEN or moves

    // Example:
    // position startpos moves e2e4 e7e5

    // Your board setup logic here

    // Example response:
    // cout << "info setting up position" << endl;
}

void handleGoCommand(const std::string& command) {
    // Extract search parameters from the command
    // Implement your search and evaluation algorithms here

    // Example:
    // go wtime 300000 btime 300000 winc 0 binc 0 depth 5

    // Example response:
    // cout << "bestmove e2e4" << endl;
}

int main() {
    

    return 0;
}
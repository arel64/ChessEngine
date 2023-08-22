
#include "src/GameState.hpp"
#include "src/UCIHandeling.hpp"
#include <memory>
#include <iostream>
#include <string>

using namespace std;

int main() {
    std::shared_ptr<Engine> engine = std::make_unique<Engine>();
    std::unique_ptr<UCIHandeling> uciHandler = std::make_unique<UCIHandeling>(engine);
    uciHandler->startUciHandler();
 
    return 0;
}
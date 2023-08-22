
#include "src/UCIHandeling.hpp"
#include <memory>
#include <iostream>
#include <string>

using namespace std;

int main() {
    std::unique_ptr<UCIHandeling> UCIHandeler = std::make_unique<UCIHandeling>();
    UCIHandeler->startUciHandler();
    return 0;
}
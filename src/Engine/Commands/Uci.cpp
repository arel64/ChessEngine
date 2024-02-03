#include "Uci.hpp"
int Uci::execute()
{
    std::cout << "id name " << getEngine()->GetName() << std::endl;
    std::cout << "id author " << getEngine()->GetAuthor() << std::endl;
    std::cout << "uciok" << std::endl;
    return 0;
}
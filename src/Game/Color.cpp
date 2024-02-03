#include "Color.hpp"
#include <cassert>
PlayerColors::Color operator~(PlayerColors::Color color)
{
    if(color == PlayerColors::WHITE)
    {
        return PlayerColors::BLACK;
    }
    else if(color == PlayerColors::BLACK)
    {
        return PlayerColors::WHITE;
    }
    assert(false);
}
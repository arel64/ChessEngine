#ifndef MACRO_COLOR_HPP
#define MACRO_COLOR_HPP
    namespace PlayerColors
    {
        enum Color
        {
            WHITE=0,
            BLACK,
            ALL,
            NONE
        };
        static const Color PLAYER_COLORS[] = {WHITE,BLACK};
    }
    PlayerColors::Color operator~(PlayerColors::Color color);
#endif
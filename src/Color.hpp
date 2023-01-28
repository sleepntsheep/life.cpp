#pragma once
#include <cstdint>

class Color {
    public:
        int r, g, b, a;
        Color(int r, int g, int b, int a) : r{r}, g{g}, b{b}, a{a} {}
        Color(uint32_t rgb) {
            r = (rgb & 0xFF0000) >> 16;
            g = (rgb & 0x00FF00) >> 8;
            b = rgb & 0x0000FF;
            a = 255;
        }
};


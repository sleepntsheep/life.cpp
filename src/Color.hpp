#pragma once
#include <cstdint>

class Color {
    public:
        uint8_t r, g, b, a;
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r{r}, g{g}, b{b}, a{a} {}
        Color(uint32_t rgb) {
            r = (rgb & 0xFF0000) >> 16;
            g = (rgb & 0x00FF00) >> 8;
            b = rgb & 0x0000FF;
            a = 255;
        }
};


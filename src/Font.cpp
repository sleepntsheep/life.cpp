#include "Font.hpp"

Font::Font(std::string title, int size)
{
    sdl_font = TTF_OpenFont(title.c_str(), size);
    if (!sdl_font)
        throw "Failed opening font";
}

Font::~Font()
{
    TTF_CloseFont(sdl_font);
}


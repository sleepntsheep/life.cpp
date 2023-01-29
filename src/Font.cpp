#include "Font.hpp"

Font::Font(std::string title, int size) : title{title}, size{size}
{
    sdl_font = TTF_OpenFont(title.c_str(), size);
    if (!sdl_font)
        throw "Failed opening font";
}

Font::Font(const Font &f) : title{f.title}, size{f.size}
{
    sdl_font = TTF_OpenFont(f.title.c_str(), f.size);
    if (!sdl_font)
        throw "Failed opening font";
}

Font::~Font()
{
    TTF_CloseFont(sdl_font);
}


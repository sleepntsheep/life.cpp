#include <SDL2/SDL_ttf.h>
#include <string>

class Font
{
    public:
        Font(std::string title, int size);
        Font(const Font &);
        ~Font();
        TTF_Font *sdl_font = nullptr;

    private:
        std::string title;
        int size;
};


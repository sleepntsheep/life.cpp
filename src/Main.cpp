#include "Renderer.hpp"
#include "Life.hpp"
#include "Game.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    {
        Game game;

        if (argc == 2) game.life.load_from_file(argv[1]);

        game.run();
    }

    TTF_Quit();
    SDL_Quit();

    return 0;
}


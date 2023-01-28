#include "Renderer.hpp"
#include "Life.hpp"
#include "Game.hpp"
#include <iostream>

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    Game game;
    game.run();
    return 0;

    SDL_Quit();
}


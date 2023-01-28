#pragma once
#include "Renderer.hpp"
#include "Life.hpp"

class Game {
    enum GameState { Setup, Playing };

    public:
        Game() : renderer("Game Of Life", 800, 600, 60) {}
        ~Game() {}
        void run();

    private:
        Life life;
        Renderer renderer;
        enum GameState state = Setup;
        int cell_size = 16;
        int border_size = 1;
        Point offset{0, 0};
};


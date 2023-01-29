#pragma once
#include "Renderer.hpp"
#include "Life.hpp"

class Game {
    enum GameState { Setup, Playing };

    public:
        Game() : renderer("Game Of Life", 800, 600, 60),
                font("font.ttf", 24) {}
        ~Game() {}
        void run();

    private:
        Life life;
        Renderer renderer;
        Font font;

        enum GameState state = Setup;
        int cell_size = 16;
        bool show_border = true;
        int border_size = 1;
        Point offset{0, 0};

        void events();
        void update();
        void render();
};


#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <utility>
#include <functional>
#include "Color.hpp"
#include "Rect.hpp"

class Renderer {
    public:
        Renderer(const std::string &title, int width, int height, int fps);
        ~Renderer();

        void clear(Color color);
        void draw_rect(Rect rect, Color color);
        void render();
        void event(std::function<void(const SDL_Event &e)> f);
        int width = 0;
        int height = 0;
        bool quit = false;

    private:
        int fps;

        SDL_Window *sdl_win = nullptr;
        SDL_Renderer *sdl_rend = nullptr;
};


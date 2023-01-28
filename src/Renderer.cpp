#include "Renderer.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

Renderer::Renderer(const std::string &title, int width, int height, int fps)
{
    sdl_win = SDL_CreateWindow(title.c_str(),
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                width, height, SDL_WINDOW_RESIZABLE);

    sdl_rend = SDL_CreateRenderer(sdl_win, -1, 0);

    this->width = width;
    this->height = height;
    this->fps = fps;
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(sdl_rend);
    SDL_DestroyWindow(sdl_win);
}

void Renderer::clear(Color color)
{
    SDL_SetRenderDrawColor(sdl_rend, color.r, color.g, color.b, color.a);
    SDL_RenderClear(sdl_rend);
}

void Renderer::draw_rect(Rect rect, Color color)
{
    SDL_SetRenderDrawColor(sdl_rend, color.r, color.g, color.b, color.a);
    SDL_Rect r = {rect.x, rect.y, rect.w, rect.h};
    SDL_RenderFillRect(sdl_rend, &r);
}

void Renderer::render()
{
    SDL_RenderPresent(sdl_rend);
    SDL_Delay(1000.0f / fps);
}

void Renderer::event(std::function<void(const SDL_Event &e)> f)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    width = ev.window.data1;
                    height = ev.window.data2;
                }
                break;
        }
        f(ev);
    }
}


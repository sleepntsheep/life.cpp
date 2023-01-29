#include "Game.hpp"
#include <algorithm>
#include <sstream>

void Game::events()
{
    renderer.event([this](const SDL_Event &ev){
        if (ev.type == SDL_MOUSEBUTTONUP) {
            int x = ev.button.x;
            int y = ev.button.y;
            if (state == Setup) {
                Point pos{x / cell_size, y / cell_size};
                pos += offset;
                if (life.get(pos)) {
                    life.erase(pos);
                } else {
                    life.set(pos);
                }
            }
        }
        if (ev.type == SDL_KEYDOWN) {
            switch (ev.key.keysym.sym) {
                case SDLK_SPACE:
                    if (state == Setup) state = Playing;
                    else if (state == Playing) state = Setup;
                    break;
                case SDLK_b:
                    show_border ^= 1;
                    break;
                case SDLK_MINUS:
                    cell_size = std::max(0, cell_size - 1);
                    break;
                case SDLK_EQUALS:
                    cell_size++;
                    break;
                case SDLK_h:
                    offset.x--;
                    break;
                case SDLK_j:
                    offset.y++;
                    break;
                case SDLK_k:
                    offset.y--;
                    break;
                case SDLK_l:
                    offset.x++;
                    break;
                case SDLK_s:
                    life.save_to_file("save");
                    break;
            }
        }
    });
}

void Game::update()
{
    if (state == Playing) {
        life.do_generation();
    }
}

void Game::render()
{
    renderer.clear(Color(0x000000));

    if (show_border) {
        for (int i = cell_size; i < renderer.width; i += cell_size) {
            renderer.draw_rect(Rect{i, 0, border_size, renderer.height}, Color(0xBEBEBE));
        }
        for (int i = cell_size; i < renderer.height; i += cell_size) {
            renderer.draw_rect(Rect{0, i, renderer.width, border_size}, Color(0xBEBEBE));
        }
    }

    life.iterate_cells([this](const Point &cell){
        auto sp = cell + -offset;
        renderer.draw_rect(Rect{sp.x * cell_size, sp.y * cell_size,
                cell_size, cell_size}, Color(0xFFFFFF));
    });

    std::stringstream ss;
    ss << "Generation: " << life.generation << '\n'
        << "(X, Y) : (" << offset.x << ", " << offset.y << ")\n";

    renderer.draw_text(ss.str(), Point{10, 10}, font, Color(0xFFFF00));

    renderer.render();
}

void Game::run()
{
    while (!renderer.quit)
    {
        update();
        events();
        render();
    }
}


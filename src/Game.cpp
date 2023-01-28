#include "Game.hpp"
#include <algorithm>

void Game::run()
{
    while (!renderer.quit)
    {
        if (state == Playing) {
            life.do_generation();
        }

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
                }
            }
        });

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

        renderer.render();
    }
}


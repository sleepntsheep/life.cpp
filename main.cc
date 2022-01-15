#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#if defined(__EMSCRIPTEN__) 
#include <SDL.h>
#include <SDL_ttf.h>
#else 
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "main.h"

SDL_Renderer *renderer;
SDL_Window *window;
TTF_Font *font;
SDL_Color Green = {0, 255, 0, 255};

unsigned int tick1 = SDL_GetTicks();
unsigned int tick2 = SDL_GetTicks();
unsigned int frames = 0;
bool running = true;
double delta;
std::string game_state = "setup";

int cell_size = CELL_SIZE;
static Uint32 next_time;
int posX = CENTER_X / cell_size;
int posY = CENTER_Y / cell_size;
unsigned int GPS = GenPS;
unsigned int gen = 0;
int width = WIDTH;
int height = HEIGHT;
unsigned int gen_passed = 0;
bool border = 1;

std::set<std::pair<int, int>> keys;

Uint32 time_left(void) {
    Uint32 now = SDL_GetTicks();
    if (next_time <= now)
        return 0;
    return next_time - now;
}

std::vector<std::pair<int, int>> get_neighbors(int x, int y) {
    std::vector<std::pair<int, int>> neighbors;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;
            neighbors.push_back(std::make_pair(x + i, y + j));
        }
    }
    return neighbors;
}

void next_gen() {
    if (gen_passed == GPS)
        return;
    std::set<std::pair<int, int>> new_keys = keys;
    for (std::pair<int, int> cell : keys) {
        std::vector<std::pair<int, int>> neighbors = get_neighbors(cell.first, cell.second);
        int alive_neighbors = 0;
        for (std::pair<int, int> neighbor : neighbors) {
            if (keys.find(neighbor) != keys.end()){
                alive_neighbors++;
                continue;
            }
            int alive_neighbors_neighbors = 0;
            std::vector<std::pair<int, int>> neighbors_of_neighbor = get_neighbors(neighbor.first, neighbor.second);
            for (auto nei: neighbors_of_neighbor)
                if (keys.find(nei) != keys.end())
                    alive_neighbors_neighbors++;
            if (alive_neighbors_neighbors == 3)
                new_keys.insert(neighbor);
        }
        if (alive_neighbors < 2 || alive_neighbors > 3)
            new_keys.erase(cell);
    }
    keys = new_keys;
    gen++;
    gen_passed = 0;
};

void write(std::string str, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = TTF_RenderText_Blended(font, str.c_str(), Green);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect message_rect = { x, y, surface->w, surface->h };
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &message_rect);
    SDL_DestroyTexture(texture);
}

void input() {
    SDL_Event event;
    SDL_StartTextInput();
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (game_state == "setup")
                            game_state = "main_game";
                        else
                            game_state = "setup";
                        break;
                    case SDLK_j:
                        posY--;
                        break;
                    case SDLK_k:
                        posY++;
                        break;
                    case SDLK_h:
                        posX++;
                        break;
                    case SDLK_l:
                        posX--;
                        break;
                    case SDLK_b:
                        border = !border;
                        break;
                    case SDLK_EQUALS:
                        GPS++;
                        break;
                    case SDLK_MINUS:
                        if (GPS > 1)
                            GPS--;
                        break;
                    case SDLK_r:
                        keys.clear();
                        gen = 0;
                        game_state = "setup";
                        break;
                    case SDLK_1:
                        if (cell_size > 1)
                            cell_size--;
                        break;
                    case SDLK_2:
                        cell_size++;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (game_state == "setup") {
                    std::pair<int, int> position;
                    position.first = x / cell_size - posX;
                    position.second = y / cell_size - posY;
                    if (keys.find(position) == keys.end())
                        keys.insert(position);
                    else
                        keys.erase(position);
                }
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    width = event.window.data1;
                    height = event.window.data2;
                }
                break;
        }
    }
}


void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    if (border) {
        for (int i = cell_size; i < width; i += cell_size) {
            SDL_Rect r = {i, 0, BORDER_SIZE, height};
            SDL_RenderFillRect(renderer, &r);
        }

        for (int i = cell_size; i < height; i += cell_size) {
            SDL_Rect r = {0, i, width, BORDER_SIZE};
            SDL_RenderFillRect(renderer, &r);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto cell : keys) {
        SDL_Rect r = { (posX + cell.first) * cell_size, (posY + cell.second) * cell_size, cell_size, cell_size};
        SDL_RenderFillRect(renderer, &r);
    }

    const std::vector<std::string> osd = {
            "Generation: " + std::to_string(gen),
            "GPS: " + std::to_string(GPS),
            "FPS: " + std::to_string(FPS),
            "X, Y: " + std::to_string(posX) + ", " + std::to_string(posY),
    };

    int oy = 10,
        text_width,
        text_height;

    for (std::string line: osd) {
        TTF_SizeText(font, line.c_str(), &text_width, &text_height);
        write(line, 10, oy);
        oy += text_height;
    }

    SDL_RenderPresent(renderer);
}

void update()
{
    if (game_state != "main_game")
        return;
    if (frames % FPS == 0)
        gen_passed = 0;
    if (GPS <= 60){
        if ((frames % (FPS / GPS)) == 0)
            next_gen();
    }
    else {
        for (int i = 0; i < (int)GPS/FPS; i++)
            next_gen();
        if (frames % ((int)((float)GPS / (float)FPS) + 1) == 0)
            next_gen();
    }
    //if (frames % FPS == FPS - 1 && gen_passed < GPS)
    //    while (gen_passed < GPS)
    //        next_gen();
}

void main_loop() {
    update();
    input();
    render();

    SDL_Delay(time_left());
    next_time += 1000 / FPS;
    frames++;
}

#ifdef __MINGW32__
int WinMain() {
#else
int main() {
#endif
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        #ifndef __EMSCRIPTEN__
        std::cout << "Failed at SDL_Init()" <<  std::endl;
        #endif
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer) < 0)
        std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
    if (TTF_Init() < 0)
        std::cout << "Failed at TTF_Init" << TTF_GetError() << std::endl;
    
    font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font) 
        std::cout << "Failed at TTF_OpenFont()" << TTF_GetError() << std::endl;

    SDL_SetWindowTitle(window, "Conway's Game of Life");

    std::cout << "Click on cell to toggle\nPress space to switch between setup/main mode\nPress - + to change GPS (gen per sec)\nUse h, j, k, l to navigate around (vim keybind)\npress 1, 2 to change cell size" << std::endl;

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
    #else
    while (running)
        main_loop();
    #endif

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

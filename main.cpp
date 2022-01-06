#include <iostream>
#include <set>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

static Uint32 next_time;
int posX = CENTER_X / CELL_SIZE;
int posY = CENTER_Y / CELL_SIZE;
int GPS = GenPS;
int gen = 0;
int width = WIDTH;
int height = HEIGHT;

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
    std::set<std::pair<int, int>> new_keys = keys;
    for (auto cell : keys) {
        std::vector<std::pair<int, int>> neighbors = get_neighbors(cell.first, cell.second);
        int alive_neighbors = 0;
        for (auto neighbor : neighbors) {
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
};

void write(std::string str, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;
//    surface = TTF_RenderText_Blended(font, ("Gen: " + std::to_string(gen) + "\nMode: " + game_state + "\nX, Y: " + std::to_string(posX) + ", " + std::to_string(posY)).c_str(), Green);
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
                        std::cout << game_state << std::endl;
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
                    case SDLK_EQUALS:
                        GPS++;
                        break;
                    case SDLK_MINUS:
                        if (GPS > 1)
                            GPS--;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (game_state == "setup") {
                    std::pair<int, int> position;
                    position.first = x / CELL_SIZE - posX;
                    position.second = y / CELL_SIZE - posY;
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

    for (int i = CELL_SIZE; i < width; i += CELL_SIZE) {
        SDL_Rect r = {i, 0, BORDER_SIZE, height};
        SDL_RenderFillRect(renderer, &r);
    }

    for (int i = CELL_SIZE; i < height; i += CELL_SIZE) {
        SDL_Rect r = {0, i, width, BORDER_SIZE};
        SDL_RenderFillRect(renderer, &r);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (auto cell : keys) {
        SDL_Rect r = { (posX + cell.first) * CELL_SIZE, (posY + cell.second) * CELL_SIZE, CELL_SIZE, CELL_SIZE};
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

void update() {
    if ((frames % (FPS / GPS)) == 0 && game_state == "main_game")
        next_gen();
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Failed at SDL_Init()" <<  std::endl;
    if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer) < 0)
        std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
    if (TTF_Init() < 0)
        std::cout << "Failed at TTF_Init" << TTF_GetError() << std::endl;
    
    font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font) 
        std::cout << "Failed at TTF_OpenFont()" << TTF_GetError() << std::endl;

    SDL_SetWindowTitle(window, "Conway's Game of Life");

    while (running) {
        update();
        input();
        render();

        SDL_Delay(time_left());
        next_time += 1000 / FPS;
        frames++;
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

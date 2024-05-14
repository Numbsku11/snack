#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 20;
const int INITIAL_SNAKE_LENGTH = 5;

// SDL variables
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Game variables
vector<pair<int, int>> snake;
pair<int, int> food;
int dx = 1;
int dy = 0;
bool gameover = false;

void Initialize() {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initialize snake
    snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    for (int i = 1; i < INITIAL_SNAKE_LENGTH; i++) {
        snake.push_back({snake[0].first - i * GRID_SIZE, snake[0].second});
    }

    // Initialize food
    food = {rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE, rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE};
}

void DrawRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void HandleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            gameover = true;
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (dy == 0) {
                        dx = 0;
                        dy = -1;
                    }
                    break;
                case SDLK_DOWN:
                    if (dy == 0) {
                        dx = 0;
                        dy = 1;
                    }
                    break;
                case SDLK_LEFT:
                    if (dx == 0) {
                        dx = -1;
                        dy = 0;
                    }
                    break;
                case SDLK_RIGHT:
                    if (dx == 0) {
                        dx = 1;
                        dy = 0;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

bool CheckCollision() {
    // Check collision with the walls
    if (snake[0].first < 0 || snake[0].first >= SCREEN_WIDTH || snake[0].second < 0 || snake[0].second >= SCREEN_HEIGHT) {
        return true;
    }

    // Check collision with itself
    for (size_t i = 1; i < snake.size(); i++) {
        if (snake[i] == snake[0]) {
            return true;
        }
    }

    return false;
}

void Update() {
    // Move the snake
    int newHeadX = snake[0].first + dx * GRID_SIZE;
    int newHeadY = snake[0].second + dy * GRID_SIZE;

    // Check if the snake eats the food
    if (newHeadX == food.first && newHeadY == food.second) {
        snake.insert(snake.begin(), {newHeadX, newHeadY});
        food = {rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE, rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE};
    }
    else {
        // Move the snake by adding a new head and removing the tail
        snake.insert(snake.begin(), {newHeadX, newHeadY});
        snake.pop_back();
    }

    // Check for collisions
    if (CheckCollision()) {
        gameover = true;
    }
}

void Render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the snake
    for (const auto& segment : snake) {
        DrawRect(segment.first, segment.second, GRID_SIZE, GRID_SIZE, {0, 255, 0, 255});
    }

    // Draw the food
    DrawRect(food.first, food.second, GRID_SIZE, GRID_SIZE, {255, 0, 0, 255});

    // Present the rendered content
    SDL_RenderPresent(renderer);
}

void CleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    Initialize();

    while (!gameover) {
        HandleInput();
        Update();
        Render();
        SDL_Delay(100); // Adjust the speed of the game
    }

    CleanUp();

    return 0;
}

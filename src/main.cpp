#include <queue>
#include <array>
#include <stdlib.h>
#include <raylib.h>
#include <assert.h>
#include <time.h>

inline constexpr int WIDTH = 800;
inline constexpr int HEIGHT = 800;
inline constexpr int FPS = 60;
inline constexpr int CELL_SIZE = 38;
inline constexpr int CELL_WIDTH = 19;
inline constexpr int CELL_HEIGHT = 19;
inline constexpr char TITLE[] = "Snake";
inline constexpr Color BACKGROUND_COLOR = {87, 138, 52, 255};
inline constexpr Color BOARD_COLOR_1 = {170, 215, 81, 255};
inline constexpr Color BOARD_COLOR_2 = {162, 209, 73, 255};
inline constexpr Color SNAKE_COLOR = {63, 108, 222, 255};
inline constexpr Color APPLE_COLOR = {231, 71, 29, 255};
inline constexpr KeyboardKey RELOAD = KEY_R;

unsigned framesElapsed;
unsigned gameOver;

std::deque<std::array<int, 2>> snake;
int appleX, appleY, dx, dy;

int AsIndex(int x, int y) {
    return y * CELL_WIDTH + x;
}

void PutApple() {
    int rx = rand() % CELL_WIDTH,
        ry = rand() % CELL_HEIGHT;
    
    for (auto [x, y] : snake) {
        if (rx == x && ry == y) {
            PutApple();
        }
    }

    appleX = rx;
    appleY = ry;
}

void Reload() {
    framesElapsed = 0;
    gameOver = 0;

    dx = 1;
    dy = 0;

    snake = {{7, 9}, {6, 9}, {5, 9}};
    PutApple();
}

int main() {

    srand((unsigned)time(nullptr));

    assert(!GetWindowHandle());

    SetTargetFPS(FPS);
    InitWindow(WIDTH, HEIGHT, TITLE);

    Reload();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        for (int i = 0; i < CELL_HEIGHT; i++) {
            for (int j = 0; j < CELL_WIDTH; j++) {
                DrawRectangle(
                    (j + 1) * CELL_SIZE,
                    (i + 1) * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    AsIndex(j, i) & 1 ? 
                        BOARD_COLOR_2 :
                        BOARD_COLOR_1
                );
            }
        }

        for (auto [x, y] : snake) {
            DrawRectangle(
                (x + 1) * CELL_SIZE,
                (y + 1) * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
                SNAKE_COLOR
            );
        }

        DrawRectangle(
            (appleX + 1) * CELL_SIZE,
            (appleY + 1) * CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            APPLE_COLOR
        );

        EndDrawing();

        if (IsKeyPressed(RELOAD)) {
            Reload();
            goto nextTick;
        }

        snake.push_front({snake.front()[0] + dx, snake.front()[1] + dy});

        if (snake.front()[0] == appleX && snake.front()[1] == appleY) {
            PutApple();
        } else {
            snake.pop_back();
        }

        for (int i = 1; i < snake.size(); i++) {
            if (snake.front()[0] == snake[i][0] && snake.front()[1] == snake[i][1]) {
                Reload();
                goto nextTick;
            }
        }

        framesElapsed += !gameOver;

        nextTick:continue;
    }

    assert(GetWindowHandle());
    CloseWindow();
}

#include <queue>
#include <iostream>
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

std::deque<int> sx, sy;
int appleX, appleY, dx, dy;

int AsIndex(int x, int y) {
    return y * CELL_WIDTH + x;
}

void Reload() {
    framesElapsed = 0;
    gameOver = 0;

    dx = 1;
    dy = 0;

    sx = {7, 6, 5};
    sy = {9, 9, 9};

    while (1) {
        int rx = rand() % CELL_WIDTH,
            ry = rand() % CELL_HEIGHT;
        
        if (ry != 9 && rx != 5 && rx != 6 && rx != 7) {
            appleX = rx;
            appleY = ry;
            break;
        }
    }
}

int main() {

    srand((unsigned)time(nullptr));

    assert(!GetWindowHandle());

    SetTargetFPS(60);
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

        for (int i = 0; i < sx.size(); i++) {
            DrawRectangle(
                (sx[i] + 1) * CELL_SIZE,
                (sy[i] + 1) * CELL_SIZE,
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

        std::cerr << sx.size() << '\n';

        if (IsKeyPressed(RELOAD)) {
            Reload();
            continue;
        }

        bool up    = IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP),
             down  = IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN),
             left  = IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT),
             right = IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT);
        
        if (up && (dx == -1 || dx == 1)) {
            dx = 0;
            dy = -1;
        }
        else if (down && (dx == -1 || dx == 1)) {
            dx = 0;
            dy = 1;
        }
        else if (left && (dy == -1 || dy == 1)) {
            dy = 0;
            dx = -1;
        }
        else if (right && (dy == -1 || dy == 1)) {
            dy = 0;
            dx = 1;
        }

        if (framesElapsed % (FPS >> 3) == 0) {
            sx.push_front(sx.front() + dx);
            sy.push_front(sy.front() + dy);
            
            if (sx.front() == appleX && sy.front() == appleY) {
                while (1) {
                    int rx = rand() % CELL_WIDTH;
                    int ry = rand() % CELL_HEIGHT;
                    int i;

                    for ( i = 0; i < sx.size(); i++) {
                        if (sx[i] == rx && sy[i] == ry) {
                            break; 
                        }
                    }

                    if (i == sx.size()) {
                        appleX = rx;
                        appleY = ry;
                        break;
                    }
                }
            } else {
                sx.pop_back();
                sy.pop_back();
            }

            for (int i = 1; i < sx.size(); i++) {
                if (sx.front() == sx[i] && sy.front() == sy[i]) {
                    Reload();
                    goto nextTick;
                }
            }

            if (sx.front() < 0 || sy.front() < 0 || 
                sx.front() >= CELL_WIDTH || sy.front() >= CELL_HEIGHT) {
                Reload();
                continue;
            }
        }

        framesElapsed += !gameOver;

        nextTick:continue;
    }

    assert(GetWindowHandle());
    CloseWindow();
}

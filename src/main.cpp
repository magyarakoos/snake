#include <queue>
#include <iostream>
#include <stdlib.h>
#include <raylib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#include "settings.h"

unsigned framesElapsed;
bool gameOver;
bool started;

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
        if (gameOver) {
            sleep(1);
            Reload();
            continue;
        }

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

        if (!started) {
            DrawRectangle(0, 0, WIDTH, HEIGHT, {0, 0, 0, 100});

            DrawTriangle(
                {
                    WIDTH / 2 - SQ_2 * CELL_SIZE,
                    HEIGHT / 2 - SQ_2 * CELL_SIZE
                },
                {
                    WIDTH / 2 - SQ_2 * CELL_SIZE,
                    HEIGHT / 2 + SQ_2 * CELL_SIZE
                },
                {
                    WIDTH / 2 + SQ_2 * CELL_SIZE,
                    HEIGHT / 2
                },
                WHITE
            );
        }

        EndDrawing();

        if (!started && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            started = 1;
        }

        if (!started) {
            continue;
        }

        bool up    = IsKeyDown(KEY_W) || IsKeyDown(KEY_UP),
             down  = IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN),
             left  = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT),
             right = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);

        if (framesElapsed % (FPS / TPS) == 0) {

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

            // epic win (you get nothing)
            if (sx.size() == CELL_WIDTH * CELL_HEIGHT) {
                gameOver = 1;
            }

            if (sx.front() < 0 || sy.front() < 0 || 
                sx.front() >= CELL_WIDTH || sy.front() >= CELL_HEIGHT) {
                gameOver = 1;
            }


            for (int i = 0; i < sx.size(); i++) {
                std::cerr << sx[i] << ' ';
            }
            std::cerr << '\n';

            for (int i = 1; i < sx.size(); i++) {
                if (sx.front() == sx[i] && sy.front() == sy[i]) {
                    gameOver = 1;
                    break;
                }
            }
        }

        framesElapsed++;
    }

    assert(GetWindowHandle());
    CloseWindow();
}
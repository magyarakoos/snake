#ifndef SETTINGS_H
#define SETTINGS_H

#include <raylib.h>

#define WIDTH 800
#define HEIGHT 800
#define FPS 60
#define TPS 7 // set difficulty with this
#define CELL_SIZE 38
#define CELL_WIDTH 19
#define CELL_HEIGHT 19
#define SQ_2 1.41421356237
#define MAX_SIZE CELL_WIDTH * CELL_HEIGHT

const char TITLE[] = "Snake";
const Color BACKGROUND_COLOR = {87, 138, 52, 255};
const Color BOARD_COLOR_1 = {170, 215, 81, 255};
const Color BOARD_COLOR_2 = {162, 209, 73, 255};
const Color SNAKE_COLOR = {63, 108, 222, 255};
const Color APPLE_COLOR = {231, 71, 29, 255};
const Color DARK = {0, 0, 0, 100};

#endif
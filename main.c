#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

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

unsigned framesElapsed;
bool gameOver;
bool started;

typedef struct Node Node;

struct Node {
    int x, y;
    Node* next;
};

Node* snake;
int appleX, appleY, dx, dy, size;

void SetApple() {
    while (1) {
        int rx = rand() % CELL_WIDTH;
        int ry = rand() % CELL_HEIGHT;

        Node* curr = snake;
        while (curr) {
            if (curr->x == rx && curr->y == ry) {
                break;
            }
            curr = curr->next;
        }

        if (!curr) {
            appleX = rx;
            appleY = ry;
            break;
        }
    }
}

void Reload() {
    framesElapsed = 0;
    gameOver = 0;
    size = 0;

    dx = 1;
    dy = 0;

    // ooc: what the fuck
    snake = (Node*)malloc(sizeof(Node));
    snake->x = 7;
    snake->y = 9;
    snake->next = (Node*)malloc(sizeof(Node));
    snake->next->x = 6;
    snake->next->y = 9;
    snake->next->next = (Node*)malloc(sizeof(Node));
    snake->next->next->x = 5;
    snake->next->next->y = 9;
    snake->next->next->next = NULL;

    SetApple();
}

int main() {

    srand((unsigned)time(NULL));

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
                    i * CELL_WIDTH + j & 1 ? 
                        BOARD_COLOR_2 :
                        BOARD_COLOR_1
                );
            }
        }

        {
            Node* curr = snake;
            while (curr) {
                DrawRectangle(
                    (curr->x + 1) * CELL_SIZE,
                    (curr->y + 1) * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    SNAKE_COLOR
                );
                curr = curr->next;
            }
        }

        DrawRectangle(
            (appleX + 1) * CELL_SIZE,
            (appleY + 1) * CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE,
            APPLE_COLOR
        );

        if (!started) {
            DrawRectangle(0, 0, WIDTH, HEIGHT, DARK);

            Vector2 v1 = {
                WIDTH / 2 - SQ_2 * CELL_SIZE,
                HEIGHT / 2 - SQ_2 * CELL_SIZE
            };
            Vector2 v2 = {
                WIDTH / 2 - SQ_2 * CELL_SIZE,
                HEIGHT / 2 + SQ_2 * CELL_SIZE
            };
            Vector2 v3 = {
                WIDTH / 2 + SQ_2 * CELL_SIZE,
                HEIGHT / 2
            };

            DrawTriangle(v1, v2, v3, WHITE);
        }

        EndDrawing();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
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

            {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->x = snake->x + dx;
                newNode->y = snake->y + dy;
                newNode->next = snake;
                snake = newNode;
            }

            size++;
            if (snake->x == appleX && snake->y == appleY) {
                SetApple();
            } else {
                Node* curr = snake;
                Node* prev = NULL;
                while (curr->next) {
                    prev = curr;
                    curr = curr->next;
                }
                prev->next = NULL;
                size--;
            }

            // epic win (you get nothing)
            if (size == CELL_WIDTH * CELL_HEIGHT) {
                gameOver = 1;
            }

            if (snake->x < 0 || snake->y < 0 || 
                snake->x >= CELL_WIDTH || snake->y >= CELL_HEIGHT) {
                gameOver = 1;
            }

            {
                Node* curr = snake->next;
                while (curr) {
                    if (curr->x == snake->x && curr->y == snake->y) {
                        gameOver = 1;
                        break;
                    }
                    curr = curr->next;
                }
            }
        }

        framesElapsed++;
    }

    assert(GetWindowHandle());
    CloseWindow();
}
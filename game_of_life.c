#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/neslib/neslib.h"

// general vars
static uint8_t spr;
static uint8_t input;

// map vars
#define WIDTH       256
#define HEIGHT      240
#define TILE_SIZE   9
#define TILES_X     15
#define TILES_Y     15

static const uint8_t ALIVE = 1;
static const uint8_t DEAD = 0;
static const uint8_t DX = (WIDTH - (TILES_X * TILE_SIZE)) / 2;
static const uint8_t DY = (HEIGHT - (TILES_Y * TILE_SIZE)) / 2;

static uint8_t i, frame = 0, paused = 0;
static uint8_t x, y;

static uint8_t fg_layer = 0;
static uint8_t bg_layer = 1;
static uint8_t neighbors;
static uint8_t next_state;
static uint8_t layers[2][TILES_Y][TILES_X] = {
        { // https://conwaylife.com/wiki/Pinwheel
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                {0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0},
                {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0},
                {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
        { // https://conwaylife.com/wiki/Pulsar
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
                {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
                {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
                {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
                {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
                {0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        }
};

/**
 * Handle wrap-around.
 * e.g.
 * TILES_X = 4
 * x = 0 -> (0 + 4) % 4 -> 4 % 4 -> 0
 * x = 1 -> (1 + 4) % 4 -> 5 % 4 -> 1
 * x = 2 -> (2 + 4) % 4 -> 6 % 4 -> 2
 * x = 3 -> (3 + 4) % 4 -> 7 % 4 -> 3
 * x = 4 -> (4 + 4) % 4 -> 8 % 4 -> 0
 */
uint8_t get_wrap(uint8_t x, uint8_t y) {
    return layers[fg_layer][(y + TILES_Y) % TILES_Y][(x + TILES_X) % TILES_X];
}

uint8_t get(uint8_t x, uint8_t y) {
    if (x < 0 || x >= TILES_X || y < 0 || y >= TILES_Y) {
        return 0;
    }
    return layers[fg_layer][y][x];
}

uint8_t count_alive_neighbors(uint8_t x, uint8_t y) {
    return get(x - 1, y - 1) + get(x, y - 1) + get(x + 1, y - 1) +
           get(x - 1, y) + get(x + 1, y) +
           get(x - 1, y + 1) + get(x, y + 1) + get(x + 1, y + 1);
}

static void randomize_map() {
    for (y = 0; y < TILES_Y; y++) {
        for (x = 0; x < TILES_X; x++) {
            if (rand8() > 207) {
                layers[fg_layer][y][x] = ALIVE;
            } else {
                layers[fg_layer][y][x] = DEAD;
            }
            layers[bg_layer][y][x] = DEAD;
        }
    }
}

static void clear_map() {
    for (y = 0; y < TILES_Y; y++) {
        for (x = 0; x < TILES_X; x++) {
            layers[fg_layer][y][x] = DEAD;
            layers[bg_layer][y][x] = DEAD;
        }
    }
}

/**
 *  @
 *   @
 * @@@
 * [y, x]
 */
void build_glider() {
    clear_map();
    x = 3;
    y = 3;
    layers[fg_layer][y + 2][x + 1] = ALIVE; // top
    layers[fg_layer][y + 1][x + 2] = ALIVE; // middle
    layers[fg_layer][y + 0][x + 0] = ALIVE; // ------
    layers[fg_layer][y + 0][x + 1] = ALIVE; // bottom
    layers[fg_layer][y + 0][x + 2] = ALIVE; // ------
}

void start_pinwheel() {
    fg_layer = 0;
    bg_layer = 1;
}
void start_pulsar() {
    fg_layer = 1;
    bg_layer = 0;
}

const uint8_t sprites_pal[32] = {
        0x0f, 0x17, 0x27, 0x37,
        0x0f, 0x29, 0x21, 0x31,
        0x0f, 0x15, 0x25, 0x35,
        0x0f, 0x19, 0x29, 0x39,
        0x0f, 0x05, 0x10, 0x10,
        0x0f, 0x07, 0x12, 0x14,
        0x0f, 0x30, 0x8f, 0x33,
};

static void swap_layers() {
    bg_layer = !bg_layer;
    fg_layer = !fg_layer;
}

static void handle_input() {
    input = pad_trigger(0);

    if (input & PAD_A + input & PAD_B) {
        randomize_map();
    }
    if (input & PAD_START) {
        paused = !paused;
    }
}

static compute_next_state() {
    for (y = 0; y < TILES_Y; y++) {
        for (x = 0; x < TILES_X; x++) {
            neighbors = count_alive_neighbors(x, y);
            next_state = layers[fg_layer][y][x]; // default to carry over state
            if (layers[fg_layer][y][x] == ALIVE) {
                // 1. Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
                // 2. Any live cell with more than three live neighbors dies, as if by overcrowding.
                if (neighbors < 2 || neighbors > 3) {
                    next_state = DEAD;

                }
                    // 3. Any live cell with two or three live neighbors lives on to the next generation.
                else {
                    next_state = ALIVE;
                }
            } else {
                // 4. Any dead cell with exactly three live neighbors becomes a live cell.
                if (neighbors == 3) {
                    next_state = ALIVE;
                }
                    // 5. Else cell remains dead.
                else {
                    next_state = DEAD;
                }
            }

            layers[bg_layer][y][x] = next_state;
        }
    }
}

static render() {
    oam_clear();
    spr = 0;
    for (y = 0; y < TILES_Y; ++y) {
        for (x = 0; x < TILES_X; ++x) {
            if (layers[fg_layer][y][x] == ALIVE) {
                neighbors = count_alive_neighbors(x, y);
                spr = oam_spr(DX + x * TILE_SIZE, DY + y * TILE_SIZE + 8, layers[fg_layer][y][x], neighbors, spr);
            }
        }
    }
}

void main(void) {
   //randomize_map();
   //build_glider();
   start_pinwheel();
   //start_pulsar();

    pal_spr(sprites_pal);   // set palette for sprites
    ppu_on_all();           // enable rendering

    while (1) {
        ppu_wait_frame();   // wait for next TV frame

        handle_input();

        if (!paused && frame % 2 == 0) {
            compute_next_state();
        }

        render();

        if (!paused && frame % 2 == 0) {
            swap_layers();
        }

        ++frame;
    }

}

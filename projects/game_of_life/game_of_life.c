#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lib/neslib/neslib.h"

// general vars
static uint8_t spr;

// map vars
#define WIDTH       256
#define HEIGHT      240
#define TILE_SIZE   13
#define TILES_X     13
#define TILES_Y     13

static const uint8_t ALIVE = 1;
static const uint8_t DEAD = 0;
static const uint8_t DX = (WIDTH - (TILES_X * TILE_SIZE)) / 2;
static const uint8_t DY = (HEIGHT - (TILES_Y * TILE_SIZE)) / 2;

static uint8_t x, y;
static uint8_t layers[2][TILES_Y][TILES_X] = {
        {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                {0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
                {0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
                {0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0},
                {0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        },
        {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        }
};

static uint8_t fg_layer = 0;
static uint8_t bg_layer = 1;
static uint8_t neighbors;

uint8_t get(uint8_t x, uint8_t y) {
    return layers[fg_layer][(y + TILES_Y) % TILES_Y][(x + TILES_X) % TILES_X];
}

uint8_t count_alive_neighbors(uint8_t x, uint8_t y) {
    return get(x - 1, y - 1) + get(x, y - 1) + get(x + 1, y - 1) +
           get(x - 1, y)     + /*        */    get(x + 1, y) +
           get(x - 1, y + 1) + get(x, y + 1) + get(x + 1, y + 1);
}

void randomize_map() {
    for (y = 0; y < TILES_Y; y++) {
        for (x = 0; x < TILES_X; x++) {
            if (rand8() > 215) {
                layers[fg_layer][y][x] = ALIVE;
            } else {
                layers[fg_layer][y][x] = DEAD;
            }
            layers[bg_layer][y][x] = DEAD;
        }
    }
}

const uint8_t sprites_pal[32] = {
        0x0f, 0x17, 0x27, 0x37,
        0x0f, 0x11, 0x21, 0x31,
        0x0f, 0x15, 0x25, 0x35,
        0x0f, 0x19, 0x29, 0x39,
        0x0f, 0x05, 0x10, 0x10,
        0x0f, 0x07, 0x12, 0x14,
        0x0f, 0x09, 0x14, 0x18,
        0x0f, 0x11, 0x16, 0x22,

//        0x0f, 0x05, 0x05, 0x05,
//        0x0f, 0x10, 0x10, 0x10,
//        0x0f, 0x15, 0x15, 0x15,
//        0x0f, 0x20, 0x20, 0x20,
//        0x0f, 0x25, 0x25, 0x25,
//        0x0f, 0x30, 0x30, 0x30,
//        0x0f, 0x35, 0x35, 0x35,
//        0x0f, 0x40, 0x40, 0x40,
};

static void swap_layers() {
    bg_layer = (bg_layer + 1) % 2;
    fg_layer = (fg_layer + 1) % 2;
}

void main(void) {
    randomize_map();

    pal_spr(sprites_pal);    // set palette for sprites

    ppu_on_all();           // enable rendering

    while (1) {
        ppu_wait_frame();
//        ppu_wait_nmi();
        for (y = 0; y < TILES_Y; ++y) {
            for (x = 0; x < TILES_X; ++x) {
                if (layers[fg_layer][y][x] > 0) {
                    neighbors = count_alive_neighbors(x, y);
                    spr = oam_spr(DX + x * TILE_SIZE, DY + y * TILE_SIZE + 8, layers[fg_layer][y][x], neighbors, spr); // 0x40 is tile number, 1 is palette
                }
            }
        }

        for (y = 0; y < TILES_Y; ++y) {
            for (x = 0; x < TILES_X; ++x) {
                neighbors = count_alive_neighbors(x, y);
                if (layers[fg_layer][y][x] == ALIVE) {
                    // 1. Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
                    // 2. Any live cell with more than three live neighbors dies, as if by overcrowding.
                    if (neighbors < 2 || neighbors > 3) {
                        layers[bg_layer][y][x] = DEAD;
                        spr = oam_spr(DX + x * TILE_SIZE, DY + y * TILE_SIZE + 8, 0, 0, spr); // clean up tile
                    } else {
                        layers[bg_layer][y][x] = ALIVE;
                    }
                    // 3. Any live cell with two or three live neighbors lives on to the next generation.
                } else {
                    // 4. Any dead cell with exactly three live neighbors becomes a live cell.
                    if (neighbors == 3) {
                        layers[bg_layer][y][x] = ALIVE;
                    } else {
                        layers[bg_layer][y][x] = DEAD;
                    }
                }
            }
        }

        swap_layers();
    }
}
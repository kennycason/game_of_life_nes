#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <joystick.h>
#include "lib/neslib/neslib.h"

// general vars
static uint8_t i, j;
static uint8_t spr;
static uint8_t joy0;

// map vars
#define TILE_SIZE   16
#define WIDTH       256
#define HEIGHT      240
#define TILES_X     WIDTH / TILE_SIZE
#define TILES_Y     HEIGHT / TILE_SIZE

static const uint8_t ALIVE = 1;
static const uint8_t DEAD = 0;

static uint8_t x, y;
static uint8_t layers[2][TILES_Y][TILES_X] = {
        { // map[y][x]
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0},

                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 50, 50, 0, 0,  0, 0, 0, 0, 0, 0},

                {0, 0, 0,  0, 0, 0, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

        },
        {// map[y][x]
                {0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 40},
                {0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 40},
                {0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0},

                {0, 0, 6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 40, 0, 0, 0},
                {0, 0, 7, 0, 0, 0, 1, 0, 0, 0, 0, 40, 0, 0, 0, 0},
                {0, 0, 8, 0, 0, 0, 1, 0, 0, 0, 40, 0, 0, 0, 0, 0},
                {0, 0, 9, 0, 0, 0, 0,  0,  0, 40, 0, 0, 0, 0, 0, 0},

                {0, 0, 10, 0, 0, 0, 0,  0,  0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30}
        }
};
//static uint8_t (*fg_ptr)[TILES_Y][TILES_X] = &fg;

//static uint8_t bg[TILES_Y][TILES_X] = { // map[y][x]
//        {0,  1, 2,  0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  40, 40},
//        {0,  0, 3,  0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  40, 40},
//        {0,  0, 4,  0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
//        {0,  0, 5,  0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  40, 0,  0},
//
//        {0,  0, 6,  0, 0, 0, 0, 0, 0, 0,  0,  0,  40, 0,  0,  0},
//        {0,  0, 7,  0, 0, 0, 0, 0, 0, 0,  0,  40, 0,  0,  0,  0},
//        {0,  0, 8,  0, 0, 0, 0, 0, 0, 0,  40, 0,  0,  0,  0,  0},
//        {0,  0, 9,  0, 0, 0, 0, 0, 0, 40, 0,  0,  0,  0,  0,  0},
//
//        {0,  0, 10, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
//        {0,  0, 11, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
//        {0,  0, 12, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
//        {0,  0, 13, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
//
//        {0,  0, 0,  0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
//        {0,  0, 0,  0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0},
//        {20, 0, 0,  0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  30}
//};
//static uint8_t (*bg_ptr)[TILES_Y][TILES_X] = &bg;
//static uint8_t (*swap_ptr)[TILES_Y][TILES_X];
static uint8_t bg_layer = 0;
static uint8_t fg_layer = 1;
static uint8_t neighbors;

uint8_t get(uint8_t x, uint8_t y) {
    return layers[fg_layer][(y + TILES_Y) % TILES_Y][(x + TILES_X) % TILES_X];
}

uint8_t count_alive_neighbors(uint8_t x, uint8_t y) {
    return get(x - 1, y - 1) + get(x, y - 1) + get(x + 1, y - 1) +
           get(x - 1, y) + get(x + 1, y) +
           get(x - 1, y + 1) + get(x, y + 1) + get(x + 1, y + 1);
}

void randomize_map() {
    for (y = 0; y < TILES_Y; y++) {
        for (x = 0; x < TILES_X; x++) {
            if (rand8() > 220) {
                layers[fg_layer][y][x] = ALIVE;
            } else {
                layers[fg_layer][y][x] = DEAD;
            }
            //   layers[bg_layer][y][x] = DEAD;
        }
    }
}

const uint8_t palSprites[16] = {
        0x0f, 0x17, 0x27, 0x37,
        0x0f, 0x11, 0x21, 0x31,
        0x0f, 0x15, 0x25, 0x35,
        0x0f, 0x19, 0x29, 0x39
};

void swap_layers() {
    bg_layer = bg_layer == 1 ? 0 : 1;
    fg_layer = fg_layer == 1 ? 0 : 1;
//    swap_ptr = fg_ptr;
//    fg_ptr = bg_ptr;
//    bg_ptr = swap_ptr;
}

void main(void) {
    set_rand(12341);
    pal_spr(palSprites); // set palette for sprites

    ppu_on_all(); //enable rendering

    randomize_map();

    // music_play(0);

    //swap_layers();
    while (1) {
        ppu_wait_frame();

//        joy0 = joy_read(0);
//        if (JOY_BTN_1(joy0)) {
//         //   balls_count--;
//        }
//        if (JOY_BTN_2(joy0)) {
//         //   balls_count++;
//        }

        for (y = 0; y < TILES_Y; y++) {
            for (x = 0; x < TILES_X; x++) {
                if (layers[fg_layer][y][x] > 0) {
//                    neighbors = count_alive_neighbors(x, y);
                    spr = oam_spr(x * TILE_SIZE, y * TILE_SIZE, 0x20, 3, spr); // 0x40 is tile number, 1 is palette
                }
            }
        }

        for (y = 0; y < TILES_Y; y++) {
            for (x = 0; x < TILES_X; x++) {
                neighbors = count_alive_neighbors(x, y);
                if (layers[fg_layer][y][x] == ALIVE) {
                    // 1. Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
                    // 2. Any live cell with more than three live neighbors dies, as if by overcrowding.
                    if (neighbors < 2 || neighbors > 3) {
                        layers[bg_layer][y][x] = DEAD;
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
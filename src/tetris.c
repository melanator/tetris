#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "tetris.h"

/*
    Shape is a array of bits stored in 2 bits.
    Representation for square
    0000        0000
    0000  = \   0000
    0011  = /   1100
    0011        1100
    Shapes rotated in CCW direction 
*/
bitmatrix shapes[SHAPES * ROTATIONS] = {
    0x0033, 0x0033, 0x0033, 0x0033,     // Square
    0x000f, 0x8888, 0xf000, 0x1111,     // Bar
    0x0027, 0x08c8, 0xe400, 0x1310,     // T-shape
    0x0047, 0x0c88, 0xe200, 0x1130,     // L -shape
    0x0017, 0x088c, 0xc800, 0x3110,     // Reverse L-shape
    0x0063, 0x04c8, 0xc600, 0x1320,     // Z-shape
    0x0036, 0x08c4, 0x6c00, 0x2310      // Reverse Z-shape
};

bool bit_shape(bitmatrix sh, int y, int x){
    return sh & (1 << (y*4 + x));
}

void update_board(tilerow *board, unsigned x, unsigned y){
    // updates board on tile
    board[y] |= (1 << x);
}

void rotate_shape(Shape* shape){
    shape->sh = (++shape->rots % 4) == 3 ? (shape->sh - 3) : ++shape->sh; 
}

int check_fill_row(){
    return -1;
}

void add_to_board(tilerow* board, Shape sh, size_t start_pos){
}

Shape init_shape(){
    Shape result;
    result.sh = &shapes[(rand() % SHAPES) * ROTATIONS];
    result.y = 0;
    result.x = rand() % TETRIS_WIDTH;  // shape to spawn randomly
    result.rots = 0;
    // Check for borders
    return result;
}

bool check_collisions(){
    return false;
}
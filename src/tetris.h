#ifndef TETRIS_H
#define TETRIS_H

#define TETRIS_WIDTH    16      /* 16 tiles stored in 2bit unsigned short */                                      
#define TETRIS_HEIGHT   20
#define ROTATIONS       4
#define SHAPES          7

typedef unsigned short tilerow;
typedef tilerow bitmatrix;

typedef struct Shape {
    bitmatrix* sh;
    size_t rots;    // Rotations
    size_t x, y;    // Location from top row, x for first signifant bit, y for ???
} Shape;

extern bitmatrix shapes[SHAPES * ROTATIONS];

/* Board manipulations*/
void update_board(tilerow *board, unsigned x, unsigned y);
void add_to_board(tilerow* board, Shape sh, size_t start_pos);

/* Game manipulations*/
bool check_collisions();
int check_fill_row();

/* Tiles manipulations*/
bool bit_shape(bitmatrix sh, int y, int x);
void rotate_shape(Shape* shape);
Shape init_shape();

#endif // TETRIS_H
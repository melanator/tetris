#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
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

// Shape start then size to left and bottom
figure_center shape_centers[SHAPES * ROTATIONS] = {
    10, 10, 10, 10,     // Square
    12, 0,  0,  3,      // Bar
    9,  4,  0,  2,      // T-shape
    9,  4,  0,  2,      // L -shape
    9,  4,  0,  2,      // Reverse L-shape
    9,  4,  0,  3,      // Z-shape
    9,  4,  0,  3       // Reverse Z-shape
};

// Index of first significant bit
Size shape_sizes[SHAPES * ROTATIONS] = {
    {2, 2}, {2, 2}, {2, 2}, {2, 2},      // Square
    {4, 1}, {1, 4}, {4, 1}, {1, 4},      // Bar
    {3, 2}, {2, 3}, {3, 2}, {2, 3},      // T-shape
    {3, 2}, {2, 3}, {3, 2}, {2, 3},      // L -shape
    {3, 2}, {2, 3}, {3, 2}, {2, 3},      // Reverse L -shape
    {3, 2}, {2, 3}, {3, 2}, {2, 3},      // Reverse Z -shape
    {3, 2}, {2, 3}, {3, 2}, {2, 3}       // Reverse Z -shape
};

// Fills array with 0's
void init_board(Board board){
    for (int i = 0; i < TETRIS_HEIGHT; i++)
        board[i] = 0x0000;
}

Game init_game(void){
    Game game;
    init_board(game.board);
    game.current_shape = init_shape();
    game.next_shape = init_shape();
    game.points = 0;
    return game;
}

Figure init_figure(){
    Figure result;
    srand(time(NULL));
    size_t index = (rand() % SHAPES) * ROTATIONS;
    result.sh = &shapes[index];
    result.center = &shape_centers[index];
    result.size = &shape_sizes[index];
    return result;
}

Shape init_shape(){
    Shape result;
    result.fig = init_figure();
    result.loc.y = 0;
    srand(time(NULL));
    result.loc.x = rand() % (TETRIS_WIDTH - result.fig.size->x);  // shape to spawn randomly
    //result.loc.x = 0; // shape to spawn randomly
    result.rots = 0;
    init_board(result.shape_board);
    add_to_board(result.shape_board, &result);
    return result;
}

bool bit_shape(bitmatrix sh, int y, int x){
    return sh & (1 << (y*4 + x));
}

void update_board(tilerow *board, unsigned x, unsigned y){
    // updates board on tile
    board[y] |= (1 << x);
}

void rotate_shape(Shape* shape){
    // Moves pointer to next shape, else go back to 3
    if ((shape->rots)++ % 4){
        shape->fig.sh = (shape->fig.sh)++;
        shape->fig.center = (shape->fig.center)++;
        shape->fig.size = (shape->fig.size)++;
    }
    else {
        shape->fig.sh = shape->fig.sh - 3;
        shape->fig.center = shape->fig.center - 3;
        shape->fig.size = shape->fig.size - 3;
    }
}

int check_fill_row(){
    return -1;
}

void add_to_board(tilerow* board, const Shape* sh){
    int start_line = *(sh->fig.center) / 4;    // Row of bitmatrix from which we start
    int start_cell = *(sh->fig.center) % 4;
    int iters = 4 - start_line;                  // How many iterations we will do
    bitmatrix pixels = *(sh->fig.sh);

    static bitmatrix masks[4] = {
        0xf000, 0x0f00, 0x00f0, 0x000f
    };
    
    for (int i = 0; i < iters; i++){
        // get 2 bytes
        bitmatrix shifted = (pixels & masks[start_line]) << start_cell; // apply mask and put first 1 to leftmost position

        // shift to end
        shifted >>= 12 - (start_line++ * 4);

        // shift to needed position
        shifted <<= TETRIS_WIDTH - 4 - sh->loc.x;
        
        // bitwise or
        board[i + sh->loc.y] |= shifted;
        
    }
}

bool check_collisions(){
    return false;
}

void set_next_shapes(Game* game){
    game->current_shape = game->next_shape;
    game->next_shape = init_shape();
}

bool game_tick(Game* game, move_choice move){
    proceed_user_input(move);
    return true;
}

void proceed_user_input(move_choice user_input){
    /* Apply user move */
    switch (user_input){
    case MOVE_LEFT:
        break;
    case MOVE_RIGHT:
        break;
    case MOVE_DOWN:
        break;
    case MOVE_ROTATE:
        break;
    case EXIT:
        break;
    case NOMOVE:
        break;
    }
}
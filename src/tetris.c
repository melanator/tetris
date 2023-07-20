#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include "tetris.h"
#include "utils.h"
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
    0x0017, 0x088c, 0xe800, 0x3110,     // Reverse L-shape
    0x0063, 0x04c8, 0x0063, 0x04c8,     // Z-shape
    0x0036, 0x08c4, 0x0036, 0x08c4      // Reverse Z-shape
};

// Shape start then size to left and bottom
figure_center shape_centers[SHAPES * ROTATIONS] = {
    10, 10, 10, 10,     // Square
    12, 0,  0,  3,      // Bar
    9,  4,  0,  2,      // T-shape
    9,  4,  0,  2,      // L -shape
    9,  4,  0,  2,      // Reverse L-shape
    9,  4,  9,  4,      // Z-shape
    9,  4,  9,  4       // Reverse Z-shape
};

// Index of first significant bit
Size shape_sizes[SHAPES * ROTATIONS] = {
    {2, 2}, {2, 2}, {2, 2}, {2, 2},      // Square
    {4, 1}, {1, 4}, {4, 1}, {1, 4},      // Bar
    {3, 2}, {2, 3}, {3, 2}, {2, 3},      // T-shape
    {3, 2}, {2, 3}, {3, 2}, {2, 3},      // L -shape
    {3, 2}, {2, 3}, {3, 2}, {2, 3},      // Reverse L -shape
    {3, 2}, {2, 3}, {3, 2}, {2, 3},      // Z -shape
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
    
    sleep_ms(15); // Sleep for 15 ms to generate new seed
    game.next_shape = init_shape();

    game.points = 0;
    
    game.level.level = 1;
    game.level.speed = 1000000000; // in ns
    game.level.last_tick = get_timestamp();
    return game;
}

Figure init_figure(){
    Figure result;
    srand(get_timestamp());
    size_t index = (rand() % SHAPES) * ROTATIONS;
    //size_t index = 5 * ROTATIONS;
    result.sh = &shapes[index];
    result.center = &shape_centers[index];
    result.size = &shape_sizes[index];
    return result;
}

Shape init_shape(){
    Shape result;
    srand(get_timestamp());

    result.fig = init_figure();
    result.loc.y = 0;
    result.loc.x = rand() % (TETRIS_WIDTH - result.fig.size->x - 1);  // shape to spawn randomly
    //result.loc.x = (TETRIS_WIDTH - result.fig.size->x); // shape to spawn randomly for debug
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

/* Return true if board changes */
bool fall_shape(Game* game, Shape* shape){
    size_t current_row = shape->loc.y + shape->fig.size->y - 1;

    if (current_row == 19)
        return false;

    if(check_colliision_below(shape->shape_board[current_row], game->board[current_row + 1]))
        return false;   
    
    else {
        shape->loc.y++;
        init_board(shape->shape_board);
        add_to_board(shape->shape_board, shape);
        return true;
    }
}

// Fall till the end
bool free_fall_shape(Game* game, Shape* shape){
    bool is_falling = true;
    while (is_falling)
        is_falling = fall_shape(game, shape);
    return true;
}

bool rotate_shape(Game* game, Shape* shape){
    // Moves pointer to next shape, else go back to 3
    if (++(shape->rots) % 4){
        shape->fig.sh++;
        shape->fig.center++;
        shape->fig.size++;
    }
    else {
        shape->fig.sh = shape->fig.sh - 3;
        shape->fig.center = shape->fig.center - 3;
        shape->fig.size = shape->fig.size - 3;
    }

    init_board(shape->shape_board);
    add_to_board(shape->shape_board, shape);
    return true;
}

int check_fill_row(){
    return -1;
}

/* Adds shape to board by some bitwise manipulations */
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

        // shift to needed position, if tile is in right end, shift right
        if (sh->loc.x < 13)
            shifted <<= TETRIS_WIDTH - 4 - sh->loc.x;
        else
            shifted >>= (sh->loc.x) - 12;
        
        // bitwise or
        board[i + sh->loc.y] |= shifted;
    }
}

void merge_boards(tilerow* lhs, tilerow* rhs){
    for (int i = 0; i < TETRIS_HEIGHT; i++)
        lhs[i] |= rhs[i];
}

bool check_collisions(tilerow* lhs, tilerow* rhs){
    for (int i = 0; i < TETRIS_HEIGHT; i++){
        if(lhs[i] & rhs[i])
            return true;
    }
    return false;
}

void set_next_shapes(Game* game){
    game->current_shape = game->next_shape;
    game->next_shape = init_shape();
}

bool is_time_to_gravity(Level* level){
    size_t gap = get_timestamp() - level->last_tick;
    return (gap > level->speed);
}


bool gravity_tick(Game* game){
    if (is_time_to_gravity(&game->level)) {
        game->level.last_tick = get_timestamp();
        return true;
    }
    return false;
}

bool game_tick(Game* game, move_choice move){
    if (finish_game(game))
        return false;

    bool is_reached_bottom = true;

    if (gravity_tick(game))
        is_reached_bottom = fall_shape(game, &game->current_shape);
    
    // Here some unlogic
    if (!is_reached_bottom)
        finish_tile(game);   

    proceed_user_input(game, move);

    return true;
}

bool finish_tile(Game* game){
    add_to_board(game->board, &game->current_shape);
    set_next_shapes(game);
    return true;
}

bool finish_game(Game* game){
    bool result = true;
    for (int i = TETRIS_HEIGHT - 1; i >= 0; i--){
        if (!result)
            return result;
        result = game->board[i] > 0;
    }
    return result;
}

bool proceed_user_input(Game* game, move_choice user_input){
    /* Apply user move */
    bool refresh_window = false;
    
    switch (user_input){
    case MOVE_LEFT:
        refresh_window = move_left(game, &game->current_shape);
        break;
    case MOVE_RIGHT:
        refresh_window = move_right(game, &game->current_shape);
        break;
    case MOVE_DOWN:
        free_fall_shape(game, &game->current_shape);
        finish_tile(game);
        refresh_window = true;
        break;
    case MOVE_ROTATE:
        refresh_window = rotate_shape(game, &game->current_shape);
        break;
    case EXIT:
        break;
    case NOMOVE:
        break;
    }

    return refresh_window;
}

bool check_colliision_below(tilerow top, tilerow bottom){
    return top & bottom;
}

int count_filled_lines(tilerow* board){
    return 0;
}

void clear_line(tilerow* board){
}


int update_score(int lines){
    return 0;
}

bool move_left(Game* game, Shape* shape){
    if (shape->loc.x == 0)
        return false;
    
    for (int i = 0; i < shape->fig.size->y; i++)
        shape->shape_board[shape->loc.y + i] <<= 1;

    if (!check_collisions(game->board, shape->shape_board)){
        --shape->loc.x;
        return true;
    }
    else{
        for (int i = 0; i < shape->fig.size->y; i++)
            shape->shape_board[shape->loc.y + i] >>= 1;
        return false;
    }
}

bool move_right(Game* game, Shape* shape){
    if ((shape->loc.x + shape->fig.size->x) == TETRIS_WIDTH)
        return false;
    for (int i = 0; i < shape->fig.size->y; i++)
        shape->shape_board[shape->loc.y + i] >>= 1;

    if (!check_collisions(game->board, shape->shape_board)){
        ++shape->loc.x;
        return true;
    }
    else{
        for (int i = 0; i < shape->fig.size->y; i++)
            shape->shape_board[shape->loc.y + i] <<= 1;
        return false;
    }
}
#ifndef TETRIS_H
#define TETRIS_H

#define TETRIS_WIDTH    16      /* 16 tiles stored in 2bit unsigned short */                                      
#define TETRIS_HEIGHT   20
#define ROTATIONS       4
#define SHAPES          7

typedef unsigned short tilerow;
typedef tilerow bitmatrix;
typedef tilerow Board[TETRIS_HEIGHT];
typedef unsigned char figure_center;

typedef enum { MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, MOVE_ROTATE, EXIT, NOMOVE  } move_choice;

typedef struct Location {
    size_t x, y;
} Location;
typedef Location Size;

typedef struct Figure {
    bitmatrix* sh;
    figure_center* center;
    Size* size;
} Figure;

typedef struct Shape {
    Figure fig;
    Location loc;
    size_t rots;    // Rotations
    Board shape_board;
} Shape;

typedef struct Game { 
    Board board; 
    Shape current_shape;
    Shape next_shape;
    unsigned int points;
} Game;

extern bitmatrix shapes[SHAPES * ROTATIONS];

Game init_game(void);

/* Board manipulations*/
void update_board(tilerow *board, unsigned x, unsigned y);
void add_to_board(tilerow* board, const Shape* sh);
void init_board(Board board);

/* Game manipulations*/
bool game_tick(Game* game, move_choice move);
bool check_collisions();
int check_fill_row();
void proceed_user_input(move_choice user_input);
void set_next_shapes(Game* game);

/* Tiles manipulations*/
bool bit_shape(bitmatrix sh, int y, int x);
void rotate_shape(Shape* shape);
Shape init_shape();
Figure init_figure();

#endif // TETRIS_H
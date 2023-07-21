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

typedef struct Level {
    size_t level, speed;
    size_t last_tick;
} Level;

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
    Level level;
    unsigned int points;
} Game;

Game init_game(void);

/* Board manipulations*/
void update_board(tilerow *board, unsigned x, unsigned y);
void add_to_board(tilerow* board, const Shape* sh);
void init_board(Board board);
void merge_boards(tilerow* lhs, tilerow* rhs);
bool check_collisions(tilerow* lhs, tilerow* rhs);

/* Collisions checks */
bool check_colliision_below(tilerow top, tilerow bottom);

/* Game manipulations*/
bool game_tick(Game* game, move_choice move);
int check_fill_row();
void set_next_shapes(Game* game);
bool gravity_tick(Game* game);
bool is_time_to_gravity(Level* level);
bool finish_game(Game* game);
int count_filled_lines(tilerow* board);
void clear_line(Board board, size_t position);

/* Tiles manipulations*/
bool bit_shape(bitmatrix sh, int y, int x);
Shape init_shape();
Figure init_figure();

/* Commands */
bool finish_tile(Game* game);
bool proceed_user_input(Game* game, move_choice user_input);
bool rotate_shape(Game* game, Shape* shape);
bool move_left(Game* game, Shape* shape);
bool move_right(Game* game, Shape* shape);
bool fall_shape(Game* game, Shape* shape);
bool free_fall_shape(Game* game, Shape* shape);

/* Score */
int update_score(Game* game, int lines);


#endif // TETRIS_H
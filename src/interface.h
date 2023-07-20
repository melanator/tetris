#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include "tetris.h"

#define PIXELS_PER_COLUMN 2
#define TOTAL_WIDTH     80
#define TILE            ACS_DIAMOND

int start_game(void);
void finish_program();
move_choice read_user_input(int* input);

/* Manipulation with windows */
void init_ncurses(void);
void print_tile(unsigned x, unsigned y);
void print_shape(Shape* shape, WINDOW *win);
void print_next_tile(WINDOW *win, Figure figure);
void print_stats(WINDOW *win);
void print_board(tilerow* board, WINDOW *win);


/* Debug windows */
void print_rulers(WINDOW* win);

#endif
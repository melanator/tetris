#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include "tetris.h"

#define PIXELS_PER_COLUMN 2
#define TOTAL_WIDTH     80
#define TILE            ACS_DIAMOND

int start_game(void);
void init_game(void);
void finish_program();
move_choice read_user_input();

/* Manipulation with windows */
void print_tile(unsigned x, unsigned y);
void print_shape(const bitmatrix sh, int x_pos, int y_pos, WINDOW *win);
void print_next_tile(WINDOW *win, Shape tile);
void print_stats(WINDOW *win);
void print_board(tilerow* board, WINDOW *win);

#endif
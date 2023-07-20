#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "interface.h"
#include "tetris.h"
#include "utils.h"

int main(int argc, char **argv){
    init_ncurses();
    int input;
    struct timespec tw = {0, 100000000};
    struct timespec tr;

    if (start_game() != '\n')
    {
        finish_program();
        return 0; // Not ENTER, exit
    }

    /* Board is a bit-field array of 16*/
    Game game = init_game();

    bool game_status = true;
    move_choice user_input = NOMOVE;

    // Windows for each section
    WINDOW *board_win = newwin(TETRIS_HEIGHT+2, TETRIS_WIDTH*PIXELS_PER_COLUMN+2, 0, 0);
    WINDOW *next_tile_win = newwin(5, 5*PIXELS_PER_COLUMN+1, 0, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);
    WINDOW *sidemenu_win = newwin(10, 20, 5, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);
    WINDOW *debug_win = newwin(10, 50, 10, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);

    timeout(0);     // Blocking on getch

    // int speed; // ms
    // int level;

    while (game_status){

        game_status = game_tick(&game, user_input);

        /* Print board */
        werase(board_win);
        print_board(game.board, board_win);
        print_board(game.current_shape.shape_board, board_win);
        print_next_tile(next_tile_win, game.next_shape.fig);
        print_stats(sidemenu_win);

        #ifdef TETRIS_DEBUG
            werase(debug_win);
            wmove(debug_win, 0, 0);
            wprintw(debug_win, "CS 0x%08x X: %ld Y: %ld R: %ld", *game.current_shape.fig.sh, 
                                                            game.current_shape.loc.x, 
                                                            game.current_shape.loc.y, 
                                                            game.current_shape.rots); 
            wmove(debug_win, 1, 0);
            wprintw(debug_win, "NS 0x%08x X: %ld", *game.next_shape.fig.sh, game.next_shape.loc.x); 
            wmove(debug_win, 2, 0);
            wprintw(debug_win, "Input: %d", input); 
            wrefresh(debug_win);
            print_rulers(board_win);
        #endif

        /* Read user input*/
        user_input = read_user_input(&input);

        //set_next_shapes(&game);
        nanosleep(&tw, &tr);
    }
    finish_program();
    return 0;
}
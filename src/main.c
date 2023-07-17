#include <stdlib.h>
#include <unistd.h>

#include "interface.h"
#include "tetris.h"

int main(int argc, char **argv){

    /* Board is a bit-field array of 16*/
    Board board = { 0x0000 };
    Game game;
    game.board = &board;
    game.current_shape = init_shape();
    game.next_shape = init_shape();

    bool game_status = true;
    move_choice user_input = NOMOVE;

    init_game();

    // Windows for each section
    WINDOW *board_win = newwin(TETRIS_HEIGHT+2, TETRIS_WIDTH*PIXELS_PER_COLUMN+2, 0, 0);
    WINDOW *next_tile_win = newwin(4, 6*PIXELS_PER_COLUMN, 0, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);
    WINDOW *sidemenu_win = newwin(10, 20, 4, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);
    WINDOW *debug_win = newwin(10, 50, 10, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);

    if (start_game() != '\n')
    {
        finish_program();
        return 0; // Not ENTER, exit
    }

    timeout(0);     // Blocking on getch

    Shape current_shape = init_shape();
    Shape next_shape = init_shape();
    
    // int speed; // ms
    // int level;

    while (game_status){

        game_status = game_tick(&game, user_input);

        /* Print board */
        print_board(board, board_win);
        print_shape(*current_shape.sh, game.current_shape.x, game.current_shape.y, board_win);
        print_next_tile(next_tile_win, game.next_shape);
        print_stats(sidemenu_win);

        #ifdef TETRIS_DEBUG
            werase(debug_win);
            wmove(debug_win, 0, 0);
            wprintw(debug_win, "CS 0x%08x X: %ld Y: %ld R: %ld", *current_shape.sh, 
                                                            current_shape.x, 
                                                            current_shape.y, 
                                                            current_shape.rots); 
            wmove(debug_win, 1, 0);
            wprintw(debug_win, "NS 0x%08x X: %ld", *next_shape.sh, next_shape.x); 
            wrefresh(debug_win);
        #endif

        /* Read user input*/
        move_choice user_input = read_user_input();

        sleep(1);
    }
    finish_program();
    return 0;
}

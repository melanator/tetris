#include <stdlib.h>
#include <unistd.h>

#include "interface.h"
#include "tetris.h"

int main(int argc, char **argv){
    /* Board is a bit-field*/
    tilerow board[TETRIS_HEIGHT] = { 0x0000 };

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

    while (1){
        /* Check if row filled when tile grounded*/
        int filled_row = check_fill_row();

        if (!check_collisions()) {
            finish_program();
        }

        while(filled_row != -1){

        }
            /* Delete row */
            /* Print new shape on top  */
            /* update next tile*/
            current_shape = next_shape;
            next_shape = init_shape();
        
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
            finish_program();
        case NOMOVE:
            break;
        }

        /* Move current tile down if needed*/
               
        /* Print board */
        print_board(board, board_win);
        print_shape(*current_shape.sh, current_shape.x, current_shape.y, board_win);
        print_next_tile(next_tile_win, next_shape);
        print_stats(sidemenu_win);
        sleep(1);
    }
    finish_program();
    return 0;
}

#include "interface.h"

int start_game(void){
    char welcome_sprite[] = "  -----   -----   -----   -----   -    ---  \n"
                            "    -     -         -     -  -    -   -     \n"
                            "    -     -----     -     --      -    ---  \n"
                            "    -     -         -     -  -    -       - \n"
                            "    -     -----     -     -   -   -    ---  \n";
    addstr(welcome_sprite);
    return getch();
}

void init_game(void){
    initscr();      // Initilize ncurses
    cbreak();       // Raw terminal mode but with few commands 
    noecho();
    keypad(stdscr, 1);
    curs_set(0);    // Remove cursor
}

void finish_program(){
    printf("Good bye\n");
    endwin();
}

move_choice read_user_input(){
    move_choice result;

    switch (getch())
    {
    case 'a':
        result = MOVE_LEFT;
        break;
    case 'A':
        result = MOVE_LEFT;
        break;
    case KEY_LEFT:
        result = MOVE_LEFT;
        break;
    case 'd':
        result = MOVE_RIGHT;
        break;
    case 'D':
        result = MOVE_RIGHT;
        break;
    case KEY_RIGHT:
        result = MOVE_RIGHT;
        break;
    case 'w':
        result = MOVE_ROTATE;
        break;
    case 'W':
        result = MOVE_ROTATE;
        break;
    case KEY_UP:
        result = MOVE_ROTATE;
        break;
    case 's':
        result = MOVE_DOWN;
        break;
    case 'S':
        result = MOVE_DOWN;
        break;
    case KEY_DOWN:
        result = MOVE_DOWN;
    case '0':
        result = EXIT;
        break;
    default:
        result = NOMOVE;
    }
    fflush(stdin);
    return result;
}

void print_tile(unsigned x, unsigned y){
    // print tile on terminal
}

void print_shape(const bitmatrix sh, int x_pos, int y_pos, WINDOW *win){
    /* bit-wise AND with shifted one*/
    static int y_offset = 1; // y-offset to print tiles
    static int x_offset = 1; // x-offset to print tiles

    for(int i = 4; i >= 0; i--){         // Row iterating
        for (int j = 4; j >= 0; j--){    // Line iterating
            if(sh & (1 << (i*4 + j))){
                wmove(win, y_pos+y_offset+(4-i), x_pos+(PIXELS_PER_COLUMN*(4-j)+x_offset));
                for(int i = 0; i < PIXELS_PER_COLUMN; i++)
                    waddch(win, TILE);
            }
        }
    }
}

void print_next_tile(WINDOW *win, Shape tile){
    werase(win);
    box(win, 0, 0);
    print_shape(*tile.sh, 0, 0, win);
    wrefresh(win);
}

void print_stats(WINDOW *win){
    wmove(win, 0, 0);
    wprintw(win, "LEVEL: %d", 0); 
    wmove(win, 1, 0);
    wprintw(win, "SCORE: %d", 0); 
    wmove(win, 2, 0);
    wprintw(win, "BEST: %d", 0);
    wrefresh(win);
}

void print_board(tilerow* board, WINDOW *win){
    box(win, 0, 0);
    for (int i = 0; i < TETRIS_HEIGHT; i++){
        for (int j = 0; j < TETRIS_WIDTH;  j++){
            if(board[i] & (1 << j)){
                wmove(win, j, i*PIXELS_PER_COLUMN);
                for (int i = 0; i < PIXELS_PER_COLUMN; i++)
                    waddch(win, TILE);
            }
        }
    }
    wrefresh(win);
}
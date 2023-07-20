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

void init_ncurses(void){
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

inline move_choice read_user_input(int* input){
    move_choice result;
    int symbol;

    fflush(stdin);  

    switch (symbol = getch())
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
    *input = symbol;
    return result;
}

void print_shape(Shape* shape, WINDOW *win){
    /* bit-wise AND with shifted one*/
    static int offset = 1; // offset to print tiles

    const bitmatrix sh = *shape->fig.sh;

    for(int i = 15; i >= 0; i--){         // Row iterating
        if(sh & (1 << i)){
            wmove(win, 3 - (i / 4), (3 - (i % 4)) * PIXELS_PER_COLUMN + offset);
            for(int i = 0; i < PIXELS_PER_COLUMN; i++)
                waddch(win, TILE);
        }
    }
}

void print_next_tile(WINDOW *win, Figure figure){
    // Temporary Shape
    Shape next_shape;
    next_shape.loc.x = 0;
    next_shape.loc.y = 0;
    next_shape.fig = figure;

    werase(win);
    box(win, 0, 0);
    print_shape(&next_shape, win);
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

#   ifdef TETRIS_DEBUG
    print_rulers(win);      // Rulers for debugging
#   else
    box(win, 0, 0);
#   endif

    static int offset = 1;   // Offset from borders
    for (int i = 0; i < TETRIS_HEIGHT; i++){
        for (int j = 0; j < TETRIS_WIDTH;  j++){
            if(board[i] & (1 << j)){
                wmove(win, i + offset, ((15 - j) * PIXELS_PER_COLUMN) + offset);
                for (int i = 0; i < PIXELS_PER_COLUMN; i++)
                    waddch(win, TILE);
            }
        }
    }
    wrefresh(win);
}

void print_rulers(WINDOW *win){
    wmove(win, 0, 1);
    wprintw(win, "0 1 2 3 4 5 6 7 8 9 a b c d e f ");
    wmove(win, TETRIS_HEIGHT+1, 1);
    wprintw(win, "0 1 2 3 4 5 6 7 8 9 a b c d e f ");
    for (int i = 0; i < TETRIS_HEIGHT; i++){
        char ch = (i % 10) + 48;
        wmove(win, i+1, 0);
        waddch(win, ch);
        wmove(win, i+1, TETRIS_WIDTH * PIXELS_PER_COLUMN + 1);
        waddch(win, ch);
    }
    wrefresh(win);
}

int finish_window(Game* game, WINDOW* win){
    timeout(-1);
    werase(win);
    wmove(win, 1, 1);
    wprintw(win, "GAME OVER\nYour score: %d\nPress any key to EXIT", game->points);
    wrefresh(win);
    return getch();
}

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

void print_shape(Shape* shape, WINDOW *win){
    /* bit-wise AND with shifted one*/
    static int y_offset = 1; // y-offset to print tiles
    static int x_offset = 1; // x-offset to print tiles

    const bitmatrix sh = *shape->fig.sh;
    int x_pos = shape->loc.x;
    int y_pos = shape->loc.y;

    for(int i = 15; i >= 0; i--){         // Row iterating
        if(sh & (1 << i)){
            wmove(win, 3 - (i / 4), (3 - (i % 4)) * PIXELS_PER_COLUMN + x_offset);
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
    box(win, 0, 0);
    static int offset = 1;   // Offset from borders
    for (int i = 0; i < TETRIS_HEIGHT; i++){
        for (int j = 0; j < TETRIS_WIDTH;  j++){
            if(board[i] & (1 << j)){
                wmove(win, i + offset, (j * PIXELS_PER_COLUMN) + offset);
                for (int i = 0; i < PIXELS_PER_COLUMN; i++)
                    waddch(win, TILE);
            }
        }
    }
    wrefresh(win);
}
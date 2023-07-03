#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

#define TETRIS_WIDTH    16      /* 16 tiles stored in 2bit unsigned short */                                      
#define TETRIS_HEIGHT   20
#define PIXELS_PER_COLUMN 2
#define TOTAL_WIDTH     80
#define TILE            ACS_DIAMOND

typedef unsigned short tilerow;
typedef tilerow Shape;
typedef enum { MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, MOVE_ROTATE, NOMOVE, EXIT } move_choice;

Shape shapes[7] = {
    0x0033,     // Square
    0x000f,     // Bar
    0x0027,     // T-shape
    0x0047,     // L -shape
    0x0017,     // Reverse L-shape
    0x0063,     // Z-shape
    0x0036      // Reverse Z-shape
};

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

void update_board(tilerow *board, unsigned x, unsigned y){
    // updates board on tile
    board[y] |= (1 << x);
}

void print_tile(unsigned x, unsigned y){
    // print tile on terminal
}

void print_shape(const Shape sh, int x_pos, int y_pos, WINDOW *win){
    /* bit-wise AND with shifted one*/
    for(int i = 3; i >= 0; i--){         // Row iterating
        for (int j = 3; j >= 0; j--){    // Line iterating
            if(sh & (1 << (i*4 + j))){
                wmove(win, y_pos+(3-i), x_pos+(PIXELS_PER_COLUMN*(3-j)));
                for(int i = 0; i < PIXELS_PER_COLUMN; i++)
                    waddch(win, TILE);
            }
        }
    }
}

void print_next_tile(WINDOW *win, Shape tile){
    werase(win);
    box(win, 0, 0);
    print_shape(tile, 1, 1, win);
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

int start_game(void){
    char welcome_sprite[] = "  -----   -----   -----   -----   -    ---  \n"
                            "    -     -         -     -  -    -   -     \n"
                            "    -     -----     -     --      -    ---  \n"
                            "    -     -         -     -  -    -       - \n"
                            "    -     -----     -     -   -   -    ---  \n";
    addstr(welcome_sprite);
    return getch();
}

Shape get_next_shape(void) {
    return shapes[rand() % 7];
}

Shape rotate_shape(Shape shape){
    return shape;
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

int check_fill_row(){
    return -1;
}

void finish_program(){
    printf("Good bye\n");
    endwin();
}

int main(int argc, char **argv){
    /* Board is a bit-field*/
    tilerow board[TETRIS_HEIGHT] = { 0x0000 };

    initscr();      // Initilize ncurses
    cbreak();       // Raw terminal mode but with few commands 
    noecho();
    keypad(stdscr, 1);
    curs_set(0);    // Remove cursor

    // Windows for each section
    WINDOW *board_win = newwin(TETRIS_HEIGHT+2, TETRIS_WIDTH*PIXELS_PER_COLUMN+2, 0, 0);
    WINDOW *next_tile_win = newwin(6, 5*PIXELS_PER_COLUMN, 0, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);
    WINDOW *sidemenu_win = newwin(10, 20, 6, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);

    if (start_game() != '\n')
    {
        finish_program();
        return 0; // Not ENTER, exit
    }

    timeout(0);     // Blocking on getch

    Shape current_shape = get_next_shape();
    Shape next_shape = get_next_shape();
    
    // int speed; // ms
    // int level;

    while (1){
        /* Check if row filled when tile grounded*/
        int filled_row = check_fill_row();
        while(filled_row != -1){

        }
            /* Delete row */
            /* Print new shape on top  */
            /* update next tile*/
            current_shape = next_shape;
            next_shape = get_next_shape();

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
        print_next_tile(next_tile_win, next_shape);
        print_stats(sidemenu_win);
        sleep(1);
    }
    finish_program();
    return 0;
}

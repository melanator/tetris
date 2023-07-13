#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

#define TETRIS_WIDTH    16      /* 16 tiles stored in 2bit unsigned short */                                      
#define TETRIS_HEIGHT   20
#define PIXELS_PER_COLUMN 2
#define TOTAL_WIDTH     80
#define TILE            ACS_DIAMOND
#define ROTATIONS       4
#define SHAPES          7

typedef unsigned short tilerow;
typedef tilerow Shape;
typedef enum { MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, MOVE_ROTATE, NOMOVE, EXIT } move_choice;

typedef struct CurrentShape {
    Shape sh;
    size_t x, y; // Location from bottom row
} CurrentShape;

/*
    Shape is a array of bits stored in 2 bits.
    Representation for square
    0000        0000
    0000  = \   0000
    0011  = /   1100
    0011        1100
    Shapes rotated in CCW direction 
*/

Shape shapes[SHAPES * ROTATIONS] = {
    0x0033, 0x00cc, 0xcc00, 0x3300,     // Square
    0x000f, 0x8888, 0xf000, 0x1111,     // Bar
    0x0027, 0x08c8, 0xe400, 0x1310,    // T-shape
    0x0047, 0x0c88, 0xe200, 0x1130,  // L -shape
    0x0017, 0x088c, 0xc800, 0x3110,   // Reverse L-shape
    0x0063, 0x04c8, 0xc600, 0x1320,     // Z-shape
    0x0036, 0x08c4, 0x6c00, 0x2310      // Reverse Z-shape
};


bool bit_shape(Shape sh, int y, int x){
    return sh & (1 << (y*4 + x));
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

void update_board(tilerow *board, unsigned x, unsigned y){
    // updates board on tile
    board[y] |= (1 << x);
}


void print_tile(unsigned x, unsigned y){
    // print tile on terminal
}

void print_shape(const Shape sh, int x_pos, int y_pos, WINDOW *win){
    /* bit-wise AND with shifted one*/
    static int y_offset = 1; // y-offset to print tiles
    static int x_offset = 2; // x-offset to print tiles

    for(int i = 1; i >= 0; i--){         // Row iterating
        for (int j = 2; j >= 0; j--){    // Line iterating
            if(sh & (1 << (i*4 + j))){
                wmove(win, y_pos+(y_offset-i), x_pos+(PIXELS_PER_COLUMN*(x_offset-j)));
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
    int index = (rand() % SHAPES) * ROTATIONS;
    return shapes[index];
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

void add_to_board(tilerow* board, Shape sh, size_t start_pos){
}

void finish_program(){
    printf("Good bye\n");
    endwin();
}

CurrentShape init_current_shape(Shape sh){
    CurrentShape result;
    result.sh = sh;
    result.y = 0;
    result.x = rand() % TETRIS_WIDTH;  // shape to spawn randomly
    // Check for borders
    return result;
}

void print_current_shape(const CurrentShape* sh, tilerow* board){

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
    WINDOW *next_tile_win = newwin(4, 4*PIXELS_PER_COLUMN, 0, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);
    WINDOW *sidemenu_win = newwin(10, 20, 4, TETRIS_WIDTH*PIXELS_PER_COLUMN+2);

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
            CurrentShape current_shape = init_current_shape(next_shape);
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
        print_current_shape(&current_shape, board);
        print_next_tile(next_tile_win, next_shape);
        print_stats(sidemenu_win);
        sleep(1);
    }
    finish_program();
    return 0;
}

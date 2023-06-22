#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <limits.h>

#define TETRIS_WIDTH    16      /* 16 tiles stored in 2bit unsigned short */                                      
#define TETRIS_HEIGHT   20
#define TOTAL_WIDTH     80
#define TILE            'X'
#define SIDE_X          37      /* Pixel position for side x */

#define clear() printf("\033[H\033[J")                          /*  Clear all terminal board */
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))            /*  Puts cursor to X, Y      */

static struct termios stored_settings;
typedef struct shape {
    int width, height;
    char *pixels; // binary field
} Shape;
typedef unsigned short tilerow;

typedef enum { MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, MOVE_ROTATE, NOMOVE} move;

Shape shapes[7] = {
    {2, 2,  "**"
            "**"},      // Square
    {4, 1, "****"},     // Bar
    {3, 2,  " * "
            "***"},     // T-shape
    {3, 2,  "*  "
            "***"},     // L -shape
    {3, 2,  "  *"
            "***"},     // Reverse L-shape
    {3, 2,  "** "
            " **"},     // Z-shape
    {3, 2,  " **"
            "** "}      // Reverse Z-shape
};

/* Board is a bit-field*/
tilerow board[TETRIS_HEIGHT] = { 0x0000 };

void set_keypress(void)
{
	struct termios new_settings;

	tcgetattr(0,&stored_settings);

	new_settings = stored_settings;

	/* 
		Отключение канонического режима и вывода на экран 
		и установка буфера ввода размером в 1 байт 
	*/
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_lflag &= (~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;

	tcsetattr(0,TCSANOW,&new_settings);
	return;
}

void reset_keypress(void)
{
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
}

void return_cursor(void){
    /* return cursor to lowest cell */
    gotoxy(0, TETRIS_HEIGHT+3);   // + 3 because of frames
}

void print_board(void){
    gotoxy(0, 0);
    printf("╔══════════════════════════════╗");
    printf("\n");
    for (int i = 0; i < TETRIS_HEIGHT; i++){
        printf("║");
        for (int j = 0; j < TETRIS_WIDTH;  j++){
            if(board[i] & (1 << j))
                printf("%c%c", TILE, TILE);
            else
                printf("  ");
        }
        printf("║");
        printf("\n");
    }
    printf("╚══════════════════════════════╝");

    printf("\n");
}

void print_next_tile(void){
    const unsigned tile_size = 4;
    gotoxy(SIDE_X, 1);
    printf("NEXT TILE:");
    gotoxy(SIDE_X, 2);
    printf("╔══════╗");
    for (int i = 0; i < 3; i++){
        gotoxy(SIDE_X, i+3);
        printf("║      ║");
    }
    gotoxy(SIDE_X, tile_size+2);
    printf("╚══════╝");
}

void update_board(unsigned x, unsigned y){
    // updates board on tile
    board[y] |= (1 << x);
}

void print_tile(unsigned x, unsigned y){
    // print tile on terminal
    gotoxy(x, y);
    printf("%c", TILE);
}

void print_shape(const Shape* sh, int x_pos, int y_pos, void(*func)(unsigned, unsigned)){
    for(int i = 0; i < sh->height; i++){
        for (int j = 0; j < sh->width; j++){
            if(sh->pixels[j+i*sh->width] == '*')
                func(x_pos+j, y_pos+i);
        }
    }
}

void print_stats(void){
    gotoxy(SIDE_X, 10);
    printf("LEVEL: %d", 0); 
    gotoxy(SIDE_X, 11); 
    printf("SCORE: %d", 0); 
    gotoxy(SIDE_X, 12);
    printf("BEST: %d", 0);
}

int start_game(void){
    char welcome_sprite[] = "  -----   -----   -----   -----   -    ---  \n"
                            "    -     -         -     -  -    -   -     \n"
                            "    -     -----     -     --      -    ---  \n"
                            "    -     -         -     -  -    -       - \n"
                            "    -     -----     -     -   -   -    ---  \n";
    printf("%s\n", welcome_sprite);
    printf("PRESS ENTER TO START");
    fflush(NULL);
    return getchar();
}

Shape get_next_shape(void) {
    return shapes[rand() % 7];
}

move read_user_input(){
    move result;

    switch (getchar())
    {
    case 'a':
        result = MOVE_LEFT;
        break;
    case 'A':
        result = MOVE_LEFT;
        break;
    case 'd':
        result = MOVE_RIGHT;
        break;
    case 'D':
        result = MOVE_RIGHT;
        break;
    case 'w':
        result = MOVE_ROTATE;
        break;
    case 'W':
        result = MOVE_ROTATE;
        break;
    case 's':
        result = MOVE_DOWN;
        break;
    case 'S':
        result = MOVE_DOWN;
        break;
    default:
        result = NOMOVE;
    }
    return result;
}

int check_fill_row(){
    return -1;
}

int main(int argc, char **argv){
    set_keypress();

    if (start_game() != '\n')
    {
        reset_keypress();
        printf("Good bye");
        return 0; // Not ENTER, exit
    }

    Shape current_shape = get_next_shape();
    Shape next_shape = get_next_shape();

    char input;
    int speed; // ms
    int level;

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
        move user_input = read_user_input();

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
        case NOMOVE:
            break;
        }
        /* Move current tile down if needed*/
        
        /* Print board */
        sleep(1);
        clear();
        print_shape(&current_shape, 2, 2, &update_board);
        print_board();
        print_next_tile();
        print_shape(&next_shape, SIDE_X+2, 4, &print_tile); 
        print_stats();
        return_cursor();
    }
    reset_keypress();
    return 0;
}

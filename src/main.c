#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#define TETRIS_WIDTH    12                                      
#define TETRIS_HEIGHT   20
#define TOTAL_WIDTH     80
#define TILE            'X'

#define clear() printf("\033[H\033[J")                          /*  Clear all terminal board */
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))            /*  Puts cursor to X, Y      */

static struct termios stored_settings;
typedef struct shape {
    int width, height;
    char *pixels; // binary field
} Shape;

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

char board[TETRIS_HEIGHT][TETRIS_WIDTH] = { 
    /*123456789012*/
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "},
    {"            "}
};

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
    printf("╔════════════════════════╗");
    printf("\n");
    for (int i = 0; i < TETRIS_HEIGHT; i++){
        printf("║");
        for (int j = 0; j < TETRIS_WIDTH;  j++){
            printf("%c%c", board[i][j], board[i][j]);
        }
        printf("║");
        printf("\n");
    }
    printf("╚════════════════════════╝");
    printf("\n");
}

void print_next_tile(void){
    const unsigned tile_size = 4;
    gotoxy(30, 1);
    printf("NEXT TILE:");
    gotoxy(30, 2);
    printf("╔══════╗");
    for (int i = 0; i < 3; i++){
        gotoxy(30, i+3);
        printf("║      ║");
    }
    gotoxy(30, tile_size+2);
    printf("╚══════╝");
}

void update_board(unsigned x, unsigned y){
    // updates board on tile
    if (y < TETRIS_HEIGHT || x < TETRIS_WIDTH)
        board[y][x] = TILE;
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
    gotoxy(30, 10);
    printf("LEVEL: %d", 0); 
    gotoxy(30, 11); 
    printf("SCORE: %d", 0); 
    gotoxy(30, 12);
    printf("BEST: %d", 0);
}

int start_game(void){
    char welcome_sprite[] = "  -----   -----   -----   -----   -    ---  \n"
                            "    -     -         -     -  -    -   -     \n"
                            "    -     -----     -     --      -    ---  \n"
                            "    -     -         -     -  -    -       - \n"
                            "    -     -----     -     -   -   -    ---  \n";
    printf("%s\n", welcome_sprite);
    printf("PRESS ANYKEY TO START");
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
    case 'r':
        result = MOVE_ROTATE;
        break;
    case 'R':
        result = MOVE_ROTATE;
        break;
    default:
        result = NOMOVE;
    }
    return result;
}

int main(int argc, char **argv){
    set_keypress();

    if (start_game() != '\n')
    {
        reset_keypress();
        printf("\n");
        return 0; // Not ENTER, exit
    }

    Shape current_shape = get_next_shape();
    Shape next_shape = get_next_shape();

    char input;
    int speed; // ms

    while (1){
        clear();
        print_shape(&current_shape, 2, 2, &update_board);
        print_board();
        print_next_tile();
        print_shape(&next_shape, 32, 4, &print_tile); 
        print_stats();
        return_cursor();
        move user_input = read_user_input();
        sleep(1);
        current_shape = next_shape;
        next_shape = get_next_shape();
    }
    reset_keypress();
    return 0;
}


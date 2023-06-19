#include <stdio.h>
#include <unistd.h>

#define TETRIS_WIDTH    12                                /*  Tiles 2 pixels long, tetris width 12     */
#define TETRIS_HEIGHT   20
#define TOTAL_WIDTH     80
#define TILE            'X'

#define clear() printf("\033[H\033[J")                      /*  Clear all terminal board */
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))         /*  Puts cursor to X, Y      */

typedef struct shape {
    int width, height;
    char *pixels; // binary field
} Shape;

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

void return_cursor(void){
    /* return cursor to lowest cell */
    gotoxy(0, TETRIS_HEIGHT+3);   // + 3 because of frames
}

void print_board(void){
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

void refresh_screen(){
    print_board();
    print_next_tile();
    print_stats();
    return_cursor();
    fflush(NULL);
}

int start_game(void){
    char input;
    char welcome_sprite[] = "  -----   -----   -----   -----   -    ---  \n"
                            "    -     -         -     -  -    -   -     \n"
                            "    -     -----     -     --      -    ---  \n"
                            "    -     -         -     -  -    -       - \n"
                            "    -     -----     -     -   -   -    ---  \n";
    printf("%s\n", welcome_sprite);
    printf("PRESS ANYKEY TO START");
    getc(stdin);
    fflush(NULL);
    return input;
}

int main(int argc, char **argv){
    start_game();
    clear();
    refresh_screen();
    print_shape(&shapes[3], 2, 2, &update_board);
    sleep(1);
    clear();
    refresh_screen();
    print_shape(&shapes[3], 32, 4, &print_tile);
    return_cursor();
    return 0;
}


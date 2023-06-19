#include <stdio.h>
#include <unistd.h>

#define TETRIS_WIDTH    24                                /*  Tiles 2 pixels long, tetris width 12     */
#define TETRIS_HEIGHT   20
#define TOTAL_WIDTH     80
#define TILE            'X'

#define clear() printf("\033[H\033[J")                      /*  Clear all terminal board */
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))         /*  Puts cursor to X, Y      */

typedef struct shape {
    int width, height;
    char *pixels;
} Shape;

char board[TETRIS_HEIGHT][TETRIS_WIDTH] = { 
    /*123456789012345678901234*/
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    {"                        "},
    
};

void print_board(void){
    printf("╔════════════════════════╗");
    printf("\n");
    for (int i = 0; i < TETRIS_HEIGHT; i++){
        printf("║");
        for (int j = 0; j < TETRIS_WIDTH;  j++){
            printf("%c", board[i][j]);
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
    printf("╔════════╗");
    for (int i = 0; i < 4; i++){
        gotoxy(30, i+3);
        printf("║        ║");
    }
    gotoxy(30, tile_size+3);
    printf("╚════════╝");
}

void update_tile(unsigned x, unsigned y){
    board[y][2*x] = TILE;
    board[y][2*x+1] = TILE;
}

void refresh_screen(){
    print_board();
    print_next_tile();
    gotoxy(30, 10);
    printf("LEVEL: %d", 0);
    gotoxy(30, 11);
    printf("SCORE: %d", 0);
    gotoxy(30, 12);
    printf("BEST: %d", 0);
    gotoxy(0, TETRIS_HEIGHT+3);   // + 3 because of frames
    fflush(NULL);
}

int start_game(void){
    char input;
    char welcome_sprite[] = "  -----   -----   -----   -----     -    ---  \n"
                            "    -     -         -     -  -      -   -     \n"
                            "    -     -----     -     --        -    ---  \n"
                            "    -     -         -     -  -      -       - \n"
                            "    -     -----     -     -   -     -    ---  \n";
    printf("%s", welcome_sprite);
    printf("\nPRESS ENTER TO START");
    while ((input = getchar()) != 'b') {}
    fflush(NULL);
    return input;
}

int main(int argc, char **argv){
    start_game();
    clear();
    refresh_screen();
    update_tile(1,1);
    update_tile(2,2);
    sleep(1);
    clear();
    refresh_screen();
    return 0;
}


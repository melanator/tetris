rls:
	gcc ./src/main.c ./src/tetris.c ./src/interface.c ./src/utils.c -lncurses -Wall -o rls

dbg:
	gcc ./src/main.c ./src/tetris.c ./src/interface.c ./src/utils.c  -lncurses -Wall -DTETRIS_DEBUG -g -o dbg
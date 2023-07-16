rls:
	gcc ./src/main.c ./src/tetris.c ./src/interface.c  -lncurses -Wall -o rls

dbg:
	gcc ./src/main.c ./src/tetris.c ./src/interface.c  -lncurses -DTETRIS_DEBUG -g -o dbg
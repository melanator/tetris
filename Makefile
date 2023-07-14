rls:
	gcc ./src/main.c -lncurses -Wall -o rls

dbg:
	gcc ./src/main.c -lncurses -DTETRIS_DEBUG -g -o dbg
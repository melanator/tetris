rls:
	gcc ./src/main.c -lncurses -Wall -o rls

dbg:
	gcc ./src/main.c -lncurses -g -o dbg
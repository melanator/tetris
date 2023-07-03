rls:
	gcc ./src/main.c -lncurses -Wall -o main_rls

dbg:
	gcc ./src/main.c -lncurses -g -o main_dbg
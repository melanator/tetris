rls:
	gcc ./src/main.c -lncurses -o main_rls

dbg:
	gcc ./src/main.c -lncurses -g -o main_dbg
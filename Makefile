play: board.c board.h logic.c logic.h play.c pos.c pos.h players.c players.h
	clang -g -Wall -o play board.c logic.c play.c \
	pos.c players.c
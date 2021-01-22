CC=clang
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

parse : specific.h parse.c ../ADTs/General/general.h ../ADTs/General/general.c
	$(CC) parse.c ../ADTs/General/general.c -o parse $(SANITIZE)

interp : specific.h interp.c ../ADTs/General/general.h ../ADTs/General/general.c
	$(CC) interp.c ../ADTs/General/general.c -o interp $(SANITIZE)

test : test.c
	$(CC) test.c -o test $(SANITIZE)

CFLAGS = -Wall -Wextra -pedantic -ansi -std=c90 -fsanitize=address -fsanitize=undefined -g3 -Wfloat-equal
CFLAGS2 = -Wall -Wextra -pedantic -ansi -std=c90 -O3 -g3 -Wfloat-equal
CC = clang
CC2 = gcc
LBLIBS = -lm

rec: recursion.c

	$(CC) recursion.c -o rec $(CFLAGS) $(LDLLIBS)

sc: ex_6_4_splchk.c

	$(CC) ex_6_4_splchk.c -o sc $(CFLAGS) $(LDLLIBS)

bool: testboolarr.c boolarr.h specific.h general.h

	$(CC) testboolarr.c -o sc $(CFLAGS) $(LDLLIBS)

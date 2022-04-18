TEST_SRC = \
	  builtins.c \
	  test-equivalent.c

MAIN_SRC = \
	  builtins.c \
	  main.c

TEST_OBJ = ${TEST_SRC:.c=.o}

MAIN_OBJ = ${MAIN_SRC:.c=.o}

#INCS = -I. -I/usr/include

CFLAGS = -g -Wall ${INCS}

all: main

main: ${MAIN_OBJ}
	cc -o $@ ${MAIN_OBJ} ${LIBS}

test: ${TEST_OBJ}
	cc -o $@ ${TEST_OBJ} ${LIBS}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f main test ${MAIN_OBJ} ${TEST_OBJ}

OBJS=testlib.o

EXT=so

CFLAGS= -shared -fPIC

../../bin/testlib.${EXT}: ${OBJS}
	${CC} ${CFLAGS} $< -o $@

all: ../../bin/testlib.${EXT}

.PHONY: all

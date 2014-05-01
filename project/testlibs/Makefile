OBJS=testlib.o

EXT=so

M32=

CFLAGS= -shared -fPIC ${M32} -pthread

../../bin/testlib.${EXT}: ${OBJS}
	${CC} ${CFLAGS} $< -o $@

all: ../../bin/testlib.${EXT}

.PHONY: all

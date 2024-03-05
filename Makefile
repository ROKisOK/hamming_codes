# Makefile code taken from professor Darell Long's piazza post 
EXEC1 = encode
EXEC2 = decode
EXEC3 = error
EXEC4 = entropy

SOURCES1 = encode.c bv.c bm.c hamming.c
SOURCES2 = decode.c bv.c bm.c hamming.c
SOURCES3 = error.c
SOURCES4 = entropy.c

OBJECTS1 = $(SOURCES1:%.c=%.o)
OBJECTS2 = $(SOURCES2:%.c=%.o)
OBJECTS3 = $(SOURCES3:%.c=%.o)
OBJECTS4 = $(SOURCES4:%.c=%.o)

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Ofast
LFLAGS = -lm

.PHONY: all clean format scan-build

all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)

$(EXEC1): $(OBJECTS1)
	$(CC) -g -o $@ $^ $(LFLAGS)

$(EXEC2): $(OBJECTS2)
	$(CC) -g -o $@ $^ $(LFLAGS)

$(EXEC3): $(OBJECTS3)
	$(CC) -g -o $@ $^ $(LFLAGS)

$(EXEC4): $(OBJECTS4)
	$(CC) -g -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -rf $(EXEC) $(OBJECTS)

scan-build: clean
	scan-build make

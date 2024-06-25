CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

EXECUTABLE = ipkcpc
OBJS = ipkcpc.c

all: $(EXECUTABLE)
$(EXECUTABLE): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm ipkcpc
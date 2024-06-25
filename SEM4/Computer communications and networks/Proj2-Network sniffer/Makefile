CC = gcc
CFLAGS = -lpcap

EXECUTABLE = ipk-sniffer
OBJS = ipk-sniffer.c

all: $(EXECUTABLE)
$(EXECUTABLE): $(OBJS)
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm $(EXECUTABLE)
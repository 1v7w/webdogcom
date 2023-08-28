CC = gcc
TARGET = webdogcom

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)


.PHONY: all clean
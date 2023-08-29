CC = gcc
TARGET = webdogcom

INCLUDE_DIR = -Iinclude/
SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

ifeq ($(debug), y)
	CFLAGS += -DDEBUG -g
endif

CFLAGS += $(INCLUDE_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o ../$(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@



all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)


.PHONY: all clean
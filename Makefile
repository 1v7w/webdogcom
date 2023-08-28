CC = gcc
TARGET = webdogcom

ifeq ($(debug), y)
	CFLAGS += -DDEBUG -g
endif

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET)

all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)


.PHONY: all clean
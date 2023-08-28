CC = gcc
TARGET = webdogcom
CONFIG = webdogcom.conf

ifeq ($(debug), y)
	CFLAGS += -DDEBUG -g
endif

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

$(TARGET): $(OBJS) $(CONFIG)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET)

$(CONFIG):
	cp webdogcom_sample.conf $(CONFIG)

all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)


.PHONY: all clean
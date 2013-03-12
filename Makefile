

INCLUDE = -Iinclude

SRC = src/main.c \
      src/gameworld.c

OBJECTS = src/main.o \
          src/gameworld.o

LIBS = 

TARGET = urd-world

CC = gcc
CPPFLAGS = 
CFLAGS = -std=c99 -DDEBUG -g
LDFLAGS = -ltelnet

.PHONY: default all clean

all: default

%.o: %.c 
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) $(LIBS) -c $< -o $@

default: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

clean: 
	-rm -f src/*.o
	-rm -f $(TARGET)

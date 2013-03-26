

INCLUDE = -Iinclude

OBJECTS = src/main.o \
          src/gameworld.o \
          src/actions.o \
          src/beginning.o \
          src/rankings.o \
          src/commands.o

LIBS = 

TARGET = urd-world

CC = gcc
CPPFLAGS = 
CFLAGS = -std=c11 -Wall -Wextra -pedantic 
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

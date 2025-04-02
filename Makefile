# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Iinclude/glad/include `pkg-config --cflags glfw3`
LDFLAGS = `pkg-config --static --libs glfw3` -lGL -lm

SOURCES = include/glad/src/glad.c $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
LNAME = libmidnight.a

$(LNAME): $(OBJECTS)
	@echo "OBJECTS: $(OBJECTS)" # Add this line
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(LNAME)

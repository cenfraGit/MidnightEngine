CC = gcc
CFLAGS = -Wall -I../../include -I../../include/glad/include
LDFLAGS = -L../../ -lmidnight -lglfw3 -lGL -lm

SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = main

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

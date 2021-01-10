SOURCE = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(SOURCE:.c=.o)
BIN = bin

all: bmp-generator

bmp-generator: $(OBJECTS)
	gcc -o $@ $^ -lm

obj/%.o : src/%.c src/main.h
	gcc -c -o $@ $< 

src/main.h: $(HEADERS)
	touch src/main.h

clean:
	rm obj/*.o bmp-generator -f
	

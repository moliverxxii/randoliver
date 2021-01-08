

all: bmp-generator

bmp-generator: main.o util.o fileUtil.o imgUtil.o filter.o
	gcc -o $@ $^ -lm

main.o: main.c main.h
	gcc -c main.c 

util.o: util.c main.h
	gcc -c util.c

fileUtil.o: fileUtil.c main.h
	gcc -c fileUtil.c

imgUtil.o: imgUtil.c main.h
	gcc -c -lm imgUtil.c

filter.o: filter.c main.h
	gcc -c filter.c

main.h: util.h fileUtil.h imgUtil.h imgUtil.h filter.h
	touch main.h

clean:
	rm *.o bmp-generator -f
	

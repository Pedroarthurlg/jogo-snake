all: main.c snake.c
	gcc -o main main.c snake.c -lraylib -lm -lpthread -ldl -lrt -lX11
snake.o: snake.c snake.h
	gcc -c snake.c
main.o: main.c snake.h
	gcc -c main.c
clean:
	rm -f *.o main
run: all
	./main
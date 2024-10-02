CC = gcc
BIN = bin/main.o bin/cpu.o
OUT = bin/main
FLAGS = -c -Wall 

all: $(BIN)
	$(CC) $(BIN) -o $(OUT)

bin/main.o: src/main.c
	$(CC) $(FLAGS) src/main.c -o bin/main.o

bin/cpu.o: src/cpu.o
	$(CC) $(FLAGS) src/cpu.c -o bin/cpu.o

clean:
	rm -rf $(BIN)

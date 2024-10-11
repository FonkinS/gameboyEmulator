CC = gcc
BIN = bin/main.o bin/cpu.o bin/ppu.o
OUT = bin/main
FLAGS = -c -Wall 

all: $(BIN)
	$(CC) $(BIN) -L /usr/local/lib -lglfw -framework OpenGL -o $(OUT)

bin/main.o: src/main.c
	$(CC) $(FLAGS) src/main.c -o bin/main.o

bin/cpu.o: src/cpu.c
	$(CC) $(FLAGS) src/cpu.c -o bin/cpu.o

bin/ppu.o: src/ppu.c
	$(CC) $(FLAGS) -w src/ppu.c -o bin/ppu.o 

clean:
	rm -rf $(BIN)

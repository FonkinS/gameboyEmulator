CC = gcc
BIN = bin/main.o bin/cpu.o bin/ppu.o bin/lcd.o bin/gameboy.o bin/interrupts.o bin/joypad.o bin/bus.o bin/timer.o bin/mbc.o bin/mbc0.o bin/mbc1.o bin/apu.o bin/mbc3.o
OUT = bin/main
FLAGS = -c -Wall -fcommon -O3

all: $(BIN) bin/renderDesktop.o bin/inputDesktop.o
	$(CC) $(BIN) bin/renderDesktop.o bin/inputDesktop.o -lSDL2 -L /usr/local/lib -lglfw -framework OpenGL -o $(OUT) -O3

raspi: $(BIN)  bin/renderPi.o bin/inputPi.o
	$(CC) $(BIN) bin/renderPi.o bin/inputPi.o -lSDL2 -lwiringPi -lm -ldl -lpthread -latomic -o $(OUT)

bin/main.o: src/main.c
	$(CC) $(FLAGS) src/main.c -o bin/main.o

bin/cpu.o: src/core/cpu.c
	$(CC) $(FLAGS) src/core/cpu.c -o bin/cpu.o

bin/ppu.o: src/core/ppu.c
	$(CC) $(FLAGS) src/core/ppu.c -o bin/ppu.o 

bin/lcd.o: src/core/lcd.c
	$(CC) $(FLAGS) src/core/lcd.c -o bin/lcd.o

bin/gameboy.o: src/core/gameboy.c
	$(CC) $(FLAGS) src/core/gameboy.c -o bin/gameboy.o

bin/interrupts.o: src/core/interrupts.c
	$(CC) $(FLAGS) src/core/interrupts.c -o bin/interrupts.o 

bin/joypad.o: src/core/joypad.c
	$(CC) $(FLAGS) src/core/joypad.c -o bin/joypad.o

bin/bus.o: src/core/bus.c
	$(CC) $(FLAGS) src/core/bus.c -o bin/bus.o

bin/timer.o: src/core/timer.c
	$(CC) $(FLAGS) src/core/timer.c -o bin/timer.o

bin/mbc.o: src/core/mbc.c
	$(CC) $(FLAGS) src/core/mbc.c -o bin/mbc.o

bin/mbc0.o: src/core/mbc/mbc0.c
	$(CC) $(FLAGS) src/core/mbc/mbc0.c -o bin/mbc0.o

bin/mbc1.o: src/core/mbc/mbc1.c
	$(CC) $(FLAGS) src/core/mbc/mbc1.c -o bin/mbc1.o

bin/apu.o: src/core/apu.c
	$(CC) $(FLAGS) src/core/apu.c -o bin/apu.o

bin/mbc3.o: src/core/mbc/mbc3.c
	$(CC) $(FLAGS) src/core/mbc/mbc3.c -o bin/mbc3.o

bin/renderDesktop.o: src/Desktop/renderDesktop.c
	$(CC) $(FLAGS) src/Desktop/renderDesktop.c -o bin/renderDesktop.o

bin/renderPi.o: src/Pi/renderPi.c
	$(CC) $(FLAGS) src/Pi/renderPi.c -o bin/renderPi.o

bin/inputDesktop.o: src/Desktop/inputDesktop.c
	$(CC) $(FLAGS) src/Desktop/inputDesktop.c -o bin/inputDesktop.o

bin/inputPi.o: src/Pi/inputPi.c
	$(CC) $(FLAGS) src/Pi/inputPi.c -o bin/inputPi.o


clean:
	rm -rf $(BIN) bin/renderDesktop.o bin/renderPi.o bin/inputDesktop.o bin/inputPi.o

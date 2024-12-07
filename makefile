CC = gcc
BIN = bin/main.o bin/cpu.o bin/ppu.o bin/lcd.o bin/gameboy.o bin/interrupts.o bin/joypad.o bin/bus.o bin/timer.o bin/mbc.o bin/mbc0.o bin/mbc1.o bin/apu.o bin/mbc3.o
OUT = bin/main
FLAGS = -c -Wall -fcommon

all: $(BIN)
	$(CC) $(BIN) -lSDL2 -o $(OUT)

raspi: $(BIN)
	$(CC) $(BIN) -lSDL2 -lwiringPi -lm -ldl -lpthread -latomic -o $(OUT)

bin/main.o: src/main.c
	$(CC) $(FLAGS) src/main.c -o bin/main.o

bin/cpu.o: src/cpu.c
	$(CC) $(FLAGS) src/cpu.c -o bin/cpu.o

bin/ppu.o: src/ppu.c
	$(CC) $(FLAGS) src/ppu.c -o bin/ppu.o 

bin/lcd.o: src/lcd.c
	$(CC) $(FLAGS) src/lcd.c -o bin/lcd.o

bin/gameboy.o: src/gameboy.c
	$(CC) $(FLAGS) src/gameboy.c -o bin/gameboy.o

bin/interrupts.o: src/interrupts.c
	$(CC) $(FLAGS) src/interrupts.c -o bin/interrupts.o 

bin/joypad.o: src/joypad.c
	$(CC) $(FLAGS) src/joypad.c -o bin/joypad.o

bin/bus.o: src/bus.c
	$(CC) $(FLAGS) src/bus.c -o bin/bus.o

bin/timer.o: src/timer.c
	$(CC) $(FLAGS) src/timer.c -o bin/timer.o

bin/mbc.o: src/mbc.c
	$(CC) $(FLAGS) src/mbc.c -o bin/mbc.o

bin/mbc0.o: src/mbc/mbc0.c
	$(CC) $(FLAGS) src/mbc/mbc0.c -o bin/mbc0.o

bin/mbc1.o: src/mbc/mbc1.c
	$(CC) $(FLAGS) src/mbc/mbc1.c -o bin/mbc1.o

bin/apu.o: src/apu.c
	$(CC) $(FLAGS) src/apu.c -o bin/apu.o

bin/mbc3.o: src/mbc/mbc3.c
	$(CC) $(FLAGS) src/mbc/mbc3.c -o bin/mbc3.o

clean:
	rm -rf $(BIN)

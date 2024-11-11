#ifndef APU_H
#define APU_H
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"

void APUInit();

uint8_t APURead(uint16_t index);
void APUWrite(uint16_t index, uint8_t value);

void APUTick(int cycles);

void APUKill();

void triggerChannel(int n);
void channelTick(int cycles, int *timer, uint8_t* dutyIndex, int period);
// TODO SUper Mario Land running slowlu?
// TODO SUper Mario Land OAM Glitches
// TODO Super Mario Land Pause Screen Glitch
// TODO Save Data (Ext RAM)
// TODO POkemon Blue Main Menu issue
// TODO Pokemon Blue Music sounds shit
// TODO CH3 and CH4

enum APUIO {
    NR10 = 0xff10, // Channel 1 Sweep
    NR11 = 0xff11, // Channel 1 Length
    NR12 = 0xff12, // Channel 1 Volume
    NR13 = 0xff13, // Channel 1 Period Low
    NR14 = 0xff14, // Channel 1 Period High
    NR21 = 0xff16, // Channel 2 Length
    NR22 = 0xff17, // Channel 2 Volume
    NR23 = 0xff18, // Channel 2 Period Low
    NR24 = 0xff19, // Channel 2 Period High
    NR30 = 0xff1a, // Channel 3 DAC
    NR31 = 0xff1b, // Channel 3 Length
    NR32 = 0xff1c, // Channel 3 Volume
    NR33 = 0xff1d, // Channel 3 Period Low
    NR34 = 0xff1e, // Channel 3 Period High
    NR41 = 0xff20, // Channel 4 Length
    NR42 = 0xff21, // Channel 4 Volume
    NR43 = 0xff22, // Channel 4 Frequency / Randomness
    NR44 = 0xff23, // Channel 4 Control
    NR50 = 0xff24, // Master Volume
    NR51 = 0xff25, // Sound Panning 
    NR52 = 0xff26, // Master Control
};

bool APUEnabled;
enum SOUND_PANNING {CENTER, LEFT, RIGHT};
enum SOUND_PANNING CH1Pan;
enum SOUND_PANNING CH2Pan;
enum SOUND_PANNING CH3Pan;
enum SOUND_PANNING CH4Pan;

uint8_t leftVolume;
uint8_t rightVolume;

// Channel 1
bool CH1Enabled;
int CH1Timer;
uint8_t CH1Pace;
bool CH1Direction;
uint8_t CH1Step;
uint8_t CH1WaveDuty;
uint8_t CH1InitLength;
uint8_t CH1Length;
uint8_t CH1InitVolume;
uint8_t CH1Volume;
bool CH1EnvDir;
uint8_t CH1SweepPace;
uint16_t CH1Period;
bool CH1LengthEnable;


// Channel 2
bool CH2Enabled;
int CH2Timer;
uint8_t CH2WaveDuty;
uint8_t CH2InitLength;
uint8_t CH2Length;
uint8_t CH2InitVolume;
uint8_t CH2Volume;
bool CH2EnvDir;
uint8_t CH2SweepPace;
uint16_t CH2Period;
bool CH2LengthEnable;


bool CH3Enabled;
bool CH4Enabled;







#endif // AUDIO_H

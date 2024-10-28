#include "lcd.h"


void LCDTick(int cycle_length) {
    if ((io_read(rLCDC) & 128)) { // Check if PPU and LCD Enabled
        int prev_ppu_mode = ppu_mode;
        dot += cycle_length;
        if (ppu_mode != 1) { // Increase PPU_Mode
            if (dot >= mode_3_len + 80) {
                ppu_mode = 0;
            } else if  (dot >= 80) {
                ppu_mode = 3;
                mode_3_len = (io_read(rSCX) & 7) ? (((io_read(rSCX) & 7) > 4) ? 178 : 176) : 172;
            }
        }
        // New Line
        if (dot >= 456) {
            dot %= 456;
            io_write(rLY, io_read(rLY)+1);
            if (io_read(rLY) >= 154) { // End oF VBlank
                ppu_mode = 2;
                io_write(rLY, 0);
            } else if (io_read(rLY) >= 144) { // VBlank!
                ppu_mode = 1;
            } else {                // Back to Start
                ppu_mode = 2;   
            }
        }
        if (ppu_mode != prev_ppu_mode) { // If change in Mode over this cycle
            if (ppu_mode == 2) {
                if (io_read(rSTAT) & 32) request_interrupt(LCDSTAT); // OAM Scan
            } else if (ppu_mode == 0) {
                if (io_read(rSTAT) & 8) request_interrupt(LCDSTAT); // Horizontal Blank
            } else if (ppu_mode == 1) {
                if (io_read(rSTAT) & 16) request_interrupt(LCDSTAT); // VBlank
                request_interrupt(VBLANK);                      // Actual VBlank Interrupt
            }
        }
        // Set LY==LYC
        io_write(rSTAT,(io_read(rSTAT) & ~4) | ((io_read(rLY) == io_read(rLYC)) << 2));
        if (io_read(rSTAT) & 4) {
            if (io_read(rLY) == io_read(rLYC) && (io_read(rSTAT) & 64)) {
                request_interrupt(LCDSTAT); // Call LYC Interrupt if allowed
            }
        }
        // Set PPU mode in LCDSTAT
        io_write(rSTAT, (io_read(rSTAT) & ~3) | ppu_mode);
        //printf("LY:%i, LYC:%i\n", io_read(rLY), io_read(rLYC));
    } else {
        //printf("LCD Turned off\n");
    }
}

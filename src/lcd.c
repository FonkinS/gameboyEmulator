#include "lcd.h"


void LCDTick(int cycle_length) {
    if ((data[0xff40] & 128)) { // Check if PPU and LCD Enabled
        int prev_ppu_mode = ppu_mode;
        dot += cycle_length;
        if (ppu_mode != 1) { // Increase PPU_Mode
            if (dot >= mode_3_len + 80) {
                ppu_mode = 0;
            } else if  (dot >= 80) {
                ppu_mode = 3;
                mode_3_len = (data[0xff43] & 7) ? (((data[0xff43] & 7) > 4) ? 178 : 176) : 172;
            }
        }
        // New Line
        if (dot >= 456) {
            dot %= 456;
            data[0xff44]++;
            if (data[0xff44] >= 154) { // End oF VBlank
                ppu_mode = 2;
                data[0xff44] = 0;
            } else if (data[0xff44] >= 144) { // VBlank!
                ppu_mode = 1;
            } else {                // Back to Start
                ppu_mode = 2;   
            }
        }
        if (ppu_mode != prev_ppu_mode) { // If change in Mode over this cycle
            if (ppu_mode == 2) {
                if (data[0xff41] & 32) data[0xff0f] |= 2; // OAM Scan
            } else if (ppu_mode == 0) {
                if (data[0xff41] & 8) data[0xff0f] |= 2; // Horizontal Blank
            } else if (ppu_mode == 1) {
                if (data[0xff41] & 16) data[0xff0f] |= 2; // VBlank
                data[0xff0f] |= 1;                      // Actual VBlank Interrupt
            }
        }
        // Set LY==LYC
        data[0xff41] = (data[0xff41] & ~4) | ((data[0xff44] == data[0xff45]) << 2);
        if (data[0xff41] & 4) {
            if (data[0xff44] == data[0xff45] && (data[0xff41] & 64)) {
                data[0xff0f] |= 2; // Call LYC Interrupt if allowed
            }
        }
        // Set PPU mode in LCDSTAT
        data[0xff41] = (data[0xff41] & ~3) | ppu_mode;
    }
}

/*
 * Copyright (C) 2025 Aaron Kainu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/



#include "ppu.h"

#include <stdint.h>
#include "bus.h"
#include "lcd.h"
#include "../render.h"


uint8_t faux_bg_screen[160*144]; // For pixel priority calculations

/* Draws a gameboy scanline to the screen[] array in the render scripts 
 * The Gameboy VRAM contains a list of tiles, and a tilemap with indices
 * to the tileset. This function finds the correct tiles on the tilemap 
 * adjusted for scrolling, then renders the pixels in that tile */
void drawScanline(int scanline) {
    if (BGWinEnable) {
        // Background rendering
        uint8_t y = (scanline + SCY) % 256; // the actual y position of the scanline (adjusted for scroll)
        uint8_t tiley = y / 8; // The actual tiled version of the y position
        uint8_t ty = (scanline + SCY) % 8; // The y position inside that tile
        
        // For each of the 20 tiles on a row in the tilemap (160 / 8)
        for (int tilex = SCX / 8; tilex < 21+SCX/8; tilex++) {
            // Get the location of the tile in the tileset in memory
            uint8_t index = BusRead(BGTileMap + tilex%32 + tiley*32);
            int tile = BGWinTileData + (int)(BGWinTileData == 0x8000 ? (uint8_t)index : (int8_t)index) * 16;

            // Retrieve the pixel data for the tile
            uint8_t first = BusRead(tile + ty*2);
            uint8_t second = BusRead(tile + ty*2+1);

            // For each of the 8 pixels in the scanline in the tile, draw it if not off-screen
            for (int tx = 0; tx < 8; tx++) {
                uint8_t x = (tilex * 8 + tx - SCX) % 256;
                if (x < 0 || x >= 160) continue;
                screen[scanline*160+x] = BGP[((first >> (7-tx)) & 1) + (((second >> (7-tx)) & 1) << 1)];
                faux_bg_screen[scanline*160+x] = ((first >> (7-tx)) & 1) + (((second >> (7-tx)) & 1) << 1);
            }
        }

        // Window rendering. Check if window is not hidden
        if (WinEnable && scanline >= WY) {
            uint8_t ty = (scanline - WY) % 8; // Y position inside the tile
            uint8_t tiley = (scanline - WY) / 8; // The actual tiled version of the y position
            
            // For each of the 20 tiles on a row in the tilemap (160/8)
            for (int tilex = WX / 8; tilex < 21; tilex++) {
                // Get the location of the tile in the tileset in memory
                uint8_t index = BusRead(WinTileMap + (tilex - WX / 8) + tiley*32);
                int tile = BGWinTileData + (int)(BGWinTileData == 0x8000 ? (uint8_t)index : (int8_t)index) * 16;

                // Retrieve the pixel data for the tile
                uint8_t first = BusRead(tile + ty*2);
                uint8_t second = BusRead(tile + ty*2+1);

                // For each of the 8 pixels in the scanline in the tile, draw it if not off-screen
                for (int tx = 0; tx < 8; tx++) {
                    uint8_t wx = (tilex - WX / 8) * 8 + tx;
                    if (wx < WX-7 || wx >= 160) continue;
                    screen[scanline*160+wx] = BGP[((first >> (7-tx)) & 1) + (((second >> (7-tx)) & 1) << 1)];
                    faux_bg_screen[scanline*160+wx] = ((first >> (7-tx)) & 1) + (((second >> (7-tx)) & 1) << 1);
                }
            }
        }
    }     

    if (OBJEnable) {
        // Object Rendering
        // Loop through every object in memory. Stored in a 4 byte format: [Y, X, Tile, Flags]
        for (int o = 0xfe00; o < 0xfea0; o+=4) {
            // Get X and Y
            int16_t y = BusRead(o)-16;
            int16_t x = BusRead(o+1);

            // If the Object is on screen
            if (scanline-y < OBJSize && scanline >= y && x > 0 && x < 176) {
                // Get tile and flags
                uint8_t tile = BusRead(o+2) & (OBJSize == 16 ? 0xfe : 0xff);
                uint8_t flags = BusRead(o+3);

                // Calculate the real y position on screen
                uint8_t act_y = scanline-y;
                if (flags & 0x40) act_y = (OBJSize - 1) - act_y; // y flip

                // Get the tile data
                uint8_t first = BusRead(0x8000 + tile*16 + act_y * 2);
                uint8_t second = BusRead(0x8000 + tile*16 + act_y * 2 + 1);
                
                // Get which palette to use
                uint8_t *obp = (flags & 0x10) ? OBP1 : OBP0;

                // Go through each pixel in tile
                for (int ox = 0; ox < 8; ox++) {
                    // Don't draw if offscreen
                    if (ox+x < 8) continue;
                    if (ox+x >= 168) break;
                    if (flags & 0x80 && faux_bg_screen[scanline*160+ox+x-8] > 0) continue; // BG Priority

                    // Calculate the pixel color
                    uint8_t color;
                    if (flags & 0x20) color = (((first >> ((ox%8))) & 1)) + (((second >> ((ox%8))) & 1) << 1); // X Flip
                    else color = (((first >> (7-(ox%8))) & 1)) + (((second >> (7-(ox%8))) & 1) << 1);
                    if (color == 0) continue;

                    // Draw Color
                    screen[scanline * 160 + ox+x-8] = obp[color-1];
                }
            }
        }
    }
}

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

#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "interrupts.h"
#include "lcd.h"

int CPUInit();

//void connect_bus(Bus *b);
int execute_op();

extern uint16_t PC; // program counter
extern uint16_t SP; // stack pointer

extern uint8_t a;
extern uint8_t b;
extern uint8_t c;
extern uint8_t d;
extern uint8_t e;
extern uint8_t f;
extern uint8_t h;
extern uint8_t l;


extern int serial_interrupt;
extern unsigned long long cycles;

enum {NOHALT, HALTIME, HALTNOIMENOINT, HALTBUG, HALTBUGEI, HALTBUGRST};
extern int halt;
 


enum flags {
    z = 7,
    n = 6,
    hy = 5,
    cy = 4
};

//int set_reg(char r, uint8_t val);   // For the Unit Tester
//uint8_t get_reg(char r);            // For the Unit Tester

uint16_t get_af();
uint16_t get_bc();
uint16_t get_de();
uint16_t get_hl();

enum RR {AF, BC, DE, HL};
uint8_t* get_rr_1(uint8_t type);
uint8_t* get_rr_2(uint8_t type);
uint16_t get_rr(uint8_t type);

enum R {A, F, B, C, D, E, H, L, MEMHL};

//bool IME; // interrupt master enable flag

       
        
int execute_cb_op(uint8_t t);

// Helper Functions    
uint8_t fetch(uint16_t index);
void post(uint16_t index, uint8_t value);
uint8_t fetchOP();
uint16_t x8_x16(uint8_t value1, uint8_t value2);
uint8_t x16_x81(uint16_t value);
uint8_t x16_x82(uint16_t value);

int get_flag(enum flags flag);
void set_flag(enum flags flag, int value);

void set_af(uint16_t value);
void set_bc(uint16_t value);
void set_de(uint16_t value);
void set_hl(uint16_t value);


uint8_t get_mem_hl(); 
void set_mem_hl(uint8_t value); 


// Op Codes

// 8 bit load commands

int LD_B_R(uint8_t r); 
int LD_C_R(uint8_t r);
int LD_D_R(uint8_t r);
int LD_E_R(uint8_t r);
int LD_H_R(uint8_t r);
int LD_L_R(uint8_t r);
int LD_HL_R(uint8_t r);                 // *(HL) = REG
int LD_A_R(uint8_t r);
                           
int LD_B_N(uint8_t t);
int LD_C_N(uint8_t t);
int LD_D_N(uint8_t t);
int LD_E_N(uint8_t t);
int LD_H_N(uint8_t t);
int LD_L_N(uint8_t t);
int LD_HL_N(uint8_t t);
int LD_A_N(uint8_t t);

int LD_A_BC(uint8_t t);                             // A = *(BC)
int LD_A_DE(uint8_t t);                             // A = *(DE)
int LD_A_NN(uint8_t t);               // A = *(Value)  
int LD_BC_A(uint8_t t);                             // *(BC) = A
int LD_DE_A(uint8_t t);                             // *(DE) = A
int LD_NN_A(uint8_t t);                             // *(NN) = A
int LD_A_ION(uint8_t t);               // A = *(FF00+n) 
int LD_ION_A(uint8_t t);               // *(FF00+n) = A
int LD_A_IOC(uint8_t t);                            // A = *(FF00+C)
int LD_IOC_A(uint8_t t);                            // *(FF00+c) = A
int LDI_HL_A(uint8_t t);                            // *(HL) = A, HL+=1
int LDI_A_HL(uint8_t t);                            // A = *(HL), HL+=1
int LDD_HL_A(uint8_t t);                            // *(HL) = A, HL-=1
int LDD_A_HL(uint8_t t);                            // A = *(HL), HL-=1

// 16 Bit Load Commands
int LD_RR_NN(uint8_t type);// RR = NN (rr is like BC or DE or HL)
int LD_SP_NN(uint8_t t); // SP = NN (is merged with op above in pan docs)
int LD_SP_HL(uint8_t t);                                                            // SP = HL
int PUSH_RR(uint8_t type);                                 // SP-=2, *(SP)=rr
int POP_RR(uint8_t type);                                  // SP+=2. rr=*(SP)
int LD_NN_SP(uint8_t t);                              // *(value1+value2) = SP. MISING IN PAN DOCS

// 8 bit Arithmetic/Logic
int ADD_A_R(uint8_t r);                // Adds register to accumulator
int ADD_A_N(uint8_t t);               // Adds value to accumulator
int ADD_A_HL(uint8_t t);                           // Adds (HL) pointer's value to accumulator
int ADC_A_R(uint8_t r);                 // Adds register and c flag to accumulator
int ADC_A_N(uint8_t t);               // Adds value and c flag to accumulator
int ADC_A_HL(uint8_t t);                           // Adds (HL) pointer's value and c flag to accumulator
int SUB_A_R(uint8_t r);                // Subtracts register from accumulator
int SUB_A_N(uint8_t t);               // Subtracts value from accumulator
int SUB_A_HL(uint8_t t);                           // Subtracts (HL) pointer's value from accumulator
int SBC_A_R(uint8_t r);                // Subtracts register and c flag from accumulator
int SBC_A_N(uint8_t t);               // Subtracts value and c flag from accumulator
int SBC_A_HL(uint8_t t);                           // Subtracts (HL) pointer's value and c flag from accumulator
int AND_A_R(uint8_t r);                // Masks A and Register, returns into A
int AND_A_N(uint8_t t);               // Masks A and value, returns into A
int AND_A_HL(uint8_t t);                           // Masks A and (HL) pointers value, returns into A
int XOR_A_R(uint8_t r);                // Opposite of Mask A and Register, returns into A
int XOR_A_N(uint8_t t);               // Opposite of Mask A and value, returns into A
int XOR_A_HL(uint8_t t);                           // Opposite of Mask A and (HL) pointers value, returns into A
int OR_A_R(uint8_t r);                 // Combines A and Register, returns into A
int OR_A_N(uint8_t t);                // Combines A and value, returns into A
int OR_A_HL(uint8_t t);                            // Combines A and (HL) pointers value, returns into A
int CP_A_R(uint8_t r);                 // Compares A and Register, returns into A
int CP_A_N(uint8_t t);                // Compares A and value, returns into A
int CP_A_HL(uint8_t t);                            // Compares A and (HL) pointers value, returns into A
int INC_R(uint8_t r);                  // Adds one to register
int INC_HL(uint8_t t);                             // Adds on to (HL) pointer's value
int DEC_R(uint8_t r);                  // Subtracts one from register
int DEC_HL(uint8_t t);                             // Subtracts one from (HL) pointer's value
int DAA(uint8_t t);                                // BCD Corrects Acumulator.(idk what that means)
int CPL(uint8_t t);                                // Flips all bits in Acumulator

// 16 Bit Arithmetic
int ADD_HL_RR(uint8_t type); // HL = HL + rr
int ADD_HL_SP(uint8_t t);
int INC_RR(uint8_t type);   // RR += 1
int DEC_RR(uint8_t type);   // RR -= 1
int INC_SP(uint8_t t);
int DEC_SP(uint8_t t);
int ADD_SP_DD(uint8_t t); // SP += DD
int LD_HL_SP_DD(uint8_t t); // HL = SP + DD


// Rotate And Shift Commands
int RLCA(uint8_t t);                             // rotates accumulator left, bit 7 -> cy flag and bit  0
int RLA(uint8_t t);                              // rotates accumulator left, bit 0 = cy bit 7 -> cy
int RRCA(uint8_t t);                             // rotates accumulator right, bit 0->cy flag and bit 7
int RCA(uint8_t t);                              // rotates accumulator right, bit 7 = cy flag, bit 0 -> cy flag
int RLC_R(uint8_t r);                // rotate register left bit 7 -> cy flag
int RLC_HL(uint8_t t);                           // rotate HL pointer's value left bit 7 -> cy flag
int RL_R(uint8_t r);                 // rotate register left bit 0 = cy flag
int RL_HL(uint8_t t);                            // rotate HL pointer's value left, 0 = cy flag
int RRC_R(uint8_t r);                // rotate register right bit 0 -> cy flag
int RRC_HL(uint8_t t);                           // rotate HL pointer's value right bit 0 -> cy flag
int RR_R(uint8_t r);                 // rotate register right bit 7 = cy flag
int RR_HL(uint8_t t);                            // rotate HL pointer's value right, 7 = cy flag
int SLA_R(uint8_t r);                // shift register left arithmetic 0 = 0
int SLA_HL(uint8_t t);                           // shift HL pointer's value left 0 = 0
int SWAP_R(uint8_t r);               // Swaps 0-4 bits and 5-8 bits. of register
int SWAP_HL(uint8_t t);                          // Swaps 0-4 bits and 5-8 bits of HL pointer's value
int SRA_R(uint8_t r);                // Shifts register right arithmetacilly (7=7)
int SRA_HL(uint8_t t);                           // Shifts HL Pointer's value right arithmetically (7=7)
int SRL_R(uint8_t r);                // Shifts register right logically (7=0)
int SRL_HL(uint8_t t);                           // Shifts HL Pointer's value right logically (7=0)

// Singlebit Commands
int BIT_0_R(uint8_t r);   // Copies Bit N of register into Z Flag
int BIT_1_R(uint8_t r);   // Copies Bit N of register into Z Flag
int BIT_2_R(uint8_t r);   // Copies Bit N of register into Z Flag
int BIT_3_R(uint8_t r);   // Copies Bit N of register into Z Flag
int BIT_4_R(uint8_t r);   // Copies Bit N of register into Z Flag
int BIT_5_R(uint8_t r);   // Copies Bit N of register into Z Flag
int BIT_6_R(uint8_t r);   // Copies Bit N of register into Z Flag
int BIT_7_R(uint8_t r);   // Copies Bit N of register into Z Flag

int SET_0_R(uint8_t r);
int SET_1_R(uint8_t r);
int SET_2_R(uint8_t r);
int SET_3_R(uint8_t r);
int SET_4_R(uint8_t r);
int SET_5_R(uint8_t r);
int SET_6_R(uint8_t r);
int SET_7_R(uint8_t r);

int RES_0_R(uint8_t r);
int RES_1_R(uint8_t r);
int RES_2_R(uint8_t r);
int RES_3_R(uint8_t r);
int RES_4_R(uint8_t r);
int RES_5_R(uint8_t r);
int RES_6_R(uint8_t r);
int RES_7_R(uint8_t r);


// CPU Control Commands
int CCF(uint8_t t);                                  // Flips CY Flag
int SCF(uint8_t t);                                  // Sets CY Flag to 1
int NOP(uint8_t t);                                  // No Operation
int HALT(uint8_t t);                                 // HAlt until Interrupt (low power)
int STOP(uint8_t t);                                 // Low power standby mode (very low power)
int DI(uint8_t t);                                   // disable interrupts
int EI(uint8_t t);                                   // Enable interrupts

// Jump commands
int JP_NN(uint8_t t);  // PC = 16 bit value1+2
int JP_HL(uint8_t t);                                // PC = HL 
int JP_FT_NN(uint8_t fl);  // PC = 16 bit value1+2 if FLAG is set
int JP_FF_NN(uint8_t fl);  // PC = 16 bit value1+2 if FLAG is NOT set
int JR_DD(uint8_t t);                   // PC += value
int JR_FT_DD(uint8_t fl);   // PC += value if FLAG is set
int JR_FF_DD(uint8_t fl);   // PC += value if FLAG is set
int CALL_NN(uint8_t t);// PC = 16 bit value1+2, SP-=2, (SP) = PC
int CALL_FT_NN(uint8_t fl);// PC = 16 bit value1+2, SP-=2, (SP) = PC IF Flag is set
int CALL_FF_NN(uint8_t fl);// PC = 16 bit value1+2, SP-=2, (SP) = PC IF Flag is set
int RET(uint8_t t);                                  // PC=(SP), SP+=2
int RET_FT(uint8_t fl);                    // PC=(SP), SP+=2 IF Flag is set
int RET_FF(uint8_t fl);                    // PC=(SP), SP+=2 IF Flag is set
int RETI(uint8_t t);                                 // PC=(SP), SP+=2, enable interrupts
int RST(uint8_t value);                     // Like CALL, but only 1 byte, and calls 00,08,10,18,20,28,30 or 38


#endif

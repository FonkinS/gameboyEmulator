#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "interrupts.h"

int CPUInit();

//void connect_bus(Bus *b);
int execute_op();

uint16_t PC; // program counter
uint16_t SP; // stack pointer

uint8_t a;
uint8_t b;
uint8_t c;
uint8_t d;
uint8_t e;
uint8_t f;
uint8_t h;
uint8_t l;
        
//int set_reg(char r, uint8_t val);   // For the Unit Tester
//uint8_t get_reg(char r);            // For the Unit Tester

uint16_t get_af();
uint16_t get_bc();
uint16_t get_de();
uint16_t get_hl();


//bool IME; // interrupt master enable flag

int serial_interrupt;
unsigned long long cycles;

enum {NOHALT, HALTIME, HALTNOIMENOINT, HALTBUG, HALTBUGEI, HALTBUGRST};
int halt;
        
        
uint8_t execute_cb_op(uint8_t next_op);

// Helper Functions    
uint8_t fetch(uint16_t index);
void post(uint16_t index, uint8_t value);
uint8_t fetchOP();
uint16_t x8_x16(uint8_t value1, uint8_t value2);
uint8_t x16_x81(uint16_t value);
uint8_t x16_x82(uint16_t value);

int get_flag(int flag);
void set_flag(int flag, int value);

void set_af(uint16_t value);
void set_bc(uint16_t value);
void set_de(uint16_t value);
void set_hl(uint16_t value);

enum flags {
    zero = 128,
    subtract = 64,
    half_carry = 32,
    carry = 16
};

uint8_t z;
uint8_t n;
uint8_t hy;
uint8_t cy;

// Op Codes

// 8 bit load commands

int LD_R_R(uint8_t* reg1, uint8_t* reg2);  // REG1 = REG2
int LD_R_N(uint8_t* reg, uint8_t value);   // REG = Value
int LD_R_HL(uint8_t* reg);                 // REG = *(HL)
int LD_HL_R(uint8_t* reg);                 // *(HL) = REG
int LD_HL_N(uint8_t value);                // *(HL) = Value
int LD_A_BC();                             // A = *(BC)
int LD_A_DE();                             // A = *(DE)
int LD_A_NN(uint8_t value1, uint8_t value2);               // A = *(Value)  
int LD_BC_A();                             // *(BC) = A
int LD_DE_A();                             // *(DE) = A
int LD_NN_A(uint8_t value1, uint8_t value2);                             // *(NN) = A
int LD_A_ION(uint8_t index);               // A = *(FF00+n) 
int LD_ION_A(uint8_t index);               // *(FF00+n) = A
int LD_A_IOC();                            // A = *(FF00+C)
int LD_IOC_A();                            // *(FF00+c) = A
int LDI_HL_A();                            // *(HL) = A, HL+=1
int LDI_A_HL();                            // A = *(HL), HL+=1
int LDD_HL_A();                            // *(HL) = A, HL-=1
int LDD_A_HL();                            // A = *(HL), HL-=1

// 16 Bit Load Commands
int LD_RR_NN(uint8_t *reg1, uint8_t *reg2, uint8_t value1, uint8_t value2);// RR = NN (rr is like BC or DE or HL)
int LD_SP_NN(uint8_t value1, uint8_t value2); // SP = NN (is merged with op above in pan docs)
int LD_SP_HL();                                                            // SP = HL
int PUSH_RR(uint8_t *reg1, uint8_t *reg2);                                 // SP-=2, *(SP)=rr
int POP_RR(uint8_t *reg1, uint8_t *reg2);                                  // SP+=2. rr=*(SP)
int LD_NN_SP(uint8_t value1, uint8_t value2);                              // *(value1+value2) = SP. MISING IN PAN DOCS

// 8 bit Arithmetic/Logic
int ADD_A_R(uint8_t* reg);                // Adds register to accumulator
int ADD_A_N(uint8_t value);               // Adds value to accumulator
int ADD_A_HL();                           // Adds (HL) pointer's value to accumulator
int ADC_A_R(uint8_t* reg);                 // Adds register and c flag to accumulator
int ADC_A_N(uint8_t value);               // Adds value and c flag to accumulator
int ADC_A_HL();                           // Adds (HL) pointer's value and c flag to accumulator
int SUB_A_R(uint8_t* reg);                // Subtracts register from accumulator
int SUB_A_N(uint8_t value);               // Subtracts value from accumulator
int SUB_A_HL();                           // Subtracts (HL) pointer's value from accumulator
int SBC_A_R(uint8_t* reg);                // Subtracts register and c flag from accumulator
int SBC_A_N(uint8_t value);               // Subtracts value and c flag from accumulator
int SBC_A_HL();                           // Subtracts (HL) pointer's value and c flag from accumulator
int AND_A_R(uint8_t* reg);                // Masks A and Register, returns into A
int AND_A_N(uint8_t value);               // Masks A and value, returns into A
int AND_A_HL();                           // Masks A and (HL) pointers value, returns into A
int XOR_A_R(uint8_t* reg);                // Opposite of Mask A and Register, returns into A
int XOR_A_N(uint8_t value);               // Opposite of Mask A and value, returns into A
int XOR_A_HL();                           // Opposite of Mask A and (HL) pointers value, returns into A
int OR_A_R(uint8_t* reg);                 // Combines A and Register, returns into A
int OR_A_N(uint8_t value);                // Combines A and value, returns into A
int OR_A_HL();                            // Combines A and (HL) pointers value, returns into A
int CP_A_R(uint8_t* reg);                 // Compares A and Register, returns into A
int CP_A_N(uint8_t value);                // Compares A and value, returns into A
int CP_A_HL();                            // Compares A and (HL) pointers value, returns into A
int INC_R(uint8_t* reg);                  // Adds one to register
int INC_HL();                             // Adds on to (HL) pointer's value
int DEC_R(uint8_t* reg);                  // Subtracts one from register
int DEC_HL();                             // Subtracts one from (HL) pointer's value
int DAA();                                // BCD Corrects Acumulator.(idk what that means)
int CPL();                                // Flips all bits in Acumulator

// 16 Bit Arithmetic
int ADD_HL_RR(uint8_t* reg1, uint8_t* reg2); // HL = HL + rr
int ADD_HL_SP();
int INC_RR(uint8_t* reg1, uint8_t* reg2);   // RR += 1
int DEC_RR(uint8_t* reg1, uint8_t* reg2);   // RR -= 1
int INC_SP();
int DEC_SP();
int ADD_SP_DD(int8_t value); // SP += DD
int LD_HL_SP_DD(int8_t value); // HL = SP + DD


// Rotate And Shift Commands
int RLCA();                             // rotates accumulator left, bit 7 -> cy flag and bit  0
int RLA();                              // rotates accumulator left, bit 0 = cy bit 7 -> cy
int RRCA();                             // rotates accumulator right, bit 0->cy flag and bit 7
int RCA();                              // rotates accumulator right, bit 7 = cy flag, bit 0 -> cy flag
int RLC_R(uint8_t* reg);                // rotate register left bit 7 -> cy flag
int RLC_HL();                           // rotate HL pointer's value left bit 7 -> cy flag
int RL_R(uint8_t* reg);                 // rotate register left bit 0 = cy flag
int RL_HL();                            // rotate HL pointer's value left, 0 = cy flag
int RRC_R(uint8_t* reg);                // rotate register right bit 0 -> cy flag
int RRC_HL();                           // rotate HL pointer's value right bit 0 -> cy flag
int RR_R(uint8_t* reg);                 // rotate register right bit 7 = cy flag
int RR_HL();                            // rotate HL pointer's value right, 7 = cy flag
int SLA_R(uint8_t* reg);                // shift register left arithmetic 0 = 0
int SLA_HL();                           // shift HL pointer's value left 0 = 0
int SWAP_R(uint8_t* reg);               // Swaps 0-4 bits and 5-8 bits. of register
int SWAP_HL();                          // Swaps 0-4 bits and 5-8 bits of HL pointer's value
int SRA_R(uint8_t* reg);                // Shifts register right arithmetacilly (7=7)
int SRA_HL();                           // Shifts HL Pointer's value right arithmetically (7=7)
int SRL_R(uint8_t* reg);                // Shifts register right logically (7=0)
int SRL_HL();                           // Shifts HL Pointer's value right logically (7=0)

// Singlebit Commands
int BIT_N_R(uint8_t value, uint8_t* reg);   // Copies Bit N of register into Z Flag
int BIT_N_HL(uint8_t value);                // Copies Bit N of HL Pointer's value into Z Flag
int SET_N_R(uint8_t value, uint8_t* reg);   // Sets Bit N of register to 1
int SET_N_HL(uint8_t value);                // Sets bit N of HL Pointer's value to 1
int RES_N_R(uint8_t value, uint8_t *reg);   // Sets Bit N of register to 0
int RES_N_HL(uint8_t value);                // Sets bit N of HL Pointer's value to 0

// CPU Control Commands
int CCF();                                  // Flips CY Flag
int SCF();                                  // Sets CY Flag to 1
int NOP();                                  // No Operation
int HALT();                                 // HAlt until Interrupt (low power)
int STOP();                                 // Low power standby mode (very low power)
int DI();                                   // disable interrupts
int EI();                                   // Enable interrupts

// Jump commands
int JP_NN(uint8_t value1, uint8_t value2);  // PC = 16 bit value1+2
int JP_HL();                                // PC = HL 
int JP_F_NN(uint8_t value1, uint8_t value2, bool flag_value);  // PC = 16 bit value1+2 if FLAG is set
int JR_DD(int8_t value);                   // PC += value
int JR_F_DD(int8_t value, bool flag_value);   // PC += value if FLAG is set
int CALL_NN(uint8_t value1, uint8_t value2);// PC = 16 bit value1+2, SP-=2, (SP) = PC
int CALL_F_NN(uint8_t value1, uint8_t value2, bool flag_value);// PC = 16 bit value1+2, SP-=2, (SP) = PC IF Flag is set
int RET();                                  // PC=(SP), SP+=2
int RET_F(bool flag_value);                    // PC=(SP), SP+=2 IF Flag is set
int RETI();                                 // PC=(SP), SP+=2, enable interrupts
int RST(uint8_t value);                     // Like CALL, but only 1 byte, and calls 00,08,10,18,20,28,30 or 38


#endif

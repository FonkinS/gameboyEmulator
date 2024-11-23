#include "cpu.h"
#include "interrupts.h"
#include <stdint.h>
#include <stdio.h>

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


int serial_interrupt;
unsigned long long cycles;

int halt;
 

int CPUInit() {
    IME = false;
    return 0;
}


void interrupt() {
    if (IME) {
        halt = NOHALT;
        // Disable IME and IF Flag
        IME = false;
        IF &= ~interrupt_called;
        
        // PUSH PC to stack
        post(SP-1, (uint8_t) (PC>>8));
        post(SP-2, (uint8_t) PC);
        SP-=2;

        // Call register
        PC = interruptTypeToLocation(interrupt_called);
    }
    interrupt_called = 0;
}

// Fetching
uint8_t fetch(uint16_t index){
    //if (index == 0xff44 && data[0xff50]) return 0x90; ONLY FOR BlarggsTESTS
    return BusRead(index);
}


uint8_t fetchOP(){
    return fetch(PC++);
}

uint16_t fetchOP16() {
    return fetchOP() + (fetchOP() << 8);
}

void set_flag(enum flags flag, int value) {
    if (value == 0) {
        f = f & ~(1 << flag);
    } else {
        f = f | (1 << flag);
    }
}

int get_flag(enum flags flag) {
    return ((f >> flag) & 1);
}

void post(uint16_t address, uint8_t value) {
    BusWrite(address, value);
}


int (*cb_ops[256])(uint8_t) = {
    RLC_R,RLC_R,RLC_R,RLC_R,RLC_R,RLC_R,RLC_HL,RLC_R,RRC_R,RRC_R,RRC_R,RRC_R,RRC_R,RRC_R,RRC_HL,RRC_R,RL_R,RL_R,RL_R,RL_R,RL_R,RL_R,RL_HL,RL_R,RR_R,RR_R,RR_R,RR_R,RR_R,RR_R,RR_HL,RR_R,SLA_R,SLA_R,SLA_R,SLA_R,SLA_R,SLA_R,SLA_HL,SLA_R,SRA_R,SRA_R,SRA_R,SRA_R,SRA_R,SRA_R,SRA_HL,SRA_R,SWAP_R,SWAP_R,SWAP_R,SWAP_R,SWAP_R,SWAP_R,SWAP_HL,SWAP_R,SRL_R,SRL_R,SRL_R,SRL_R,SRL_R,SRL_R,SRL_HL,SRL_R,BIT_0_R,BIT_0_R,BIT_0_R,BIT_0_R,BIT_0_R,BIT_0_R,BIT_0_R,BIT_0_R,BIT_1_R,BIT_1_R,BIT_1_R,BIT_1_R,BIT_1_R,BIT_1_R,BIT_1_R,BIT_1_R,BIT_2_R,BIT_2_R,BIT_2_R,BIT_2_R,BIT_2_R,BIT_2_R,BIT_2_R,BIT_2_R,BIT_3_R,BIT_3_R,BIT_3_R,BIT_3_R,BIT_3_R,BIT_3_R,BIT_3_R,BIT_3_R,BIT_4_R,BIT_4_R,BIT_4_R,BIT_4_R,BIT_4_R,BIT_4_R,BIT_4_R,BIT_4_R,BIT_5_R,BIT_5_R,BIT_5_R,BIT_5_R,BIT_5_R,BIT_5_R,BIT_5_R,BIT_5_R,BIT_6_R,BIT_6_R,BIT_6_R,BIT_6_R,BIT_6_R,BIT_6_R,BIT_6_R,BIT_6_R,BIT_7_R,BIT_7_R,BIT_7_R,BIT_7_R,BIT_7_R,BIT_7_R,BIT_7_R,BIT_7_R,RES_0_R,RES_0_R,RES_0_R,RES_0_R,RES_0_R,RES_0_R,RES_0_R,RES_0_R,RES_1_R,RES_1_R,RES_1_R,RES_1_R,RES_1_R,RES_1_R,RES_1_R,RES_1_R,RES_2_R,RES_2_R,RES_2_R,RES_2_R,RES_2_R,RES_2_R,RES_2_R,RES_2_R,RES_3_R,RES_3_R,RES_3_R,RES_3_R,RES_3_R,RES_3_R,RES_3_R,RES_3_R,RES_4_R,RES_4_R,RES_4_R,RES_4_R,RES_4_R,RES_4_R,RES_4_R,RES_4_R,RES_5_R,RES_5_R,RES_5_R,RES_5_R,RES_5_R,RES_5_R,RES_5_R,RES_5_R,RES_6_R,RES_6_R,RES_6_R,RES_6_R,RES_6_R,RES_6_R,RES_6_R,RES_6_R,RES_7_R,RES_7_R,RES_7_R,RES_7_R,RES_7_R,RES_7_R,RES_7_R,RES_7_R,SET_0_R,SET_0_R,SET_0_R,SET_0_R,SET_0_R,SET_0_R,SET_0_R,SET_0_R,SET_1_R,SET_1_R,SET_1_R,SET_1_R,SET_1_R,SET_1_R,SET_1_R,SET_1_R,SET_2_R,SET_2_R,SET_2_R,SET_2_R,SET_2_R,SET_2_R,SET_2_R,SET_2_R,SET_3_R,SET_3_R,SET_3_R,SET_3_R,SET_3_R,SET_3_R,SET_3_R,SET_3_R,SET_4_R,SET_4_R,SET_4_R,SET_4_R,SET_4_R,SET_4_R,SET_4_R,SET_4_R,SET_5_R,SET_5_R,SET_5_R,SET_5_R,SET_5_R,SET_5_R,SET_5_R,SET_5_R,SET_6_R,SET_6_R,SET_6_R,SET_6_R,SET_6_R,SET_6_R,SET_6_R,SET_6_R,SET_7_R,SET_7_R,SET_7_R,SET_7_R,SET_7_R,SET_7_R,SET_7_R,SET_7_R
};
uint8_t cb_params[256] = {
B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A
};

int execute_cb_op(uint8_t t) {
    uint8_t op = fetchOP();
    return (cb_ops[op])(cb_params[op]);
}

int (*ops[256])(uint8_t) = {
NOP,LD_RR_NN,LD_BC_A,INC_RR,INC_R,DEC_R,LD_B_N,RLCA,LD_NN_SP,ADD_HL_RR,LD_A_BC,DEC_RR,INC_R,DEC_R,LD_C_N,RRCA,STOP,LD_RR_NN,LD_DE_A,INC_RR,INC_R,DEC_R,LD_D_N,RLA,JR_DD,ADD_HL_RR,LD_A_DE,DEC_RR,INC_R,DEC_R,LD_E_N,RCA,JR_FF_DD,LD_RR_NN,LDI_HL_A,INC_RR,INC_R,DEC_R,LD_H_N,DAA,JR_FT_DD,ADD_HL_RR,LDI_A_HL,DEC_RR,INC_R,DEC_R,LD_L_N,CPL,JR_FF_DD,LD_SP_NN,LDD_HL_A,INC_SP,INC_HL,DEC_HL,LD_HL_N,SCF,JR_FT_DD,ADD_HL_SP,LDD_A_HL,DEC_SP,INC_R,DEC_R,LD_A_N,CCF,LD_B_R,LD_B_R,LD_B_R,LD_B_R,LD_B_R,LD_B_R,LD_B_R,LD_B_R,LD_C_R,LD_C_R,LD_C_R,LD_C_R,LD_C_R,LD_C_R,LD_C_R,LD_C_R,LD_D_R,LD_D_R,LD_D_R,LD_D_R,LD_D_R,LD_D_R,LD_D_R,LD_D_R,LD_E_R,LD_E_R,LD_E_R,LD_E_R,LD_E_R,LD_E_R,LD_E_R,LD_E_R,LD_H_R,LD_H_R,LD_H_R,LD_H_R,LD_H_R,LD_H_R,LD_H_R,LD_H_R,LD_L_R,LD_L_R,LD_L_R,LD_L_R,LD_L_R,LD_L_R,LD_L_R,LD_L_R,LD_HL_R,LD_HL_R,LD_HL_R,LD_HL_R,LD_HL_R,LD_HL_R,HALT,LD_HL_R,LD_A_R,LD_A_R,LD_A_R,LD_A_R,LD_A_R,LD_A_R,LD_A_R,LD_A_R,ADD_A_R,ADD_A_R,ADD_A_R,ADD_A_R,ADD_A_R,ADD_A_R,ADD_A_HL,ADD_A_R,ADC_A_R,ADC_A_R,ADC_A_R,ADC_A_R,ADC_A_R,ADC_A_R,ADC_A_HL,ADC_A_R,SUB_A_R,SUB_A_R,SUB_A_R,SUB_A_R,SUB_A_R,SUB_A_R,SUB_A_HL,SUB_A_R,SBC_A_R,SBC_A_R,SBC_A_R,SBC_A_R,SBC_A_R,SBC_A_R,SBC_A_HL,SBC_A_R,AND_A_R,AND_A_R,AND_A_R,AND_A_R,AND_A_R,AND_A_R,AND_A_HL,AND_A_R,XOR_A_R,XOR_A_R,XOR_A_R,XOR_A_R,XOR_A_R,XOR_A_R,XOR_A_HL,XOR_A_R,OR_A_R,OR_A_R,OR_A_R,OR_A_R,OR_A_R,OR_A_R,OR_A_HL,OR_A_R,CP_A_R,CP_A_R,CP_A_R,CP_A_R,CP_A_R,CP_A_R,CP_A_HL,CP_A_R,RET_FF,POP_RR,JP_FF_NN,JP_NN,CALL_FF_NN,PUSH_RR,ADD_A_N,RST,RET_FT,RET,JP_FT_NN,execute_cb_op ,CALL_FT_NN,CALL_NN,ADC_A_N,RST,RET_FF,POP_RR,JP_FF_NN,NOP,CALL_FF_NN,PUSH_RR,SUB_A_N,RST,RET_FT,RETI,JP_FT_NN,NOP,CALL_FT_NN,NOP,SBC_A_N,RST,LD_ION_A,POP_RR,LD_IOC_A,NOP,NOP,PUSH_RR,AND_A_N,RST,ADD_SP_DD,JP_HL,LD_NN_A,NOP,NOP,NOP,XOR_A_N,RST,LD_A_ION,POP_RR,LD_A_IOC,DI,NOP,PUSH_RR,OR_A_N,RST,LD_HL_SP_DD,LD_SP_HL,LD_A_NN,EI,NOP,NOP,CP_A_N,RST
};
uint8_t params[256] = {0,BC,0,BC,B,B,0,0,0,BC,0,BC,C,C,0,0,0,DE,0,DE,D,D,0,0,0,DE,0,DE,E,E,0,0,z,HL,0,HL,H,H,0,0,z,HL,0,HL,L,L,0,0,cy,0,0,0,0,0,0,0,cy,0,0,0,A,A,0,0,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,MEMHL,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,B,C,D,E,H,L,0,A,z,BC,z,0,z,BC,0,0,z,0,z,0,z,0,0,1,cy,DE,cy,0,cy,DE,0,2,cy,0,cy,0,cy,0,0,3,0,HL,0,0,0,HL,0,4,0,0,0,0,0,0,0,5,0,AF,0,0,0,AF,0,6,0,0,0,0,0,0,0,7};

int execute_op() {
    uint8_t op = fetchOP();
    return (ops[op])(params[op]);
}



uint16_t x8_x16(uint8_t value1, uint8_t value2) {
    return ((uint16_t) value1<<8) + (value2);
}

uint8_t x16_x81(uint16_t value) {
    return (uint8_t) (value>>8);
}

uint8_t x16_x82(uint16_t value) {
    return (uint8_t) value;
}

// 16 bit Register setting
uint16_t get_af(){
    return (a<<8) + f;
}

void set_af(uint16_t value){
    a = (uint8_t) (value>>8);
    f = (uint8_t) value;
}

uint16_t get_bc(){
    return (b<<8) + c;
}

void set_bc(uint16_t value){
    b = (uint8_t) (value>>8);
    c = (uint8_t) value;
}

uint16_t get_de(){
    return (d<<8)+e;
}

void set_de(uint16_t value){
    d = (uint8_t) (value>>8);
    e = (uint8_t) value;
}

uint16_t get_hl(){
    return (h<<8)+l;
}

void set_hl(uint16_t value){
    h = (uint8_t) (value>>8);
    l = (uint8_t) value;
}

uint8_t get_mem_hl() {
    return fetch((h<<8)+l);
}
void set_mem_hl(uint8_t value) {
    post(get_hl(), value);
}

uint8_t *regs[8] = {&a, &f, &b, &c, &d, &e, &h, &l};
uint8_t* get_rr_1(uint8_t type) {
    return regs[type * 2];
}

uint8_t* get_rr_2(uint8_t type) {
    return regs[type * 2+1];
}

uint16_t get_rr(uint8_t type) {
    return (*regs[type * 2] << 8) + *regs[type*2+1];
}


// Opcodes:
int LD_B_R(uint8_t r){
    if (r == MEMHL) b = get_mem_hl();
    else b = *(regs[r]);
    return r == MEMHL ? 8 : 4;
}
int LD_C_R(uint8_t r){
    if (r == MEMHL) c = get_mem_hl();
    else c = *(regs[r]);
    return r == MEMHL ? 8 : 4;
}
int LD_D_R(uint8_t r){
    if (r == MEMHL) d = get_mem_hl();
    else d = *(regs[r]);
    return r == MEMHL ? 8 : 4;
}
int LD_E_R(uint8_t r){
    if (r == MEMHL) e = get_mem_hl();
    else e = *(regs[r]);
    return r == MEMHL ? 8 : 4;
}
int LD_H_R(uint8_t r){
    if (r == MEMHL) h = get_mem_hl();
    else h = *(regs[r]);
    return r == MEMHL ? 8 : 4;
}
int LD_L_R(uint8_t r){
    if (r == MEMHL) l = get_mem_hl();
    else l = *(regs[r]);
    return r == MEMHL ? 8 : 4;
}
int LD_HL_R(uint8_t r){
    set_mem_hl(*(regs[r]));
    return 8;
}
int LD_A_R(uint8_t r){
    if (r == MEMHL) a = get_mem_hl();
    else a = *(regs[r]);
    return r == MEMHL ? 8 : 4;
}



int LD_B_N(uint8_t t){
    b = fetchOP();
    return 8;
}
int LD_C_N(uint8_t t){
    c = fetchOP();
    return 8;
}
int LD_D_N(uint8_t t){
    d = fetchOP();
    return 8;
}
int LD_E_N(uint8_t t){
    e = fetchOP();
    return 8;
}
int LD_H_N(uint8_t t){
    h = fetchOP();
    return 8;
}
int LD_L_N(uint8_t t){
    l = fetchOP();
    return 8;
}
int LD_HL_N(uint8_t t){
    set_mem_hl(fetchOP());
    return 12;
}
int LD_A_N(uint8_t t){
    a = fetchOP();
    return 8;
}


int LD_A_BC(uint8_t t){
    a = fetch(get_bc());
    return 8;
}                             // A = *(BC)
int LD_A_DE(uint8_t t){
    a = fetch(get_de());
    return 8;
}                             // A = *(DE)
int LD_A_NN(uint8_t t){
    a = fetch(fetchOP16());
    return 16;
}               // A = *(Value)
int LD_BC_A(uint8_t t){
    post(get_bc(), a);
    return 8;
}                           // *(BC) = A
int LD_DE_A(uint8_t t){
    post(get_de(), a);
    return 8;
}                             // *(DE) = A
int LD_NN_A(uint8_t t){
    post(fetchOP16(), a);
    return 16;
}                             // *(NN) = A
int LD_A_ION(uint8_t t){
    a = fetch((uint16_t)(0xFF00 + (uint16_t)fetchOP()));
    return 12;
}               // A = *(FF00+n) 
int LD_ION_A(uint8_t t){
    post(0xFF00 + fetchOP(), a);
    return 12;
}               // *(FF00+n) = A
int LD_A_IOC(uint8_t t){
    a = fetch(0xFF00 + c);
    return 8;
}                            // A = *(FF00+C)
int LD_IOC_A(uint8_t t){
    post(0xFF00 + c, a);
    return 8;
}                            // *(FF00+c) = A
int LDI_HL_A(uint8_t t){
    post(get_hl(), a);
    set_hl(get_hl() + 0x0001);
    return 8;
}                            // *(HL) = A, HL+=1
int LDI_A_HL(uint8_t t){
    a = get_mem_hl();
    set_hl(get_hl()+0x0001);
    return 8;
}                            // A = *(HL), HL+=1
int LDD_HL_A(uint8_t t){
    post(get_hl(), a);
    set_hl(get_hl()-1);
    return 8;
}                            // *(HL) = A, HL-=1
int LDD_A_HL(uint8_t t){
    a = get_mem_hl();
    set_hl(get_hl()-1);
    return 8;
}                            // A = *(HL), HL-=1
        
// 16 Bit Load Commands
int LD_RR_NN(uint8_t type){
    *get_rr_2(type) = fetchOP();
    *get_rr_1(type) = fetchOP();
    return 12;
}// RR = NN (rr is like BC or DE or HL)
int LD_SP_NN(uint8_t t){
    SP = fetchOP16();
    return 12;
} // SP = NN (is merged with op above in pan docs)
int LD_SP_HL(uint8_t t){
    SP = get_hl();
    return 8;
}                                                            // SP = HL
int PUSH_RR(uint8_t type){
    post(SP-1, *get_rr_1(type));
    post(SP-2, *get_rr_2(type));
    SP-=2;
    return 16;
}                                 // SP-=2, *(SP)=rr
int POP_RR(uint8_t type){
    *get_rr_2(type) = fetch(SP);
    *get_rr_1(type) = fetch(SP+1);
    SP+=2;
    return 12;
}                                  // SP+=2. rr=*(SP)

int LD_NN_SP(uint8_t t) {
    uint16_t address = fetchOP16();
    post(address, (uint8_t) SP);
    post(address + 1, (uint8_t) (SP >> 8));
    return 20;
}
        
// 8 bit Arithmetic/Logic
int ADD_A_R(uint8_t r){
    uint8_t *reg = regs[r];
    set_flag(hy, (a & 0xf) + (*reg & 0xf) > 0xf);
    set_flag(cy, a + *reg > 0xff);
    a += *reg;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 4;
}                // Adds register to accumulator
int ADD_A_N(uint8_t t){
    uint8_t value = fetchOP();
    set_flag(hy, (a & 0xf) + (value & 0xf) > 0xf);
    set_flag(cy, a + value > 0xff);
    a += value;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 8;
}               // Adds value to accumulator
int ADD_A_HL(uint8_t t){
    set_flag(hy, (a & 0xf) + (get_mem_hl() & 0xf) > 0xf);
    set_flag(cy, a + get_mem_hl() > 0xff);
    a += get_mem_hl();
    set_flag(z, a==0);
    set_flag(n, 0);
    return 8;
}                           // Adds (HL) pointer's value to accumulator
int ADC_A_R(uint8_t r) {
    uint8_t *reg = regs[r];
    set_flag(hy, (a & 0xf) + (*reg & 0xf) + get_flag(cy)> 0xf);
    bool p = get_flag(cy);
    set_flag(cy, a + *reg + get_flag(cy) > 0xff);
    a += *reg + p;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 4;
}                 // Adds register and c flag to accumulator
int ADC_A_N(uint8_t t){
    uint8_t value = fetchOP();
    set_flag(hy, (a & 0xf) + (value & 0xf) + get_flag(cy)> 0xf);
    bool p = get_flag(cy);
    set_flag(cy, a + value + get_flag(cy) > 0xff);
    a += value + p;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 8;
}               // Adds value and c flag to accumulator
int ADC_A_HL(uint8_t t){
    set_flag(hy, (a & 0xf) + (get_mem_hl() & 0xf) + get_flag(cy)> 0xf);
    bool p = get_flag(cy);
    set_flag(cy, a + get_mem_hl() + get_flag(cy) > 0xff);
    a += get_mem_hl() + p;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 8;
}                           // Adds (HL) pointer's value and c flag to accumulator
int SUB_A_R(uint8_t r){
    uint8_t *reg = regs[r];
    set_flag(hy, (a & 0xf) - (*reg & 0xf) < 0x0);
    set_flag(cy, a - *reg < 0x0000);
    a -= *reg;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 4;
}                // Subtracts register from accumulator
int SUB_A_N(uint8_t t){
    uint8_t value = fetchOP();
    set_flag(hy, (a & 0xf) - (value & 0xf) < 0x0);
    set_flag(cy, a - value < 0x0000);
    a -= value;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 8;
}               // Subtracts value from accumulator
int SUB_A_HL(uint8_t t){
    set_flag(hy, (a & 0xf) - (get_mem_hl() & 0xf) < 0x0);
    set_flag(cy, a - get_mem_hl() < 0x0000);
    a -= get_mem_hl();
    set_flag(z, a==0);
    set_flag(n, 1);
    return 8;
}                           // Subtracts (HL) pointer's value from accumulator
int SBC_A_R(uint8_t r){
    uint8_t *reg = regs[r];
    set_flag(hy, (a & 0xf) - (*reg & 0xf) - get_flag(cy) < 0x0);
    bool p = get_flag(cy);
    set_flag(cy, a - *reg - get_flag(cy) < 0x0000);
    a = a - *reg - p;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 4;
}                // Subtracts register and c flag from accumulator
int SBC_A_N(uint8_t t){
    uint8_t value = fetchOP();
    set_flag(hy, (a & 0xf) - (value & 0xf) - get_flag(cy) < 0x0);
    bool p = get_flag(cy);
    set_flag(cy, a - value - get_flag(cy) < 0x0000);
    a = a - value - p;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 8;
}               // Subtracts value and c flag from accumulator
int SBC_A_HL(uint8_t t){
    set_flag(hy, (a & 0xf) - (get_mem_hl() & 0xf) - get_flag(cy) < 0x0);
    bool p = get_flag(cy);
    set_flag(cy, a - get_mem_hl() - get_flag(cy) < 0x0000);
    a = a - get_mem_hl() - p;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 8;
}                           // Subtracts (HL) pointer's value and c flag from accumulator
int AND_A_R(uint8_t r){
    uint8_t *reg = regs[r];
    a &= *reg;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(cy, 0);
    return 4;
}                // Masks A and Register, returns into A
int AND_A_N(uint8_t t){
    uint8_t value = fetchOP();
    a &= value;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(cy, 0);
    return 8;
}               // Masks A and value, returns into A
int AND_A_HL(uint8_t t){
    a &= get_mem_hl();
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(cy, 0);
    return 8;
}                           // Masks A and (HL) pointers value, returns into A
int XOR_A_R(uint8_t r){
    uint8_t *reg = regs[r];
    a ^= *reg;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 4;
}                // Opposite of Mask A and Register, returns into A
int XOR_A_N(uint8_t t){
    uint8_t value = fetchOP();
    a ^= value;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 8;
}               // Opposite of Mask A and value, returns into A
int XOR_A_HL(uint8_t t){
    a^= get_mem_hl();
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 8;
}                           // Opposite of Mask A and (HL) pointers value, returns into A
int OR_A_R(uint8_t r){
    uint8_t *reg = regs[r];
    a |= *reg;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 4;
}                 // Combines A and Register, returns into A
int OR_A_N(uint8_t t){
    uint8_t value = fetchOP();
    a |= value;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 8;
}                // Combines A and value, returns into A
int OR_A_HL(uint8_t t){
    a |= get_mem_hl();
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 8;
}                            // Combines A and (HL) pointers value, returns into A
int CP_A_R(uint8_t r){ 
    uint8_t *reg = regs[r];
    set_flag(z, a==*reg);
    set_flag(n, 1);
    set_flag(hy, (a & 0xf) - (*reg & 0xf) < 0);
    set_flag(cy, a-*reg < 0);
    return 4;
}                 // Compares A and Register, returns into A
int CP_A_N(uint8_t t){
    uint8_t value = fetchOP();
    set_flag(z, a==value);
    set_flag(n, 1);
    set_flag(hy, (a & 0xf) - (value & 0xf) < 0);
    set_flag(cy, a-value < 0);
    return 8;
}                // Compares A and value, returns into A
int CP_A_HL(uint8_t t){
    set_flag(z, a==get_mem_hl());
    set_flag(n, 1);
    set_flag(hy, (a & 0xf) - (get_mem_hl() & 0xf) < 0);
    set_flag(cy, a-get_mem_hl() < 0);
    return 8;
}                            // Compares A and (HL) pointers value, returns into A
int INC_R(uint8_t r){
    uint8_t *reg = regs[r];
    *reg +=1;
    set_flag(z, *reg == 0);
    set_flag(n, 0);
    set_flag(hy, (*reg>>4) % 2 != ((*reg-1)>>4) % 2);
    return 4;
}                  // Adds one to register
int INC_HL(uint8_t t){
    post(get_hl(), get_mem_hl()+1);
    set_flag(z, get_mem_hl() == 0);
    set_flag(n, 0);
    set_flag(hy, (get_mem_hl() & 0xf) == 0);
    return 12;
}                             // Adds on to (HL) pointer's value
int DEC_R(uint8_t r){
    uint8_t *reg = regs[r];
    *reg -= 1;
    set_flag(z, *reg == 0);
    set_flag(n, 1);
    set_flag(hy, (*reg>>4) % 2 != ((*reg+1)>>4) % 2);
    return 4;
}                  // Subtracts one from register
int DEC_HL(uint8_t t){
    post(get_hl(), get_mem_hl()-1);
    set_flag(z, get_mem_hl() == 0);
    set_flag(n, 1);
    set_flag(hy, (fetch((get_hl()))&0x10) != ((get_mem_hl()+1)&0x10));
    return 12;
}                             // Subtracts one from (HL) pointer's value
int DAA(uint8_t t){
    if (!get_flag(n)) {  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
        if (get_flag(cy)|| a > 0x99) { 
            a += 0x60; set_flag(cy,1); 
        }
        if (get_flag(hy) || (a & 0x0f) > 0x09) { 
            a += 0x6; 
        }
    } else {  // after a subtraction, only adjust if (half-)carry occurred
        if (get_flag(cy)) { 
            a -= 0x60; 
        }
        if (get_flag(hy)) { 
            a -= 0x6; 
        }
}
    set_flag(hy, 0);
    set_flag(z, a==0);
    return 4;
}                                // BCD Corrects Acumulator.(idk what that means)
int CPL(uint8_t t){
    set_flag(n, 1);
    set_flag(hy, 1);
    a ^= 0xff;
    return 4;
}                                // Flips all bits in Acumulator

// 16 Bit Arithmetic
int ADD_HL_RR(uint8_t type){
    uint16_t first = get_hl();
    uint16_t second = get_rr(type);
    set_flag(hy, (first&0xfff)+(second&0xfff) > 0xfff);
    set_flag(cy, first + second > 0xffff);
    set_hl(first+second);
    set_flag(n, 0);
    return 8;
} // HL = HL + rr

int ADD_HL_SP(uint8_t t) {
    set_flag(hy, (get_hl() & 0xfff) + (SP & 0xfff) > 0xfff);
    set_flag(cy, (int)get_hl() + (int)SP > 0xffff);
    set_hl(get_hl() + SP);
    set_flag(n, 0);
    return 8;
}

int INC_RR(uint8_t type){
    *get_rr_1(type) = x16_x81(get_rr(type)+1);
    *get_rr_2(type) = x16_x82(get_rr(type)+1);
    return 8;
}   // RR += 1
int DEC_RR(uint8_t type){
    *get_rr_1(type) = x16_x81(get_rr(type)-1);
    *get_rr_2(type) = x16_x82(get_rr(type)-1);
    return 8;
}   // RR -= 1

int INC_SP(uint8_t t) {
    ++SP;
    return 8;
}

int DEC_SP(uint8_t t) {
    SP--;
    return 8;
}
int ADD_SP_DD(uint8_t t){
    int8_t value = fetchOP();
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, (value&0xf) + (SP & 0xf) > 0xf);
    set_flag(cy, (uint8_t)value + (int)(SP & 0xff) > 0xff);
    SP += value;
    return 16;
} // SP += DD

int LD_HL_SP_DD(uint8_t t){
    int8_t value = fetchOP();
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, ((uint8_t)value & 0xf) + (SP & 0xf) > 0xf);
    set_flag(cy, (uint8_t)value + (SP & 0xff) > 0xff);
    set_hl(SP + value);
    return 12;
} // HL = SP + DD
        

        // Rotate And Shift Commands
int RLCA(uint8_t t){
    set_flag(cy, a>>7);
    a = (a << 1) | (a >> 7); 
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 4;
}                             // rotates accumulator left, bit 7 -> cy flag and bit  0
int RLA(uint8_t t){
    bool p = a>>7;
    a = (a << 1) | (a >> 7);
    if (get_flag(cy)== 0) { a = a & ~1;
    } else {a = a | 1;}
    set_flag(cy, p);
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 4;
}                              // rotates accumulator left, bit 0 = cy bit 7 -> cy
int RRCA(uint8_t t){
    set_flag(cy, a & 1);
    a = (a >> 1) | (a << 7); 
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 4;
}                             // rotates accumulator right, bit 0->cy flag and bit 7
int RCA(uint8_t t){
    int p = a % 2;
    a = (a >> 1) | (a << 7);
    if (get_flag(cy)== 0) { a = a & ~128;
    } else {a = a | 128;}
    set_flag(cy, p);
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 4;
}                              // rotates accumulator right, bit 7 = cy flag, bit 0 -> cy flag
int RLC_R(uint8_t r){
    uint8_t *reg = regs[r];
    set_flag(cy, *reg>>7);
    *reg = (*reg << 1) | (*reg >> 7); 
    set_flag(z, *reg==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 8;
}                // rotate register left bit 7 -> cy flag
int RLC_HL(uint8_t t){
    set_flag(cy, get_mem_hl() >>7);
    post(get_hl(), (get_mem_hl() << 1) | (fetch(get_hl()) >> 7)); 
    set_flag(z, get_mem_hl()==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 16;
}                           // rotate HL pointer's value left bit 7 -> cy flag
int RL_R(uint8_t r){
    uint8_t *reg = regs[r];
    bool p = get_flag(cy);
    set_flag(cy, *reg >> 7);
    *reg = (*reg << 1) | (*reg >> 7);
    if (p== 0) { *reg = *reg & ~1;
    } else {*reg = *reg | 1;}
    set_flag(z, *reg==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 8;

}                 // rotate register left bit 0 = cy flag
int RL_HL(uint8_t t){
    bool p = get_flag(cy);
    set_flag(cy, get_mem_hl() >> 7);
    post(get_hl(), (get_mem_hl() << 1) | (fetch(get_hl()) >> 7));
    if (p== 0) { post(get_hl(), get_mem_hl() & ~ 1);
    } else {post(get_hl(), get_mem_hl() | 1);}
    set_flag(z, get_mem_hl() == 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 16;

}                            // rotate HL pointer's value left, 0 = cy flag
int RRC_R(uint8_t r){
    uint8_t *reg = regs[r];
    set_flag(cy, (*reg)&1);
    *reg = ((*reg) >> 1) | ((*reg) << 7); 
    set_flag(z, *reg==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 8;

}                // rotate register right bit 0 -> cy flag
int RRC_HL(uint8_t t){
    bool p = get_mem_hl() & 1;
    post(get_hl(), (get_mem_hl() >> 1) | (fetch(get_hl()) << 7)); 
    set_flag(cy, p);
    set_flag(z, get_mem_hl()==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 16;
}                           // rotate HL pointer's value right bit 0 -> cy flag
int RR_R(uint8_t r){
    uint8_t *reg = regs[r];
    bool p = (*reg) & 1;
    *reg = ((*reg) >> 1) | ((*reg) << 7);
    if (get_flag(cy)== 0) { *reg = (*reg) & ~128;
    } else {*reg = (*reg) | 128;}
    set_flag(cy, p);
    set_flag(z, *reg==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 8;
}                 // rotate register right bit 7 = cy flag
int RR_HL(uint8_t t){
    bool p = get_mem_hl() & 1;
    post(get_hl(), (get_mem_hl() >> 1) | (fetch(get_hl()) << 7));
    if (get_flag(cy)== 0) {post(get_hl(), get_mem_hl() & 127);
    } else {post(get_hl(), get_mem_hl() | 128);}
    set_flag(cy, p);
    set_flag(z, get_mem_hl() == 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 16;
}                            // rotate HL pointer's value right, 7 = cy flag
int SLA_R(uint8_t r){
    uint8_t *reg = regs[r];
    bool p = *reg & 128;
    *reg = *reg << 1;
    set_flag(cy, p);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, *reg==0);
    return 8;
}                // shift register left arithmetic 0 = 0
int SLA_HL(uint8_t t){
    bool p = get_mem_hl() & 128;
    post(get_hl(), get_mem_hl() << 1);
    set_flag(cy, p);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, get_mem_hl()==0);
    return 16;
}                           // shift HL pointer's value left 0 = 0
int SWAP_R(uint8_t r){
    uint8_t *reg = regs[r];
    *reg = *reg << 4 | *reg >> 4;
    set_flag(z, *reg==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy,0);
    return 8;
}               // Swaps 0-4 bits and 5-8 bits. of register
int SWAP_HL(uint8_t t){
    post(get_hl(), get_mem_hl() << 4 | fetch(get_hl()) >> 4);
    set_flag(z, get_mem_hl()==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy,0);
    return 16;
}                          // Swaps 0-4 bits and 5-8 bits of HL pointer's value
int SRA_R(uint8_t r){
    uint8_t *reg = regs[r];
    set_flag(cy, *reg & 1);
    *reg = *reg >> 1 | (*reg & 128);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, *reg==0);
    return 8;
}                // Shifts register right arithmetacilly (7=7)
int SRA_HL(uint8_t tmp){
    set_flag(cy, get_mem_hl() & 1);
    int t = get_mem_hl();
    post(get_hl(), t >> 1 | (t & 128));
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, get_mem_hl()==0);
    return 16;
}                           // Shifts HL Pointer's value right arithmetically (7=7)
int SRL_R(uint8_t r){
    uint8_t *reg = regs[r];
    set_flag(cy, *reg & 1);
    *reg = *reg >> 1;
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, *reg==0);
    return 8;
}                // Shifts register right logically (7=0)
int SRL_HL(uint8_t t){
    set_flag(cy, get_mem_hl() & 1);
    post(get_hl(), get_mem_hl() >> 1);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, get_mem_hl()==0);
    return 16;
}                           // Shifts HL Pointer's value right logically (7=0)
        
// Singlebit Commands
int BIT_0_R(uint8_t r){
    uint8_t value = (r == MEMHL) ? get_mem_hl() : *(regs[r]);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !(1 & value));
    return r == MEMHL ? 12 : 8;
}   // Copies Bit N of register into Z Flag
int BIT_1_R(uint8_t r){
    uint8_t value = (r == MEMHL) ? get_mem_hl() : *(regs[r]);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !((2 & value) >> 1));
    return r == MEMHL ? 12 : 8;
}
int BIT_2_R(uint8_t r){
    uint8_t value = (r == MEMHL) ? get_mem_hl() : *(regs[r]);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !((4 & value) >> 2));
    return r == MEMHL ? 12 : 8;
}
int BIT_3_R(uint8_t r){
    uint8_t value = (r == MEMHL) ? get_mem_hl() : *(regs[r]);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !((8 & value) >> 3));
    return r == MEMHL ? 12 : 8;
}
int BIT_4_R(uint8_t r){
    uint8_t value = (r == MEMHL) ? get_mem_hl() : *(regs[r]);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !((16 & value) >> 4));
    return r == MEMHL ? 12 : 8;
}
int BIT_5_R(uint8_t r){
    uint8_t value = (r == MEMHL) ? get_mem_hl() : *(regs[r]);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !((32 & value) >> 5));
    return r == MEMHL ? 12 : 8;
}
int BIT_6_R(uint8_t r){
    uint8_t value = (r == MEMHL) ? get_mem_hl() : *(regs[r]);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !((64 & value) >> 6));
    return r == MEMHL ? 12 : 8;
}
int BIT_7_R(uint8_t r){
    uint8_t value = (r == MEMHL) ? get_mem_hl() : *(regs[r]);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !((128 & value) >> 7));
    return r == MEMHL ? 12 : 8;
}

int SET_0_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() | 1);
    else *(regs[r]) = *(regs[r]) | 1;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 1
int SET_1_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() | 2);
    else *(regs[r]) = *(regs[r]) | 2;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 1
int SET_2_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() | 4);
    else *(regs[r]) = *(regs[r]) | 4;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 1
int SET_3_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() | 8);
    else *(regs[r]) = *(regs[r]) | 8;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 1
int SET_4_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() | 16);
    else *(regs[r]) = *(regs[r]) | 16;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 1
int SET_5_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() | 32);
    else *(regs[r]) = *(regs[r]) | 32;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 1
int SET_6_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() | 64);
    else *(regs[r]) = *(regs[r]) | 64;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 1
int SET_7_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() | 128);
    else *(regs[r]) = *(regs[r]) | 128;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 1


int RES_0_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() & ~1);
    else *(regs[r]) = *(regs[r]) & ~1;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 0
int RES_1_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() & ~2);
    else *(regs[r]) = *(regs[r]) & ~2;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 0
int RES_2_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() & ~4);
    else *(regs[r]) = *(regs[r]) & ~4;
    return r == MEMHL ? 16 : 8;
}   // Sets Bit N of register to 0
int RES_3_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() & ~8);
    else *(regs[r]) = *(regs[r]) & ~8;
    return r == MEMHL ? 16 : 8;
}       
int RES_4_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() & ~16);
    else *(regs[r]) = *(regs[r]) & ~16;
    return r == MEMHL ? 16 : 8;
}
int RES_5_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() & ~32);
    else *(regs[r]) = *(regs[r]) & ~32;
    return r == MEMHL ? 16 : 8;
}
int RES_6_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() & ~64);
    else *(regs[r]) = *(regs[r]) & ~64;
    return r == MEMHL ? 16 : 8;
}
int RES_7_R(uint8_t r){
    if (r == MEMHL) set_mem_hl(get_mem_hl() & ~128);
    else *(regs[r]) = *(regs[r]) & ~128;
    return r == MEMHL ? 16 : 8;
}


// CPU Control Commands
int CCF(uint8_t t){
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, get_flag(cy) ^ 1);
    return 4;
}                                  // Flips CY Flag
int SCF(uint8_t t){
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 1);
    return 4;
}                                  // Sets CY Flag to 1
int NOP(uint8_t t){
    return 4;
}                                  // No Operation
int HALT(uint8_t t){
    if (IME) halt = HALTIME;
    else {
        if ((fetch(0xff0f) & fetch(0xffff)) == 0) {
            halt = HALTNOIMENOINT;
        } else { // HALT BUG
            printf("HALT BUG ME NO LIKE\n\n\n\n");
        }
    }
    return 4;
}                                 // HAlt until Interrupt (low power)
int STOP(uint8_t t){
    // STILL MUST DO
    return 4;
}                                 // Low power standby mode (very low power)
int DI(uint8_t t){
    IME = false;
    return 4;
}                                   // disable interrupts
int EI(uint8_t t){
    IME = true;
    return 4;
}                                   // Enable interrupts

// Jump commands
int JP_NN(uint8_t t){
    PC = fetchOP16();
    return 16;
}  // PC = 16 bit value1+2
int JP_HL(uint8_t t){
    PC = get_hl();
    return 4;
}                                // PC = HL 
int JP_FT_NN(uint8_t fl){
    uint16_t value = fetchOP16();
    if (get_flag(fl)) {
        PC = value;
        return 16;
    }
    return 12;
}
int JP_FF_NN(uint8_t fl){
    uint16_t value = fetchOP16();
    if (!get_flag(fl)) {
        PC = value;
        return 16;
    }
    return 12;
}
int JR_DD(uint8_t t){
    PC += (int8_t)fetchOP();    // minus 2 because the instruction itself is 2 bytes
    return 12;
}                   // PC += value
int JR_FT_DD(uint8_t fl){
    int8_t value = fetchOP();
    if (get_flag(fl)) {
        PC += value;
        return 12;
    }
    return 8;
}   // PC += value if FLAG is set
int JR_FF_DD(uint8_t fl){
    int8_t value = fetchOP();
    if (!get_flag(fl)) {
        PC += value;
        return 12;
    }
    return 8;
}
int CALL_NN(uint8_t t){
    uint16_t value = fetchOP16();
    post(SP-1, (uint8_t) (PC>>8));
    post(SP-2, (uint8_t) PC);
    SP-=2;
    PC = value;
    return 24;
}// PC = 16 bit value1+2, SP-=2, (SP) = PC
int CALL_FT_NN(uint8_t fl){
    uint16_t value = fetchOP16();
    if (get_flag(fl)) {
        post(SP-1, (uint8_t) (PC>>8));
        post(SP-2, (uint8_t) PC);
        SP-=2;
        PC = value;
        return 24;
    }
    return 12;
}// PC = 16 bit value1+2, SP-=2, (SP) = PC IF Flag is set
int CALL_FF_NN(uint8_t fl){
    uint16_t value = fetchOP16();
    if (!get_flag(fl)) {
        post(SP-1, (uint8_t) (PC>>8));
        post(SP-2, (uint8_t) PC);
        SP-=2;
        PC = value;
        return 24;
    }
    return 12;
}
int RET(uint8_t t){
    PC = x8_x16(fetch(SP+1), fetch(SP));
    SP += 2;
    return 16;
}                                  // PC=(SP), SP+=2
int RET_FT(uint8_t fl){
    if (get_flag(fl)) {
        PC = x8_x16(fetch(SP+1), fetch(SP));
        SP += 2;
        return 20;
    }
    return 8;
}                    // PC=(SP), SP+=2 IF Flag is set
int RET_FF(uint8_t fl){
    if (!get_flag(fl)) {
        PC = x8_x16(fetch(SP+1), fetch(SP));
        SP += 2;
        return 20;
    }
    return 8;
}
int RETI(uint8_t t){
    PC = x8_x16(fetch(SP+1), fetch(SP));
    SP += 2;
    IME = true;
    return 16;
}                                 // PC=(SP), SP+=2, enable interrupts
int RST(uint8_t value){
    int v[8] = {0,0x8,0x10,0x18,0x20,0x28,0x30,0x38};
    post(SP-1, (uint8_t) (PC>>8));
    post(SP-2,(uint8_t) PC);
    SP -= 2;
    PC = v[value];
    return 16;
}                     // Like CALL, but only 1 byte, and calls 00,08,10,18,20,28,30 or 38


#include "cpu.h"
#include <stdint.h>
#include <stdio.h>

/*int set_reg(char r, uint8_t val) {
    switch (r) {
        case 'a':
            a = val;
            break;
        case 'b':
            b = val;
            break;
        case 'c':
            c = val;
            break;
        case 'd':
            d = val;
            break;
        case 'e':
            e = val;
            break;
        case 'f':
            f = val;
            break;
        case 'l':
            l = val;
            break;
        case 'h':
            h = val;
            break;
        default:
            return -1;
            break;
    }
    return 0;
}*/
/*
uint8_t get_reg(char r) {
    switch(r) {
        case 'a':
            return a;
            break;
        case 'b':
            return b;
            break;
        case 'c':
            return c;
            break;
        case 'd':
            return d;
            break;
        case 'e':
            return e;
            break;
        case 'f':
            return f;
            break;
        case 'l':
            return l;
            break;
        case 'h':
            return h;
            break;
        default:
            return -1;
            break;
    }
}*/

/*void connect_bus(Bus *b){
    bus = b;
}*/

// Fetching
uint8_t fetch(uint16_t index){
    if (index == 0xff44) return 0x90;
    return data[index];
}


uint8_t fetchOP(){
    PC++;
    //printf("%x ", data[PC-1]);
    return data[PC-1];
}

void set_flag(int flag, int value) {
    if (value == 0) {
        f = f & ~(1 << flag);
    } else {
        f = f | (1 << flag);
    }
}

int get_flag(int flag) {
    return ((f >> flag) & 1);
}

void post(uint16_t address, uint8_t value) {
    if (address == 0xff02 && (value & 0x80)) serial_interrupt = 8;
    if (address == 0xff04) data[address] = 0;
    data[address] = value;
}

uint8_t execute_cb_op(uint8_t next_op) {
        switch(next_op) {
                case 0x00:
                    return RLC_R(&b);
                    break;
                case 0x01:
                    return RLC_R(&c);
                    break;
                case 0x02:
                    return RLC_R(&d);
                    break;
                case 0x03:
                    return RLC_R(&e);
                    break;
                case 0x04:
                    return RLC_R(&h);
                    break;
                case 0x05:
                    return RLC_R(&l);
                    break;
                case 0x06:
                    return RLC_HL();
                    break;
                case 0x07:
                    return RLC_R(&a);
                    break;
                case 0x08:
                    return RRC_R(&b);
                    break;
                case 0x09:
                    return RRC_R(&c);
                    break;
                case 0x0a:
                    return RRC_R(&d);
                    break;
                case 0x0b:
                    return RRC_R(&e);
                    break;
                case 0x0c:
                    return RRC_R(&h);
                    break;
                case 0x0d:
                    return RRC_R(&l);
                    break;
                case 0x0e:
                    return RRC_HL();
                    break;
                case 0x0f:
                    return RRC_R(&a);
                    break;
                case 0x10:
                    return RL_R(&b);
                    break;
                case 0x11:
                    return RL_R(&c);
                    break;
                case 0x12:
                    return RL_R(&d);
                    break;
                case 0x13:
                    return RL_R(&e);
                    break;
                case 0x14:
                    return RL_R(&h);
                    break;
                case 0x15:
                    return RL_R(&l);
                    break;
                case 0x16:
                    return RL_HL();
                    break;
                case 0x17:
                    return RL_R(&a);
                    break;
                case 0x18:
                    return RR_R(&b);
                    break;
                case 0x19:
                    return RR_R(&c);
                    break;
                case 0x1a:
                    return RR_R(&d);
                    break;
                case 0x1b:
                    return RR_R(&e);
                    break;
                case 0x1c:
                    return RR_R(&h);
                    break;
                case 0x1d:
                    return RR_R(&l);
                    break;
                case 0x1e:
                    return RR_HL();
                    break;
                case 0x1f:
                    return RR_R(&a);
                    break;
                case 0x20:
                    return SLA_R(&b);
                    break;
                case 0x21:
                    return SLA_R(&c);
                    break;
                case 0x22:
                    return SLA_R(&d);
                    break;
                case 0x23:
                    return SLA_R(&e);
                    break;
                case 0x24:
                    return SLA_R(&h);
                    break;
                case 0x25:
                    return SLA_R(&l);
                    break;
                case 0x26:
                    return SLA_HL();
                    break;
                case 0x27:
                    return SLA_R(&a);
                    break;
                case 0x28:
                    return SRA_R(&b);
                    break;
                case 0x29:
                    return SRA_R(&c);
                    break;
                case 0x2a:
                    return SRA_R(&d);
                    break;
                case 0x2b:
                    return SRA_R(&e);
                    break;
                case 0x2c:
                    return SRA_R(&h);
                    break;
                case 0x2d:
                    return SRA_R(&l);
                    break;
                case 0x2e:
                    return SRA_HL();
                    break;
                case 0x2f:
                    return SRA_R(&a);
                    break;
                case 0x30:
                    return SWAP_R(&b);
                    break;
                case 0x31:
                    return SWAP_R(&c);
                    break;
                case 0x32:
                    return SWAP_R(&d);
                    break;
                case 0x33:
                    return SWAP_R(&e);
                    break;
                case 0x34:
                    return SWAP_R(&h);
                    break;
                case 0x35:
                    return SWAP_R(&l);
                    break;
                case 0x36:
                    return SWAP_HL();
                    break;
                case 0x37:
                    return SWAP_R(&a);
                    break;
                case 0x38:
                    return SRL_R(&b);
                    break;
                case 0x39:
                    return SRL_R(&c);
                    break;
                case 0x3a:
                    return SRL_R(&d);
                    break;
                case 0x3b:
                    return SRL_R(&e);
                    break;
                case 0x3c:
                    return SRL_R(&h);
                    break;
                case 0x3d:
                    return SRL_R(&l);
                    break;
                case 0x3e:
                    return SRL_HL();
                    break;
                case 0x3f:
                    return SRL_R(&a);
                    break;
                case 0x40:
                    return BIT_N_R(0, &b);
                    break;
                case 0x41:
                    return BIT_N_R(0, &c);
                    break;
                case 0x42:
                    return BIT_N_R(0, &d);
                    break;
                case 0x43:
                    return BIT_N_R(0, &e);
                    break;
                case 0x44:
                    return BIT_N_R(0, &h);
                    break;
                case 0x45:
                    return BIT_N_R(0, &l);
                    break;
                case 0x46:
                    return BIT_N_HL(0);
                    break;
                case 0x47:
                    return BIT_N_R(0,&a);
                case 0x48:
                    return BIT_N_R(1, &b);
                    break;
                case 0x49:
                    return BIT_N_R(1, &c);
                    break;
                case 0x4a:
                    return BIT_N_R(1, &d);
                    break;
                case 0x4b:
                    return BIT_N_R(1, &e);
                    break;
                case 0x4c:
                    return BIT_N_R(1, &h);
                    break;
                case 0x4d:
                    return BIT_N_R(1, &l);
                    break;
                case 0x4e:
                    return BIT_N_HL(1);
                    break;
                case 0x4f:
                    return BIT_N_R(1, &a);
                    break;
                case 0x50:
                    return BIT_N_R(2, &b);
                    break;
                case 0x51:
                    return BIT_N_R(2, &c);
                    break;
                case 0x52:
                    return BIT_N_R(2, &d);
                    break;
                case 0x53:
                    return BIT_N_R(2, &e);
                    break;
                case 0x54:
                    return BIT_N_R(2, &h);
                    break;
                case 0x55:
                    return BIT_N_R(2, &l);
                    break;
                case 0x56:
                    return BIT_N_HL(2);
                    break;
                case 0x57:
                    return BIT_N_R(2, &a);
                    break;
                case 0x58:
                    return BIT_N_R(3, &b);
                    break;
                case 0x59:
                    return BIT_N_R(3, &c);
                    break;
                case 0x5a:
                    return BIT_N_R(3, &d);
                    break;
                case 0x5b:
                    return BIT_N_R(3, &e);
                    break;
                case 0x5c:
                    return BIT_N_R(3, &h);
                    break;
                case 0x5d:
                    return BIT_N_R(3, &l);
                    break;
                case 0x5e:
                    return BIT_N_HL(3);
                    break;
                case 0x5f:
                    return BIT_N_R(3, &a);
                    break;
                case 0x60:
                    return BIT_N_R(4, &b);
                    break;
                case 0x61:
                    return BIT_N_R(4, &c);
                    break;
                case 0x62:
                    return BIT_N_R(4, &d);
                    break;
                case 0x63:
                    return BIT_N_R(4, &e);
                    break;
                case 0x64:
                    return BIT_N_R(4, &h);
                    break;
                case 0x65:
                    return BIT_N_R(4, &l);
                    break;
                case 0x66:
                    return BIT_N_HL(4);
                    break;
                case 0x67:
                    return BIT_N_R(4, &a);
                    break;
                case 0x68:
                    return BIT_N_R(5, &b);
                    break;
                case 0x69:
                    return BIT_N_R(5, &c);
                    break;
                case 0x6a:
                    return BIT_N_R(5, &d);
                    break;
                case 0x6b:
                    return BIT_N_R(5, &e);
                    break;
                case 0x6c:
                    return BIT_N_R(5, &h);
                    break;
                case 0x6d:
                    return BIT_N_R(5, &l);
                    break;
                case 0x6e:
                    return BIT_N_HL(5);
                    break;
                case 0x6f:
                    return BIT_N_R(5, &a);
                    break;
                case 0x70:
                    return BIT_N_R(6, &b);
                    break;
                case 0x71:
                    return BIT_N_R(6, &c);
                    break;
                case 0x72:
                    return BIT_N_R(6, &d);
                    break;
                case 0x73:
                    return BIT_N_R(6, &e);
                    break;
                case 0x74:
                    return BIT_N_R(6, &h);
                    break;
                case 0x75:
                    return BIT_N_R(6, &l);
                    break;
                case 0x76:
                    return BIT_N_HL(6);
                    break;
                case 0x77:
                    return BIT_N_R(6,&a);
                    break;
                case 0x78:
                    return BIT_N_R(7, &b);
                    break;
                case 0x79:
                    return BIT_N_R(7, &c);
                    break;
                case 0x7a:
                    return BIT_N_R(7, &d);
                    break;
                case 0x7b:
                    return BIT_N_R(7, &e);
                    break;
                case 0x7c:
                    return BIT_N_R(7, &h);
                    break;
                case 0x7d:
                    return BIT_N_R(7, &l);
                    break;
                case 0x7e:
                    return BIT_N_HL(7);
                    break;
                case 0x7f:
                    return BIT_N_R(7,&a);
                    break;
                case 0x80:
                    return RES_N_R(0, &b);
                    break;
                case 0x81:
                    return RES_N_R(0, &c);
                    break;
                case 0x82:
                    return RES_N_R(0, &d);
                    break;
                case 0x83:
                    return RES_N_R(0, &e);
                    break;
                case 0x84:
                    return RES_N_R(0, &h);
                    break;
                case 0x85:
                    return RES_N_R(0, &l);
                    break;
                case 0x86:
                    return RES_N_HL(0);
                    break;
                case 0x87:
                    return RES_N_R(0,&a);
                case 0x88:
                    return RES_N_R(1, &b);
                    break;
                case 0x89:
                    return RES_N_R(1, &c);
                    break;
                case 0x8a:
                    return RES_N_R(1, &d);
                    break;
                case 0x8b:
                    return RES_N_R(1, &e);
                    break;
                case 0x8c:
                    return RES_N_R(1, &h);
                    break;
                case 0x8d:
                    return RES_N_R(1, &l);
                    break;
                case 0x8e:
                    return RES_N_HL(1);
                    break;
                case 0x8f:
                    return RES_N_R(1, &a);
                    break;
                case 0x90:
                    return RES_N_R(2, &b);
                    break;
                case 0x91:
                    return RES_N_R(2, &c);
                    break;
                case 0x92:
                    return RES_N_R(2, &d);
                    break;
                case 0x93:
                    return RES_N_R(2, &e);
                    break;
                case 0x94:
                    return RES_N_R(2, &h);
                    break;
                case 0x95:
                    return RES_N_R(2, &l);
                    break;
                case 0x96:
                    return RES_N_HL(2);
                    break;
                case 0x97:
                    return RES_N_R(2, &a);
                    break;
                case 0x98:
                    return RES_N_R(3, &b);
                    break;
                case 0x99:
                    return RES_N_R(3, &c);
                    break;
                case 0x9a:
                    return RES_N_R(3, &d);
                    break;
                case 0x9b:
                    return RES_N_R(3, &e);
                    break;
                case 0x9c:
                    return RES_N_R(3, &h);
                    break;
                case 0x9d:
                    return RES_N_R(3, &l);
                    break;
                case 0x9e:
                    return RES_N_HL(3);
                    break;
                case 0x9f:
                    return RES_N_R(3, &a);
                    break;
                case 0xa0:
                    return RES_N_R(4, &b);
                    break;
                case 0xa1:
                    return RES_N_R(4, &c);
                    break;
                case 0xa2:
                    return RES_N_R(4, &d);
                    break;
                case 0xa3:
                    return RES_N_R(4, &e);
                    break;
                case 0xa4:
                    return RES_N_R(4, &h);
                    break;
                case 0xa5:
                    return RES_N_R(4, &l);
                    break;
                case 0xa6:
                    return RES_N_HL(4);
                    break;
                case 0xa7:
                    return RES_N_R(4, &a);
                    break;
                case 0xa8:
                    return RES_N_R(5, &b);
                    break;
                case 0xa9:
                    return RES_N_R(5, &c);
                    break;
                case 0xaa:
                    return RES_N_R(5, &d);
                    break;
                case 0xab:
                    return RES_N_R(5, &e);
                    break;
                case 0xac:
                    return RES_N_R(5, &h);
                    break;
                case 0xad:
                    return RES_N_R(5, &l);
                    break;
                case 0xae:
                    return RES_N_HL(5);
                    break;
                case 0xaf:
                    return RES_N_R(5, &a);
                    break;
                case 0xb0:
                    return RES_N_R(6, &b);
                    break;
                case 0xb1:
                    return RES_N_R(6, &c);
                    break;
                case 0xb2:
                    return RES_N_R(6, &d);
                    break;
                case 0xb3:
                    return RES_N_R(6, &e);
                    break;
                case 0xb4:
                    return RES_N_R(6, &h);
                    break;
                case 0xb5:
                    return RES_N_R(6, &l);
                    break;
                case 0xb6:
                    return RES_N_HL(6);
                    break;
                case 0xb7:
                    return RES_N_R(6,&a);
                    break;
                case 0xb8:
                    return RES_N_R(7, &b);
                    break;
                case 0xb9:
                    return RES_N_R(7, &c);
                    break;
                case 0xba:
                    return RES_N_R(7, &d);
                    break;
                case 0xbb:
                    return RES_N_R(7, &e);
                    break;
                case 0xbc:
                    return RES_N_R(7, &h);
                    break;
                case 0xbd:
                    return RES_N_R(7, &l);
                    break;
                case 0xbe:
                    return RES_N_HL(7);
                    break;
                case 0xbf:
                    return RES_N_R(7,&a);
                    break;
                case 0xc0:
                    return SET_N_R(0, &b);
                    break;
                case 0xc1:
                    return SET_N_R(0, &c);
                    break;
                case 0xc2:
                    return SET_N_R(0, &d);
                    break;
                case 0xc3:
                    return SET_N_R(0, &e);
                    break;
                case 0xc4:
                    return SET_N_R(0, &h);
                    break;
                case 0xc5:
                    return SET_N_R(0, &l);
                    break;
                case 0xc6:
                    return SET_N_HL(0);
                    break;
                case 0xc7:
                    return SET_N_R(0,&a);
                case 0xc8:
                    return SET_N_R(1, &b);
                    break;
                case 0xc9:
                    return SET_N_R(1, &c);
                    break;
                case 0xca:
                    return SET_N_R(1, &d);
                    break;
                case 0xcb:
                    return SET_N_R(1, &e);
                    break;
                case 0xcc:
                    return SET_N_R(1, &h);
                    break;
                case 0xcd:
                    return SET_N_R(1, &l);
                    break;
                case 0xce:
                    return SET_N_HL(1);
                    break;
                case 0xcf:
                    return SET_N_R(1, &a);
                    break;
                case 0xd0:
                    return SET_N_R(2, &b);
                    break;
                case 0xd1:
                    return SET_N_R(2, &c);
                    break;
                case 0xd2:
                    return SET_N_R(2, &d);
                    break;
                case 0xd3:
                    return SET_N_R(2, &e);
                    break;
                case 0xd4:
                    return SET_N_R(2, &h);
                    break;
                case 0xd5:
                    return SET_N_R(2, &l);
                    break;
                case 0xd6:
                    return SET_N_HL(2);
                    break;
                case 0xd7:
                    return SET_N_R(2, &a);
                    break;
                case 0xd8:
                    return SET_N_R(3, &b);
                    break;
                case 0xd9:
                    return SET_N_R(3, &c);
                    break;
                case 0xda:
                    return SET_N_R(3, &d);
                    break;
                case 0xdb:
                    return SET_N_R(3, &e);
                    break;
                case 0xdc:
                    return SET_N_R(3, &h);
                    break;
                case 0xdd:
                    return SET_N_R(3, &l);
                    break;
                case 0xde:
                    return SET_N_HL(3);
                    break;
                case 0xdf:
                    return SET_N_R(3, &a);
                    break;
                case 0xe0:
                    return SET_N_R(4, &b);
                    break;
                case 0xe1:
                    return SET_N_R(4, &c);
                    break;
                case 0xe2:
                    return SET_N_R(4, &d);
                    break;
                case 0xe3:
                    return SET_N_R(4, &e);
                    break;
                case 0xe4:
                    return SET_N_R(4, &h);
                    break;
                case 0xe5:
                    return SET_N_R(4, &l);
                    break;
                case 0xe6:
                    return SET_N_HL(4);
                    break;
                case 0xe7:
                    return SET_N_R(4, &a);
                    break;
                case 0xe8:
                    return SET_N_R(5, &b);
                    break;
                case 0xe9:
                    return SET_N_R(5, &c);
                    break;
                case 0xea:
                    return SET_N_R(5, &d);
                    break;
                case 0xeb:
                    return SET_N_R(5, &e);
                    break;
                case 0xec:
                    return SET_N_R(5, &h);
                    break;
                case 0xed:
                    return SET_N_R(5, &l);
                    break;
                case 0xee:
                    return SET_N_HL(5);
                    break;
                case 0xef:
                    return SET_N_R(5, &a);
                    break;
                case 0xf0:
                    return SET_N_R(6, &b);
                    break;
                case 0xf1:
                    return SET_N_R(6, &c);
                    break;
                case 0xf2:
                    return SET_N_R(6, &d);
                    break;
                case 0xf3:
                    return SET_N_R(6, &e);
                    break;
                case 0xf4:
                    return SET_N_R(6, &h);
                    break;
                case 0xf5:
                    return SET_N_R(6, &l);
                    break;
                case 0xf6:
                    return SET_N_HL(6);
                    break;
                case 0xf7:
                    return SET_N_R(6,&a);
                    break;
                case 0xf8:
                    return SET_N_R(7, &b);
                    break;
                case 0xf9:
                    return SET_N_R(7, &c);
                    break;
                case 0xfa:
                    return SET_N_R(7, &d);
                    break;
                case 0xfb:
                    return SET_N_R(7, &e);
                    break;
                case 0xfc:
                    return SET_N_R(7, &h);
                    break;
                case 0xfd:
                    return SET_N_R(7, &l);
                    break;
                case 0xfe:
                    return SET_N_HL(7);
                    break;
                case 0xff:
                    return SET_N_R(7,&a);
                    break;
            }
    return -1;
}

int execute_op() {
    //printf("\n%x", PC);
    uint8_t op = fetchOP();
    uint8_t next_op;        // for cb
    switch (op) {
        case 0x00:
            return NOP();
            break;
        case 0x01:
            return LD_RR_NN(&b,&c,fetchOP(), fetchOP());
            break;
        case 0x02:
            return LD_BC_A();
            break;
        case 0x03:
            return INC_RR(&b,&c);
            break;
        case 0x04:
            return INC_R(&b);
            break;
        case 0x05:
            return DEC_R(&b);
            break;
        case 0x06:
            return LD_R_N(&b, fetchOP());
            break;
        case 0x07:
            return RLCA();
            break;
        case 0x08:
            return LD_NN_SP(fetchOP(), fetchOP());
            break;
        case 0x09:
            return ADD_HL_RR(&b,&c);
            break;
        case 0x0a:
            return LD_A_BC();
            break;
        case 0x0b:
            return DEC_RR(&b,&c);
            break;
        case 0x0c:
            return INC_R(&c);
            break;
        case 0x0d:
            return DEC_R(&c);
            break;
        case 0x0e:
            return LD_R_N(&c, fetchOP());
            break;
        case 0x0f:
            return RRCA();
            break;
        case 0x10:
            return STOP();
            break;
        case 0x11:
            return LD_RR_NN(&d, &e, fetchOP(), fetchOP());
            break;
        case 0x12:
            return LD_DE_A();
            break;
        case 0x13:
            return INC_RR(&d, &e);
            break;
        case 0x14:
            return INC_R(&d);
            break;
        case 0x15:
            return DEC_R(&d);
            break;
        case 0x16:
            return LD_R_N(&d, fetchOP());
            break;
        case 0x17:
            return RLA();
            break;
        case 0x18:
            return JR_DD(fetchOP());
            break;
        case 0x19:
            return ADD_HL_RR(&d, &e);
            break;
        case 0x1a:
            return LD_A_DE();
            break;
        case 0x1b:
            return DEC_RR(&d, &e);
            break;
        case 0x1c:
            return INC_R(&e);
            break;
        case 0x1d:
            return DEC_R(&e);
            break;
        case 0x1e:
            return LD_R_N(&e, fetchOP());
            break;
        case 0x1f:
            return RCA();
            break;
        case 0x20:
            return JR_F_DD(fetchOP(), !(bool)get_flag(z));
            break;
        case 0x21:
            return LD_RR_NN(&h, &l, fetchOP(), fetchOP());
            break;
        case 0x22:
            return LDI_HL_A();
            break;
        case 0x23:
            return INC_RR(&h, &l);
            break;
        case 0x24:
            return INC_R(&h);
            break;
        case 0x25:
            return DEC_R(&h);
            break;
        case 0x26:
            return LD_R_N(&h, fetchOP());
            break;
        case 0x27:
            return DAA();
            break;
        case 0x28:
            return JR_F_DD(fetchOP(), (bool) get_flag(z));
            break;
        case 0x29:
            return ADD_HL_RR(&h,&l);
            break;
        case 0x2a:
            return LDI_A_HL();
            break;
        case 0x2b:
            return DEC_RR(&h,&l);
            break;
        case 0x2c:
            return INC_R(&l);
            break;
        case 0x2d:
            return DEC_R(&l);
            break;
        case 0x2e:
            return LD_R_N(&l, fetchOP());
            break;
        case 0x2f:
            return CPL();
            break;
        case 0x30:
            return JR_F_DD(fetchOP(), !(bool)get_flag(cy));
            break;
        case 0x31:
            return LD_SP_NN(fetchOP(), fetchOP());
            break;
        case 0x32:
            return LDD_HL_A();
            break;
        case 0x33:
            return INC_SP();
            break;
        case 0x34:
            return INC_HL();
            break;
        case 0x35:
            return DEC_HL();
            break;
        case 0x36:
            return LD_HL_N(fetchOP());
            break;
        case 0x37:
            return SCF();
            break;
        case 0x38:
            return JR_F_DD(fetchOP(), (bool) get_flag(cy));
            break;
        case 0x39:
            return ADD_HL_SP();
            break;
        case 0x3a:
            return LDD_A_HL();
            break;
        case 0x3b:
            return DEC_SP();
            break;
        case 0x3c:
            return INC_R(&a);
            break;
        case 0x3d:
            return DEC_R(&a);
            break;
        case 0x3e:
            return LD_R_N(&a, fetchOP());
            break;
        case 0x3f:
            return CCF();
            break;
        case 0x40:
            return LD_R_R(&b, &b);
            break;
        case 0x41:
            return LD_R_R(&b, &c);
            break;
        case 0x42:
            return LD_R_R(&b, &d);
            break;
        case 0x43:
            return LD_R_R(&b, &e);
            break;
        case 0x44:
            return LD_R_R(&b, &h);
            break;
        case 0x45:
            return LD_R_R(&b, &l);
            break;
        case 0x46:
            return LD_R_HL(&b);
            break;
        case 0x47:
            return LD_R_R(&b, &a);
            break;
        case 0x48:
            return LD_R_R(&c, &b);
            break;
        case 0x49:
            return LD_R_R(&c, &c);
            break;
        case 0x4a:
            return LD_R_R(&c, &d);
            break;
        case 0x4b:
            return LD_R_R(&c, &e);
            break;
        case 0x4c:
            return LD_R_R(&c, &h);
            break;
        case 0x4d:
            return LD_R_R(&c, &l);
            break;
        case 0x4e:
            return LD_R_HL(&c);
            break;
        case 0x4f:
            return LD_R_R(&c, &a);
            break;
        case 0x50:
            return LD_R_R(&d, &b);
            break;
        case 0x51:
            return LD_R_R(&d, &c);
            break;
        case 0x52:
            return LD_R_R(&d, &d);
            break;
        case 0x53:
            return LD_R_R(&d, &e);
            break;
        case 0x54:
            return LD_R_R(&d, &h);
            break;
        case 0x55:
            return LD_R_R(&d, &l);
            break;
        case 0x56:
            return LD_R_HL(&d);
            break;
        case 0x57:
            return LD_R_R(&d, &a);
            break;
        case 0x58:
            return LD_R_R(&e, &b);
            break;
        case 0x59:
            return LD_R_R(&e, &c);
            break;
        case 0x5a:
            return LD_R_R(&e, &d);
            break;
        case 0x5b:
            return LD_R_R(&e, &e);
            break;
        case 0x5c:
            return LD_R_R(&e, &h);
            break;
        case 0x5d:
            return LD_R_R(&e, &l);
            break;
        case 0x5e:
            return LD_R_HL(&e);
            break;
        case 0x5f:
            return LD_R_R(&e, &a);
            break;
        case 0x60:
            return LD_R_R(&h, &b);
            break;
        case 0x61:
            return LD_R_R(&h, &c);
            break;
        case 0x62:
            return LD_R_R(&h, &d);
            break;
        case 0x63:
            return LD_R_R(&h, &e);
            break;
        case 0x64:
            return LD_R_R(&h, &h);
            break;
        case 0x65:
            return LD_R_R(&h, &l);
            break;
        case 0x66:
            return LD_R_HL(&h);
            break;
        case 0x67:
            return LD_R_R(&h, &a);
            break;
        case 0x68:
            return LD_R_R(&l, &b);
            break;
        case 0x69:
            return LD_R_R(&l, &c);
            break;
        case 0x6a:
            return LD_R_R(&l, &d);
            break;
        case 0x6b:
            return LD_R_R(&l, &e);
            break;
        case 0x6c:
            return LD_R_R(&l, &h);
            break;
        case 0x6d:
            return LD_R_R(&l, &l);
            break;
        case 0x6e:
            return LD_R_HL(&l);
            break;
        case 0x6f:
            return LD_R_R(&l, &a);
            break;
        case 0x70:
            return LD_HL_R(&b);
            break;
        case 0x71:
            return LD_HL_R(&c);
            break;
        case 0x72:
            return LD_HL_R(&d);
            break;
        case 0x73:
            return LD_HL_R(&e);
            break;
        case 0x74:
            return LD_HL_R(&h);
            break;
        case 0x75:
            return LD_HL_R(&l);
            break;
        case 0x76:
            return HALT();
            break;
        case 0x77:
            return LD_HL_R(&a);
            break;
        case 0x78:
            return LD_R_R(&a, &b);
            break;
        case 0x79:
            return LD_R_R(&a, &c);
            break;
        case 0x7a:
            return LD_R_R(&a, &d);
            break;
        case 0x7b:
            return LD_R_R(&a, &e);
            break;
        case 0x7c:
            return LD_R_R(&a, &h);
            break;
        case 0x7d:
            return LD_R_R(&a, &l);
            break;
        case 0x7e:
            return LD_R_HL(&a);
            break;
        case 0x7f:
            return LD_R_R(&a, &a);
            break;
        case 0x80:
            return ADD_A_R(&b);
            break;
        case 0x81:
            return ADD_A_R(&c);
            break;
        case 0x82:
            return ADD_A_R(&d);
            break;
        case 0x83:
            return ADD_A_R(&e);
            break;
        case 0x84:
            return ADD_A_R(&h);
            break;
        case 0x85:
            return ADD_A_R(&l);
            break;
        case 0x86:
            return ADD_A_HL();
            break;
        case 0x87:
            return ADD_A_R(&a);
            break;
        case 0x88:
            return ADC_A_R(&b);
            break;
        case 0x89:
            return ADC_A_R(&c);
            break;
        case 0x8a:
            return ADC_A_R(&d);
            break;
        case 0x8b:
            return ADC_A_R(&e);
            break;
        case 0x8c:
            return ADC_A_R(&h);
            break;
        case 0x8d:
            return ADC_A_R(&l);
            break;
        case 0x8e:
            return ADC_A_HL();
            break;
        case 0x8f:
            return ADC_A_R(&a);
            break;
        case 0x90:
            return SUB_A_R(&b);
            break;
        case 0x91:
            return SUB_A_R(&c);
            break;
        case 0x92:
            return SUB_A_R(&d);
            break;
        case 0x93:
            return SUB_A_R(&e);
            break;
        case 0x94:
            return SUB_A_R(&h);
            break;
        case 0x95:
            return SUB_A_R(&l);
            break;
        case 0x96:
            return SUB_A_HL();
            break;
        case 0x97:
            return SUB_A_R(&a);
            break;
        case 0x98:
            return SBC_A_R(&b);
            break;
        case 0x99:
            return SBC_A_R(&c);
            break;
        case 0x9a:
            return SBC_A_R(&d);
            break;
        case 0x9b:
            return SBC_A_R(&e);
            break;
        case 0x9c:
            return SBC_A_R(&h);
            break;
        case 0x9d:
            return SBC_A_R(&l);
            break;
        case 0x9e:
            return SBC_A_HL();
            break;
        case 0x9f:
            return SBC_A_R(&a);
            break;
        case 0xa0:
            return AND_A_R(&b);
            break;
        case 0xa1:
            return AND_A_R(&c);
            break;
        case 0xa2:
            return AND_A_R(&d);
            break;
        case 0xa3:
            return AND_A_R(&e);
            break;
        case 0xa4:
            return AND_A_R(&h);
            break;
        case 0xa5:
            return AND_A_R(&l);
            break;
        case 0xa6:
            return AND_A_HL();
            break;
        case 0xa7:
            return AND_A_R(&a);
            break;
        case 0xa8:
            return XOR_A_R(&b);
            break;
        case 0xa9:
            return XOR_A_R(&c);
            break;
        case 0xaa:
            return XOR_A_R(&d);
            break;
        case 0xab:
            return XOR_A_R(&e);
            break;
        case 0xac:
            return XOR_A_R(&h);
            break;
        case 0xad:
            return XOR_A_R(&l);
            break;
        case 0xae:
            return XOR_A_HL();
            break;
        case 0xaf:
            return XOR_A_R(&a);
            break;
        case 0xb0:
            return OR_A_R(&b);
            break;
        case 0xb1:
            return OR_A_R(&c);
            break;
        case 0xb2:
            return OR_A_R(&d);
            break;
        case 0xb3:
            return OR_A_R(&e);
            break;
        case 0xb4:
            return OR_A_R(&h);
            break;
        case 0xb5:
            return OR_A_R(&l);
            break;
        case 0xb6:
            return OR_A_HL();
            break;
        case 0xb7:
            return OR_A_R(&a);
            break;
        case 0xb8:
            return CP_A_R(&b);
            break;
        case 0xb9:
            return CP_A_R(&c);
            break;
        case 0xba:
            return CP_A_R(&d);
            break;
        case 0xbb:
            return CP_A_R(&e);
            break;
        case 0xbc:
            return CP_A_R(&h);
            break;
        case 0xbd:
            return CP_A_R(&l);
            break;
        case 0xbe:
            return CP_A_HL();
            break;
        case 0xbf:
            return CP_A_R(&a);
            break;
        case 0xc0:
            return RET_F(!(bool)get_flag(z));
            break;
        case 0xc1:
            return POP_RR(&b, &c);
            break;
        case 0xc2:
            return JP_F_NN(fetchOP(), fetchOP(), !(bool) get_flag(z));
            break;
        case 0xc3:
            return JP_NN(fetchOP(), fetchOP());
            break;
        case 0xc4:
            return CALL_F_NN(fetchOP(), fetchOP(), !(bool) get_flag(z));
            break;
        case 0xc5:
            return PUSH_RR(&b, &c);
            break;
        case 0xc6:
            return ADD_A_N(fetchOP());
            break;
        case 0xc7:
            return RST(0);
            break;
        case 0xc8:
            return RET_F((bool) get_flag(z));
            break;
        case 0xc9:
            return RET();
            break;
        case 0xca:
            return JP_F_NN(fetchOP(), fetchOP(), (bool) get_flag(z));
            break;
        case 0xcb:
            next_op = fetchOP();
            return execute_cb_op(next_op); 
            break;
        case 0xcc:
            return CALL_F_NN(fetchOP(), fetchOP(), (bool) get_flag(z));
            break;
        case 0xcd:
            return CALL_NN(fetchOP(), fetchOP());
            break;
        case 0xce:
            return ADC_A_N(fetchOP());
            break;
        case 0xcf:
            return RST(1);
            break;
        case 0xd0:
            return RET_F(!(bool) get_flag(cy));
            break;
        case 0xd1:
            return POP_RR(&d, &e);
            break;
        case 0xd2:
            return JP_F_NN(fetchOP(), fetchOP(), !(bool) get_flag(cy));
            break;
        case 0xd4:
            return CALL_F_NN(fetchOP(), fetchOP(), !(bool) get_flag(cy));
            break;
        case 0xd5:
            return PUSH_RR(&d, &e);
            break;
        case 0xd6:
            return SUB_A_N(fetchOP());
            break;
        case 0xd7:
            return RST(2);
            break;
        case 0xd8:
            return RET_F((bool) get_flag(cy));
            break;
        case 0xd9:
            return RETI();
            break;
        case 0xda:
            return JP_F_NN(fetchOP(), fetchOP(), (bool) get_flag(cy));
            break;
        case 0xdc:
            return CALL_F_NN(fetchOP(), fetchOP(), (bool) get_flag(cy));
            break;
        case 0xde:
            return SBC_A_N(fetchOP());
            break;
        case 0xdf:
            return RST(3);
            break;
        case 0xe0:
            return LD_ION_A(fetchOP());
            break;
        case 0xe1:
            return POP_RR(&h,&l);
            break;
        case 0xe2:
            return LD_IOC_A();
            break;
        case 0xe5:
            return PUSH_RR(&h, &l);
            break;
        case 0xe6:
            return AND_A_N(fetchOP());
            break;
        case 0xe7:
            return RST(4);
            break;
        case 0xe8:
            return ADD_SP_DD(fetchOP());
            break;
        case 0xe9:
            return JP_HL();
            break;
        case 0xea:
            return LD_NN_A(fetchOP(), fetchOP());
            break;
        case 0xee:
            return XOR_A_N(fetchOP());
            break;
        case 0xef:
            return RST(5);
            break;
        case 0xf0:
            return LD_A_ION(fetchOP());
            break;
        case 0xf1:
            return POP_RR(&a,&f);
            break;
        case 0xf2:
            return LD_A_IOC();
            break;
        case 0xf3:
            return DI();
            break;
        case 0xf5:
            return PUSH_RR(&a,&f);
            break;
        case 0xf6:
            return OR_A_N(fetchOP());
            break;
        case 0xf7:
            return RST(6);
            break;
        case 0xf8:
            return LD_HL_SP_DD(fetchOP());
            break;
        case 0xf9:
            return LD_SP_HL();
            break;
        case 0xfa:
            return LD_A_NN(fetchOP(), fetchOP());
            break;
        case 0xfb:
            return EI();
            break;
        case 0xfe:
            return CP_A_N(fetchOP());
            break;
        case 0xff:
            return RST(7);
            break;
         
    }

    return -1;
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

void interrupt(uint8_t type) {
    //printf("TYPE: %i - IME: %s && IE: %i && IF: %i \t AT:%i\n", type, IME?"true":"false", (fetch(0xffff)), (fetch(0xff0f)), PC);
    if (IME && (fetch(0xffff) >> type) % 2 && (fetch(0xff0f) >> type) % 2) {
        
        // Disable IME and IF Flag
        IME = false;
        post(0xff0f, (uint8_t) fetch(0xff0f) & ~(1 << type));
        
        // PUSH PC to stack
        post(SP-1, (uint8_t) (PC>>8));
        post(SP-2, (uint8_t) PC);
        SP-=2;

        // Call register
        PC = 0x40 + type * 8;

        // Tell the CPU loop that it is interrupting
        // This is needed for the correcoooooooot timings
        is_interrupting = true;
        if (type == SERIAL) {
            printf("%c\n", data[0xff01]);
        }

        //bus->debug_msg = 0x69;
    }
}


// Opcodes:
int LD_R_R(uint8_t* reg1, uint8_t* reg2){
    *reg1 = *reg2;
    return 4;

}  // REG1 = REG2
int LD_R_N(uint8_t* reg, uint8_t value){
    
    *reg = value;
    return 8;
}   // REG = Value
int LD_R_HL(uint8_t* reg){
    *reg = fetch(get_hl());
    return 8;
}                 // REG = *(HL)
int LD_HL_R(uint8_t* reg){
    post(get_hl(), *reg);
    return 8;
}// *(HL) = REG
int LD_HL_N(uint8_t value){
    post(get_hl(), value);
    return 12;
}                // *(HL) = Value
int LD_A_BC(){
    a = fetch(get_bc());
    return 8;
}                             // A = *(BC)
int LD_A_DE(){
    a = fetch(get_de());
    return 8;
}                             // A = *(DE)
int LD_A_NN(uint8_t value1, uint8_t value2){
    a = fetch(x8_x16(value2, value1));
    return 16;
}               // A = *(Value)
int LD_BC_A(){
    post(get_bc(), a);
    return 8;
}                           // *(BC) = A
int LD_DE_A(){
    post(get_de(), a);
    return 8;
}                             // *(DE) = A
int LD_NN_A(uint8_t value1, uint8_t value2){
    post(x8_x16(value2, value1), a);
    return 16;
}                             // *(NN) = A
int LD_A_ION(uint8_t index){
    a = fetch((uint16_t)(0xFF00 + (uint16_t)index));
    return 12;
}               // A = *(FF00+n) 
int LD_ION_A(uint8_t index){
    post(0xFF00 + index, a);
    return 12;
}               // *(FF00+n) = A
int LD_A_IOC(){
    a = fetch(0xFF00 + c);
    return 8;
}                            // A = *(FF00+C)
int LD_IOC_A(){
    post(0xFF00 + c, a);
    return 8;
}                            // *(FF00+c) = A
int LDI_HL_A(){
    post(get_hl(), a);
    set_hl(get_hl() + 0x0001);
    return 8;
}                            // *(HL) = A, HL+=1
int LDI_A_HL(){
    a = fetch(get_hl());
    set_hl(get_hl()+0x0001);
    return 8;
}                            // A = *(HL), HL+=1
int LDD_HL_A(){
    post(get_hl(), a);
    set_hl(get_hl()-1);
    return 8;
}                            // *(HL) = A, HL-=1
int LDD_A_HL(){
    a = fetch(get_hl());
    set_hl(get_hl()-1);
    return 8;
}                            // A = *(HL), HL-=1
        
// 16 Bit Load Commands
int LD_RR_NN(uint8_t *reg1, uint8_t *reg2, uint8_t value1, uint8_t value2){
    *reg2 = value1;
    *reg1 = value2;
    return 12;
}// RR = NN (rr is like BC or DE or HL)
int LD_SP_NN(uint8_t value1, uint8_t value2){
    SP = x8_x16(value2, value1);
    return 12;
} // SP = NN (is merged with op above in pan docs)
int LD_SP_HL(){
    SP = get_hl();
    return 8;
}                                                            // SP = HL
int PUSH_RR(uint8_t *reg1, uint8_t *reg2){
    post(SP-1, *reg1);
    post(SP-2, *reg2);
    SP-=2;
    return 16;
}                                 // SP-=2, *(SP)=rr
int POP_RR(uint8_t *reg1, uint8_t *reg2){
    *reg2 = fetch(SP);
    *reg1 = fetch(SP+1);
    SP+=2;
    return 12;
}                                  // SP+=2. rr=*(SP)

int LD_NN_SP(uint8_t value1, uint8_t value2) {
    post(x8_x16(value2,value1), (uint8_t) SP);
    post(x8_x16(value2,value1) + 1, (uint8_t) (SP >> 8));
    return 20;
}
        
// 8 bit Arithmetic/Logic
int ADD_A_R(uint8_t* reg){
    set_flag(hy, (a & 0xf) + (*reg & 0xf) > 0xf);
    set_flag(cy, a + *reg > 0xff);
    a += *reg;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 4;
}                // Adds register to accumulator
int ADD_A_N(uint8_t value){
    set_flag(hy, (a & 0xf) + (value & 0xf) > 0xf);
    set_flag(cy, a + value > 0xff);
    a += value;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 8;
}               // Adds value to accumulator
int ADD_A_HL(){
    set_flag(hy, (a & 0xf) + (fetch(get_hl()) & 0xf) > 0xf);
    set_flag(cy, a + fetch(get_hl()) > 0xff);
    a += fetch(get_hl());
    set_flag(z, a==0);
    set_flag(n, 0);
    return 8;
}                           // Adds (HL) pointer's value to accumulator
int ADC_A_R(uint8_t* reg) {
    set_flag(hy, (a & 0xf) + (*reg & 0xf) + get_flag(cy)> 0xf);
    bool p = get_flag(cy);
    set_flag(cy, a + *reg + get_flag(cy) > 0xff);
    a += *reg + p;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 4;
}                 // Adds register and c flag to accumulator
int ADC_A_N(uint8_t value){
    set_flag(hy, (a & 0xf) + (value & 0xf) + get_flag(cy)> 0xf);
    bool p = get_flag(cy);
    set_flag(cy, a + value + get_flag(cy) > 0xff);
    a += value + p;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 8;
}               // Adds value and c flag to accumulator
int ADC_A_HL(){
    set_flag(hy, (a & 0xf) + (fetch(get_hl()) & 0xf) + get_flag(cy)> 0xf);
    bool p = get_flag(cy);
    set_flag(cy, a + fetch(get_hl()) + get_flag(cy) > 0xff);
    a += fetch(get_hl()) + p;
    set_flag(z, a==0);
    set_flag(n, 0);
    return 8;
}                           // Adds (HL) pointer's value and c flag to accumulator
int SUB_A_R(uint8_t* reg){
    set_flag(hy, (a & 0xf) - (*reg & 0xf) < 0x0);
    set_flag(cy, a - *reg < 0x0000);
    a -= *reg;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 4;
}                // Subtracts register from accumulator
int SUB_A_N(uint8_t value){
    set_flag(hy, (a & 0xf) - (value & 0xf) < 0x0);
    set_flag(cy, a - value < 0x0000);
    a -= value;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 8;
}               // Subtracts value from accumulator
int SUB_A_HL(){
    set_flag(hy, (a & 0xf) - (fetch(get_hl()) & 0xf) < 0x0);
    set_flag(cy, a - fetch(get_hl()) < 0x0000);
    a -= fetch(get_hl());
    set_flag(z, a==0);
    set_flag(n, 1);
    return 8;
}                           // Subtracts (HL) pointer's value from accumulator
int SBC_A_R(uint8_t* reg){
    set_flag(hy, (a & 0xf) - (*reg & 0xf) - get_flag(cy) < 0x0);
    bool p = get_flag(cy);
    set_flag(cy, a - *reg - get_flag(cy) < 0x0000);
    a = a - *reg - p;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 4;
}                // Subtracts register and c flag from accumulator
int SBC_A_N(uint8_t value){
    set_flag(hy, (a & 0xf) - (value & 0xf) - get_flag(cy) < 0x0);
    bool p = get_flag(cy);
    set_flag(cy, a - value - get_flag(cy) < 0x0000);
    a = a - value - p;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 8;
}               // Subtracts value and c flag from accumulator
int SBC_A_HL(){
    set_flag(hy, (a & 0xf) - (fetch(get_hl()) & 0xf) - get_flag(cy) < 0x0);
    bool p = get_flag(cy);
    set_flag(cy, a - fetch(get_hl()) - get_flag(cy) < 0x0000);
    a = a - fetch(get_hl()) - p;
    set_flag(z, a==0);
    set_flag(n, 1);
    return 8;
}                           // Subtracts (HL) pointer's value and c flag from accumulator
int AND_A_R(uint8_t* reg){
    a &= *reg;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(cy, 0);
    return 4;
}                // Masks A and Register, returns into A
int AND_A_N(uint8_t value){
    a &= value;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(cy, 0);
    return 8;
}               // Masks A and value, returns into A
int AND_A_HL(){
    a &= fetch(get_hl());
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(cy, 0);
    return 8;
}                           // Masks A and (HL) pointers value, returns into A
int XOR_A_R(uint8_t* reg){
    a ^= *reg;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 4;
}                // Opposite of Mask A and Register, returns into A
int XOR_A_N(uint8_t value){
    a ^= value;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 8;
}               // Opposite of Mask A and value, returns into A
int XOR_A_HL(){
    a^= fetch(get_hl());
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 8;
}                           // Opposite of Mask A and (HL) pointers value, returns into A
int OR_A_R(uint8_t* reg){
    a |= *reg;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 4;
}                 // Combines A and Register, returns into A
int OR_A_N(uint8_t value){
    a |= value;
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 8;
}                // Combines A and value, returns into A
int OR_A_HL(){
    a |= fetch(get_hl());
    set_flag(z, a==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 0);
    return 8;
}                            // Combines A and (HL) pointers value, returns into A
int CP_A_R(uint8_t* reg){ 
    set_flag(z, a==*reg);
    set_flag(n, 1);
    set_flag(hy, (a & 0xf) - (*reg & 0xf) < 0);
    set_flag(cy, a-*reg < 0);
    return 4;
}                 // Compares A and Register, returns into A
int CP_A_N(uint8_t value){
    set_flag(z, a==value);
    set_flag(n, 1);
    set_flag(hy, (a & 0xf) - (value & 0xf) < 0);
    set_flag(cy, a-value < 0);
    return 8;
}                // Compares A and value, returns into A
int CP_A_HL(){
    set_flag(z, a==fetch(get_hl()));
    set_flag(n, 1);
    set_flag(hy, (a & 0xf) - (fetch(get_hl()) & 0xf) < 0);
    set_flag(cy, a-fetch(get_hl()) < 0);
    return 8;
}                            // Compares A and (HL) pointers value, returns into A
int INC_R(uint8_t* reg){
    *reg +=1;
    set_flag(z, *reg == 0);
    set_flag(n, 0);
    set_flag(hy, (*reg>>4) % 2 != ((*reg-1)>>4) % 2);
    return 4;
}                  // Adds one to register
int INC_HL(){
    post(get_hl(), fetch(get_hl())+1);
    set_flag(z, fetch(get_hl()) == 0);
    set_flag(n, 0);
    set_flag(hy, (fetch(get_hl()) & 0xf) == 0);
    return 12;
}                             // Adds on to (HL) pointer's value
int DEC_R(uint8_t* reg){
    *reg -= 1;
    set_flag(z, *reg == 0);
    set_flag(n, 1);
    set_flag(hy, (*reg>>4) % 2 != ((*reg+1)>>4) % 2);
    return 4;
}                  // Subtracts one from register
int DEC_HL(){
    post(get_hl(), fetch(get_hl())-1);
    set_flag(z, fetch(get_hl()) == 0);
    set_flag(n, 1);
    set_flag(hy, (fetch((get_hl()))&0x10) != ((fetch(get_hl())+1)&0x10));
    return 12;
}                             // Subtracts one from (HL) pointer's value
int DAA(){
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
int CPL(){
    set_flag(n, 1);
    set_flag(hy, 1);
    a ^= 0xff;
    return 4;
}                                // Flips all bits in Acumulator

// 16 Bit Arithmetic
int ADD_HL_RR(uint8_t* reg1, uint8_t* reg2){
    uint16_t first = get_hl();
    uint16_t second = x8_x16(*reg1, *reg2);
    set_flag(hy, (first&0xfff)+(second&0xfff) > 0xfff);
    set_flag(cy, first + second > 0xffff);
    set_hl(first+second);
    set_flag(n, 0);
    return 8;
} // HL = HL + rr

int ADD_HL_SP() {
    set_flag(hy, (get_hl() & 0xfff) + (SP & 0xfff) > 0xfff);
    set_flag(cy, (int)get_hl() + (int)SP > 0xffff);
    set_hl(get_hl() + SP);
    set_flag(n, 0);
    return 8;
}

int INC_RR(uint8_t* reg1, uint8_t* reg2){
    *reg1 = x16_x81(x8_x16(*reg1, *reg2)+1);
    *reg2 = x16_x82(x8_x16(*reg1, *reg2)+1);
    return 8;
}   // RR += 1
int DEC_RR(uint8_t* reg1, uint8_t* reg2){
    *reg1 = x16_x81(x8_x16(*reg1, *reg2)-1);
    *reg2 = x16_x82(x8_x16(*reg1, *reg2)-1);
    return 8;
}   // RR -= 1

int INC_SP() {
    ++SP;
    return 8;
}

int DEC_SP() {
    SP--;
    return 8;
}
int ADD_SP_DD(int8_t value){
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, (value&0xf) + (SP & 0xf) > 0xf);
    set_flag(cy, (uint8_t)value + (int)(SP & 0xff) > 0xff);
    SP += value;
    return 16;
} // SP += DD

int LD_HL_SP_DD(int8_t value){
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, ((uint8_t)value & 0xf) + (SP & 0xf) > 0xf);
    set_flag(cy, (uint8_t)value + (SP & 0xff) > 0xff);
    set_hl(SP + value);
    return 12;
} // HL = SP + DD
        

        // Rotate And Shift Commands
int RLCA(){
    set_flag(cy, a>>7);
    a = (a << 1) | (a >> 7); 
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 4;
}                             // rotates accumulator left, bit 7 -> cy flag and bit  0
int RLA(){
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
int RRCA(){
    set_flag(cy, a & 1);
    a = (a >> 1) | (a << 7); 
    set_flag(z, 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 4;
}                             // rotates accumulator right, bit 0->cy flag and bit 7
int RCA(){
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
int RLC_R(uint8_t* reg){
    set_flag(cy, *reg>>7);
    *reg = (*reg << 1) | (*reg >> 7); 
    set_flag(z, *reg==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 8;
}                // rotate register left bit 7 -> cy flag
int RLC_HL(){
    set_flag(cy, fetch(get_hl()) >>7);
    post(get_hl(), (fetch(get_hl()) << 1) | (fetch(get_hl()) >> 7)); 
    set_flag(z, fetch(get_hl())==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 12;
}                           // rotate HL pointer's value left bit 7 -> cy flag
int RL_R(uint8_t* reg){
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
int RL_HL(){
    bool p = get_flag(cy);
    set_flag(cy, fetch(get_hl()) >> 7);
    post(get_hl(), (fetch(get_hl()) << 1) | (fetch(get_hl()) >> 7));
    if (p== 0) { post(get_hl(), fetch(get_hl()) & ~ 1);
    } else {post(get_hl(), fetch(get_hl()) | 1);}
    set_flag(z, fetch(get_hl()) == 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 12;

}                            // rotate HL pointer's value left, 0 = cy flag
int RRC_R(uint8_t* reg){
    set_flag(cy, (*reg)&1);
    *reg = ((*reg) >> 1) | ((*reg) << 7); 
    set_flag(z, *reg==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 8;

}                // rotate register right bit 0 -> cy flag
int RRC_HL(){
    bool p = fetch(get_hl()) & 1;
    post(get_hl(), (fetch(get_hl()) >> 1) | (fetch(get_hl()) << 7)); 
    set_flag(cy, p);
    set_flag(z, fetch(get_hl())==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 12;
}                           // rotate HL pointer's value right bit 0 -> cy flag
int RR_R(uint8_t* reg){
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
int RR_HL(){
    bool p = fetch(get_hl()) & 1;
    post(get_hl(), (fetch(get_hl()) >> 1) | (fetch(get_hl()) << 7));
    if (get_flag(cy)== 0) {post(get_hl(), fetch(get_hl()) & 127);
    } else {post(get_hl(), fetch(get_hl()) | 128);}
    set_flag(cy, p);
    set_flag(z, fetch(get_hl()) == 0);
    set_flag(n, 0);
    set_flag(hy, 0);
    return 12;
}                            // rotate HL pointer's value right, 7 = cy flag
int SLA_R(uint8_t* reg){
    bool p = *reg & 128;
    *reg = *reg << 1;
    set_flag(cy, p);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, *reg==0);
    return 8;
}                // shift register left arithmetic 0 = 0
int SLA_HL(){
    bool p = fetch(get_hl()) & 128;
    post(get_hl(), fetch(get_hl()) << 1);
    set_flag(cy, p);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, fetch(get_hl())==0);
    return 16;
}                           // shift HL pointer's value left 0 = 0
int SWAP_R(uint8_t* reg){
    *reg = *reg << 4 | *reg >> 4;
    set_flag(z, *reg==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy,0);
    return 8;
}               // Swaps 0-4 bits and 5-8 bits. of register
int SWAP_HL(){
    post(get_hl(), fetch(get_hl()) << 4 | fetch(get_hl()) >> 4);
    set_flag(z, fetch(get_hl())==0);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy,0);
    return 16;
}                          // Swaps 0-4 bits and 5-8 bits of HL pointer's value
int SRA_R(uint8_t* reg){
    set_flag(cy, *reg & 1);
    *reg = *reg >> 1 | (*reg & 128);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, *reg==0);
    return 8;
}                // Shifts register right arithmetacilly (7=7)
int SRA_HL(){
    set_flag(cy, fetch(get_hl()) & 1);
    uint8_t t = fetch(get_hl());
    post(get_hl(), t >> 1 | (t & 128));
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, fetch(get_hl())==0);
    return 16;
}                           // Shifts HL Pointer's value right arithmetically (7=7)
int SRL_R(uint8_t* reg){
    set_flag(cy, *reg & 1);
    *reg = *reg >> 1;
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, *reg==0);
    return 8;
}                // Shifts register right logically (7=0)
int SRL_HL(){
    set_flag(cy, fetch(get_hl()) & 1);
    post(get_hl(), fetch(get_hl()) >> 1);
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(z, fetch(get_hl())==0);
    return 16;
}                           // Shifts HL Pointer's value right logically (7=0)
        
// Singlebit Commands
int BIT_N_R(uint8_t value, uint8_t* reg){
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !(((1 << value) & *reg) >> value));
    return 8;
}   // Copies Bit N of register into Z Flag
int BIT_N_HL(uint8_t value){
    set_flag(n, 0);
    set_flag(hy, 1);
    set_flag(z, !(((1 << value) & fetch(get_hl())) >> value));
    return 12;
}                // Copies Bit N of HL Pointer's value into Z Flag
int SET_N_R(uint8_t value, uint8_t* reg){
    *reg = *reg | (1 << value);
    return 8;
}   // Sets Bit N of register to 1
int SET_N_HL(uint8_t value){
    post(get_hl(), fetch(get_hl()) | (1 << value));
    return 16;
}                // Sets bit N of HL Pointer's value to 1
int RES_N_R(uint8_t value, uint8_t *reg){
    *reg = *reg & ~(1 << value);
    return 8;
}   // Sets Bit N of register to 0
int RES_N_HL(uint8_t value){
    post(get_hl(), fetch(get_hl()) & ~(1 << value));
    return 16;
}                // Sets bit N of HL Pointer's value to 0
        
// CPU Control Commands
int CCF(){
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, get_flag(cy) ^ 1);
    return 4;
}                                  // Flips CY Flag
int SCF(){
    set_flag(n, 0);
    set_flag(hy, 0);
    set_flag(cy, 1);
    return 4;
}                                  // Sets CY Flag to 1
int NOP(){
    return 4;
}                                  // No Operation
int HALT(){
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
int STOP(){
    // STILL MUST DO
    return 4;
}                                 // Low power standby mode (very low power)
int DI(){
    IME = false;
    return 4;
}                                   // disable interrupts
int EI(){
    IME = true;
    return 4;
}                                   // Enable interrupts

// Jump commands
int JP_NN(uint8_t value1, uint8_t value2){
    PC = x8_x16(value2, value1);
    return 16;
}  // PC = 16 bit value1+2
int JP_HL(){
    PC = get_hl();
    return 4;
}                                // PC = HL 
int JP_F_NN(uint8_t value1, uint8_t value2, bool flag_value){
    if (flag_value) {
        PC = x8_x16(value2, value1);
        return 16;
    }
    return 12;
}  // PC = 16 bit value1+2 if FLAG is set
int JR_DD(int8_t value){
    PC += value;    // minus 2 because the instruction itself is 2 bytes
    return 12;
}                   // PC += value
int JR_F_DD(int8_t value, bool flag_value){
    if (flag_value) {
        PC += value;
        return 12;
    }
    return 8;
}   // PC += value if FLAG is set
int CALL_NN(uint8_t value1, uint8_t value2){
    post(SP-1, (uint8_t) (PC>>8));
    post(SP-2, (uint8_t) PC);
    SP-=2;
    PC = x8_x16(value2,value1);
    return 24;
}// PC = 16 bit value1+2, SP-=2, (SP) = PC
int CALL_F_NN(uint8_t value1, uint8_t value2, bool flag_value){
    if (flag_value) {
        post(SP-1, (uint8_t) (PC>>8));
        post(SP-2, (uint8_t) PC);
        SP-=2;
        PC = x8_x16(value2,value1);
        return 24;
    }
    return 12;
}// PC = 16 bit value1+2, SP-=2, (SP) = PC IF Flag is set
int RET(){
    PC = x8_x16(fetch(SP+1), fetch(SP));
    SP += 2;
    return 16;
}                                  // PC=(SP), SP+=2
int RET_F(bool flag_value){
    if (flag_value) {
        PC = x8_x16(fetch(SP+1), fetch(SP));
        SP += 2;
        return 20;
    }
    return 8;
}                    // PC=(SP), SP+=2 IF Flag is set
int RETI(){
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


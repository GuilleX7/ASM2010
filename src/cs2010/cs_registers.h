/** @file cs_registers.h */

#ifndef CS_REGISTERS_H
#define CS_REGISTERS_H

#include <stdint.h>

#define CS_REGISTERS_RMIN 0
#define CS_REGISTERS_RMAX 7

#define CS_SIGNAL_WMAR (1ul << 0)
#define CS_SIGNAL_WMDR (1ul << 1)
#define CS_SIGNAL_IOMDR (1ul << 2)
#define CS_SIGNAL_WMEM (1ul << 3)
#define CS_SIGNAL_RMEM (1ul << 4)
#define CS_SIGNAL_WIR (1ul << 5)
#define CS_SIGNAL_WPC (1ul << 6)
#define CS_SIGNAL_RPC (1ul << 7)
#define CS_SIGNAL_CPC (1ul << 8)
#define CS_SIGNAL_IPC (1ul << 9)
#define CS_SIGNAL_ISP (1ul << 10)
#define CS_SIGNAL_DSP (1ul << 11)
#define CS_SIGNAL_CSP (1ul << 12)
#define CS_SIGNAL_RSP (1ul << 13)
#define CS_SIGNAL_INM (1ul << 14)
#define CS_SIGNAL_SRW (1ul << 15)
#define CS_SIGNAL_WAC (1ul << 16)
#define CS_SIGNAL_RAC (1ul << 17)
#define CS_SIGNAL_ALUOP0 (1ul << 18)
#define CS_SIGNAL_ALUOP1 (1ul << 19)
#define CS_SIGNAL_ALUOP2 (1ul << 20)
#define CS_SIGNAL_ALUOP3 (1ul << 21)
#define CS_SIGNAL_WREG (1ul << 22)

#define CS_SIGNALS_NONE 0
#define CS_SIGNALS_ALUOP (CS_SIGNAL_ALUOP0 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_ALUOP3)
#define CS_SIGNALS_FETCH (CS_SIGNAL_IPC | CS_SIGNAL_WIR)

#define CS_ALUOP_CLC 0
#define CS_ALUOP_SEC (CS_SIGNAL_ALUOP0 | CS_SIGNAL_ALUOP1)
#define CS_ALUOP_SHR CS_SIGNAL_ALUOP2
#define CS_ALUOP_SHL (CS_SIGNAL_ALUOP0 | CS_SIGNAL_ALUOP2)
#define CS_ALUOP_TRA (CS_SIGNAL_ALUOP1 | CS_SIGNAL_ALUOP2)
#define CS_ALUOP_ADD CS_SIGNAL_ALUOP3
#define CS_ALUOP_SUB (CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1)
#define CS_ALUOP_TRB (CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2)

#define CS_SR_C (1u << 0)
#define CS_SR_Z (1u << 1)
#define CS_SR_N (1u << 2)
#define CS_SR_V (1u << 3)

struct cs_registers {
    uint32_t signals;
    uint16_t ir;
    uint8_t r0;
    uint8_t r1;
    uint8_t r2;
    uint8_t r3;
    uint8_t r4;
    uint8_t r5;
    uint8_t r6;
    uint8_t r7;
    uint8_t *regfile[8];
    uint8_t sp;
    uint8_t pc;
    uint8_t ac;
    uint8_t sr;
    uint8_t mdr;
    uint8_t mar;
};
typedef struct cs_registers cs_registers;

#endif /* CS_REGISTERS_H */
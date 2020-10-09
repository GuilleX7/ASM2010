/** @file cs_registers.h */

#ifndef CS_REGISTERS_H
#define CS_REGISTERS_H

#include <stdint.h>

#define CS_REGISTERS_RMIN 0
#define CS_REGISTERS_RMAX 7

#define CS_SIGNALS_NONE 0
#define CS_SIGNAL_WMAR (1 << 0)
#define CS_SIGNAL_WMDR (1 << 1)
#define CS_SIGNAL_IOMDR (1 << 2)
#define CS_SIGNAL_WMEM (1 << 3)
#define CS_SIGNAL_RMEM (1 << 4)
#define CS_SIGNAL_WIR (1 << 5)
#define CS_SIGNAL_WPC (1 << 6)
#define CS_SIGNAL_RPC (1 << 7)
#define CS_SIGNAL_CPC (1 << 8)
#define CS_SIGNAL_IPC (1 << 9)
#define CS_SIGNAL_ISP (1 << 10)
#define CS_SIGNAL_DSP (1 << 11)
#define CS_SIGNAL_CSP (1 << 12)
#define CS_SIGNAL_RSP (1 << 13)
#define CS_SIGNAL_INM (1 << 14)
#define CS_SIGNAL_SRW (1 << 15)
#define CS_SIGNAL_WAC (1 << 16)
#define CS_SIGNAL_RAC (1 << 17)
#define CS_SIGNAL_ALUOP0 (1 << 18)
#define CS_SIGNAL_ALUOP1 (1 << 19)
#define CS_SIGNAL_ALUOP2 (1 << 20)
#define CS_SIGNAL_ALUOP3 (1 << 21)
#define CS_SIGNAL_ALUOP (CS_SIGNAL_ALUOP0 | CS_SIGNAL_ALUOP1 | CS_SIGNAL_ALUOP2 | CS_SIGNAL_ALUOP3)
#define CS_SIGNAL_WREG (1 << 22)

#define CS_ALUOP_CLC 0
#define CS_ALUOP_SEC (CS_SIGNAL_ALUOP0 | CS_SIGNAL_ALUOP1)
#define CS_ALUOP_SHR CS_SIGNAL_ALUOP2
#define CS_ALUOP_SHL (CS_SIGNAL_ALUOP0 | CS_SIGNAL_ALUOP2)
#define CS_ALUOP_TRA (CS_SIGNAL_ALUOP1 | CS_SIGNAL_ALUOP2)
#define CS_ALUOP_ADD CS_SIGNAL_ALUOP3
#define CS_ALUOP_SUB (CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP1)
#define CS_ALUOP_TRB (CS_SIGNAL_ALUOP3 | CS_SIGNAL_ALUOP2)

#define CS_SR_C (1 << 0)
#define CS_SR_Z (1 << 1)
#define CS_SR_N (1 << 2)
#define CS_SR_V (1 << 3)

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
    uint8_t sp;
    uint8_t pc;
    uint8_t ac;
    uint8_t sr;
    uint8_t mdr;
    uint8_t mar;
};
typedef struct cs_registers cs_registers;

#endif /* CS_REGISTERFILE_H */
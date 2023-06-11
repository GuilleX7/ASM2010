/** @file cs2010_platform.c */

#include "../../../include/asm2010.h"

#include "../cs_opcodes.h"

#include "cs2010_opcodes.h"
#include "cs2010_signals.h"

#include "cs2010_platform.h"

cs_instruction_op const cs2010_platform_opcodes[] = {
    [CS_INS_I_ST] =
        {
            cs2010_op_st_stepper,
            cs2010_op_st_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC,
                CS2010_SIGNALS_ALUOP_TRANSFER_A | CS_SIGNAL_RAC | CS_SIGNAL_WAC | CS_SIGNAL_WMAR,
                CS_SIGNAL_RAC | CS_SIGNAL_WMDR,
                CS_SIGNAL_WMEM | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_LD] =
        {
            cs2010_op_ld_stepper,
            cs2010_op_ld_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC,
                CS_SIGNAL_RAC | CS_SIGNAL_WMAR,
                CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
                CS_SIGNAL_WREG | CS_SIGNAL_IOMDR | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_STS] =
        {
            cs2010_op_sts_stepper,
            cs2010_op_sts_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                CS2010_SIGNALS_ALUOP_TRANSFER_A | CS_SIGNAL_RAC | CS_SIGNAL_WAC | CS_SIGNAL_WMAR,
                CS_SIGNAL_RAC | CS_SIGNAL_WMDR,
                CS_SIGNAL_WMEM | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_LDS] =
        {
            cs2010_op_lds_stepper,
            cs2010_op_lds_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                CS_SIGNAL_RAC | CS_SIGNAL_WMAR,
                CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
                CS_SIGNAL_WREG | CS_SIGNAL_IOMDR | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_CALL] =
        {
            cs2010_op_call_stepper,
            cs2010_op_call_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC | CS_SIGNAL_INM | CS_SIGNAL_RPC | CS_SIGNAL_WMDR,
                CS_SIGNAL_RSP | CS_SIGNAL_DSP | CS_SIGNAL_WMAR,
                CS_SIGNAL_RAC | CS_SIGNAL_WPC | CS_SIGNAL_WMEM,
                CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_RET] =
        {
            cs2010_op_ret_stepper,
            cs2010_op_ret_microstepper,
            {
                CS_SIGNAL_ISP,
                CS_SIGNAL_RSP | CS_SIGNAL_WMAR,
                CS_SIGNAL_RMEM | CS_SIGNAL_IOMDR | CS_SIGNAL_WMDR,
                CS_SIGNAL_WPC | CS_SIGNAL_IOMDR,
                CS_SIGNALS_FETCH,
            },
        },
    [CS_INS_I_BRXX] =
        {
            cs_op_brxx_stepper,
            cs_op_brxx_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                CS_SIGNAL_RAC | CS_SIGNAL_WPC,
                CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_JMP] =
        {
            cs_op_jmp_stepper,
            cs_op_jmp_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                CS_SIGNAL_RAC | CS_SIGNAL_WPC,
                CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_ADD] =
        {
            cs_op_add_stepper,
            cs_op_add_microstepper,
            {
                CS2010_SIGNALS_ALUOP_ADD | CS_SIGNAL_WAC | CS_SIGNAL_SRW,
                CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [0x9] = CS_OP_NOOP,
    [CS_INS_I_SUB] =
        {
            cs_op_sub_stepper,
            cs_op_sub_microstepper,
            {
                CS2010_SIGNALS_ALUOP_SUB | CS_SIGNAL_WAC | CS_SIGNAL_SRW,
                CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_CP] =
        {
            cs_op_cp_stepper,
            cs_op_cp_stepper,
            {
                CS2010_SIGNALS_ALUOP_SUB | CS_SIGNAL_SRW | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [0xc] = CS_OP_NOOP,
    [0xd] = CS_OP_NOOP,
    [0xe] = CS_OP_NOOP,
    [CS_INS_I_MOV] =
        {
            cs_op_mov_stepper,
            cs_op_mov_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC,
                CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [0x10] = CS_OP_NOOP,
    [0x11] = CS_OP_NOOP,
    [CS_INS_I_CLC] =
        {
            cs2010_op_clc_stepper,
            cs2010_op_clc_stepper,
            {
                CS_SIGNAL_SRW | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_SEC] =
        {
            cs2010_op_sec_stepper,
            cs2010_op_sec_stepper,
            {
                CS2010_SIGNALS_ALUOP_SEC | CS_SIGNAL_SRW | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_ROR] =
        {
            cs2010_op_ror_stepper,
            cs2010_op_ror_microstepper,
            {
                CS2010_SIGNALS_ALUOP_SHR | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
                CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_ROL] =
        {
            cs2010_op_rol_stepper,
            cs2010_op_rol_microstepper,
            {
                CS2010_SIGNALS_ALUOP_SHL | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
                CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [0x16] = CS_OP_NOOP,
    [CS_INS_I_STOP] =
        {
            cs_op_stop_stepper,
            cs_op_stop_stepper,
            {
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_ADDI] =
        {
            cs2010_op_addi_stepper,
            cs2010_op_addi_microstepper,
            {
                CS2010_SIGNALS_ALUOP_ADD | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
                CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [0x19] = CS_OP_NOOP,
    [CS_INS_I_SUBI] =
        {
            cs_op_subi_stepper,
            cs_op_subi_microstepper,
            {
                CS2010_SIGNALS_ALUOP_SUB | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
                CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [CS_INS_I_CPI] =
        {
            cs_op_cpi_stepper,
            cs_op_cpi_stepper,
            {
                CS2010_SIGNALS_ALUOP_SUB | CS_SIGNAL_SRW | CS_SIGNAL_INM | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
    [0x1c] = CS_OP_NOOP,
    [0x1e] = CS_OP_NOOP,
    [CS_INS_I_LDI] =
        {
            cs_op_ldi_stepper,
            cs_op_ldi_microstepper,
            {
                CS2010_SIGNALS_ALUOP_TRANSFER_B | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
                CS_SIGNALS_NONE,
            },
        },
};

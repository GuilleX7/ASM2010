/** @file cs3_platform.c */

#include "../../../include/asm2010.h"

#include "../cs_opcodes.h"

#include "cs3_opcodes.h"

#include "cs3_platform.h"

cs_instruction_op const cs3_platform_opcodes[] = {
    [CS_INS_I_ST]   = {cs3_op_st_stepper,
                       cs3_op_st_microstepper,
                       {
                         CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC,
                         CS3_SIGNAL_ALU_TA | CS_SIGNAL_RAC | CS_SIGNAL_WAC | CS_SIGNAL_WMAR,
                         CS_SIGNAL_WMEM | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                         CS_SIGNALS_NONE,
                         CS_SIGNALS_NONE,
                     }},
    [CS_INS_I_LD]   = {cs3_op_ld_stepper,
                       cs3_op_ld_microstepper,
                       {
                         CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC,
                         CS_SIGNAL_RAC | CS_SIGNAL_WMAR,
                         CS_SIGNAL_RMEM | CS_SIGNAL_WREG | CS_SIGNALS_FETCH,
                         CS_SIGNALS_NONE,
                         CS_SIGNALS_NONE,
                     }},
    [CS_INS_I_STS]  = {cs3_op_sts_stepper,
                       cs3_op_sts_microstepper,
                       {
                          CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                          CS3_SIGNAL_ALU_TA | CS_SIGNAL_RAC | CS_SIGNAL_WAC | CS_SIGNAL_WMAR,
                          CS_SIGNAL_WMEM | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                      }},
    [CS_INS_I_LDS]  = {cs3_op_lds_stepper,
                       cs3_op_lds_microstepper,
                       {
                          CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                          CS_SIGNAL_RAC | CS_SIGNAL_WMAR,
                          CS_SIGNAL_WREG | CS_SIGNAL_RMEM | CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                      }},
    [CS_INS_I_CALL] = {cs3_op_call_stepper,
                       cs3_op_call_microstepper,
                       {
                           CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC | CS_SIGNAL_INM | CS_SIGNAL_WMAR | CS_SIGNAL_RSP |
                               CS_SIGNAL_DSP,
                           CS_SIGNAL_WMEM | CS_SIGNAL_RPC,
                           CS_SIGNAL_RAC | CS_SIGNAL_WPC | CS_SIGNALS_FETCH,
                           CS_SIGNALS_FETCH,
                           CS_SIGNALS_NONE,
                       }},
    [CS_INS_I_RET]  = {cs3_op_ret_stepper,
                       cs3_op_ret_microstepper,
                       {
                          CS_SIGNAL_ISP,
                          CS_SIGNAL_RSP | CS_SIGNAL_WMAR,
                          CS_SIGNAL_WPC | CS_SIGNAL_RMEM,
                          CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                      }},
    [CS_INS_I_BRXX] = {cs_op_brxx_stepper,
                       cs_op_brxx_microstepper,
                       {
                           CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                           CS_SIGNAL_RAC | CS_SIGNAL_WPC,
                           CS_SIGNALS_FETCH,
                           CS_SIGNALS_NONE,
                           CS_SIGNALS_NONE,
                       }},
    [CS_INS_I_JMP]  = {cs_op_jmp_stepper,
                       cs_op_jmp_microstepper,
                       {
                          CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                          CS_SIGNAL_RAC | CS_SIGNAL_WPC,
                          CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                      }},
    [CS_INS_I_ADD]  = {cs_op_add_stepper,
                       cs_op_add_microstepper,
                       {
                          CS3_SIGNAL_ALU_S | CS_SIGNAL_WAC | CS_SIGNAL_SRW,
                          CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                      }},
    [0x9]           = CS_OP_NOOP,
    [CS_INS_I_SUB]  = {cs_op_sub_stepper,
                       cs_op_sub_microstepper,
                       {
                          CS3_SIGNAL_ALU_R | CS_SIGNAL_WAC | CS_SIGNAL_SRW,
                          CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                      }},
    [CS_INS_I_CP]   = {cs_op_cp_stepper,
                       cs_op_cp_stepper,
                       {
                         CS3_SIGNAL_ALU_R | CS_SIGNAL_SRW | CS_SIGNALS_FETCH,
                         CS_SIGNALS_NONE,
                         CS_SIGNALS_NONE,
                         CS_SIGNALS_NONE,
                         CS_SIGNALS_NONE,
                     }},
    [0xc]           = CS_OP_NOOP,
    [0xd]           = CS_OP_NOOP,
    [0xe]           = CS_OP_NOOP,
    [CS_INS_I_MOV]  = {cs_op_mov_stepper,
                       cs_op_mov_microstepper,
                       {
                          CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC,
                          CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                      }},
    [0x10]          = CS_OP_NOOP,
    [0x11]          = CS_OP_NOOP,
    [0x12]          = CS_OP_NOOP,
    [0x13]          = CS_OP_NOOP,
    [0x14]          = CS_OP_NOOP,
    [0x15]          = CS_OP_NOOP,
    [0x16]          = CS_OP_NOOP,
    [CS_INS_I_STOP] = {cs_op_stop_stepper,
                       cs_op_stop_stepper,
                       {
                           CS_SIGNALS_NONE,
                           CS_SIGNALS_NONE,
                           CS_SIGNALS_NONE,
                           CS_SIGNALS_NONE,
                           CS_SIGNALS_NONE,
                       }},
    [0x18]          = CS_OP_NOOP,
    [0x19]          = CS_OP_NOOP,
    [CS_INS_I_SUBI] = {cs_op_subi_stepper,
                       cs_op_subi_microstepper,
                       {
                           CS3_SIGNAL_ALU_R | CS_SIGNAL_WAC | CS_SIGNAL_SRW | CS_SIGNAL_INM,
                           CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                           CS_SIGNALS_NONE,
                           CS_SIGNALS_NONE,
                           CS_SIGNALS_NONE,
                       }},
    [CS_INS_I_CPI]  = {cs_op_cpi_stepper,
                       cs_op_cpi_stepper,
                       {
                          CS3_SIGNAL_ALU_R | CS_SIGNAL_SRW | CS_SIGNAL_INM | CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                      }},
    [0x1c]          = CS_OP_NOOP,
    [0x1d]          = CS_OP_NOOP,
    [0x1e]          = CS_OP_NOOP,
    [CS_INS_I_LDI]  = {cs_op_ldi_stepper,
                       cs_op_ldi_microstepper,
                       {
                          CS3_SIGNAL_ALU_TB | CS_SIGNAL_WAC | CS_SIGNAL_INM,
                          CS_SIGNAL_WREG | CS_SIGNAL_RAC | CS_SIGNALS_FETCH,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                          CS_SIGNALS_NONE,
                      }},
};
/** @file cs2010_opcodes.c */

#include "../../../include/asm2010.h"

#include "../cs_instructions.h"
#include "../cs_opcodes.h"

#include "cs2010_opcodes.h"

/* CS2010 ST */
int cs2010_op_st_stepper(cs_machine *cs) {
    cs->registers.mar = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
    cs->registers.ac  = *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
    cs->registers.mdr = cs->registers.ac;
    cs_write_output(cs, cs->registers.mar, cs->registers.mdr);
    return CS_OP_DO_FETCH;
}

int cs2010_op_st_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.ac;
            cs->registers.ac  = *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 2:
            cs->registers.mdr = cs->registers.ac;
            return CS_OP_DO_MICROFETCH;
        case 3:
        default:
            cs_write_output(cs, cs->registers.mar, cs->registers.mdr);
            return CS_OP_DO_FETCH;
    }
}

/* CS2010 LD */
int cs2010_op_ld_stepper(cs_machine *cs) {
    cs->registers.ac                                       = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
    cs->registers.mar                                      = cs->registers.ac;
    cs->registers.mdr                                      = cs_read_input(cs, cs->registers.mar);
    *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs->registers.mdr;
    return CS_OP_DO_FETCH;
}

int cs2010_op_ld_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.ac;
            return CS_OP_DO_MICROFETCH;
        case 2:
            cs->registers.mdr = cs_read_input(cs, cs->registers.mar);
            return CS_OP_DO_MICROFETCH;
        case 3:
        default:
            *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs->registers.mdr;
            return CS_OP_DO_FETCH;
    }
}

/* CS2010 STS */
int cs2010_op_sts_stepper(cs_machine *cs) {
    cs->registers.mar = CS_GET_ARG_B(cs->registers.ir);
    cs->registers.ac  = *cs->registers.regfile[CS_GET_ARG_A(cs->registers.ir)];
    cs->registers.mdr = cs->registers.ac;
    cs_write_output(cs, cs->registers.mar, cs->registers.mdr);
    return CS_OP_DO_FETCH;
}

int cs2010_op_sts_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.ac;
            cs->registers.ac  = *cs->registers.regfile[CS_GET_ARG_A(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 2:
            cs->registers.mdr = cs->registers.ac;
            return CS_OP_DO_MICROFETCH;
        case 3:
        default:
            cs_write_output(cs, cs->registers.mar, cs->registers.mdr);
            return CS_OP_DO_FETCH;
    }
}

/* CS2010 LDS */
int cs2010_op_lds_stepper(cs_machine *cs) {
    cs->registers.ac                                       = CS_GET_ARG_B(cs->registers.ir);
    cs->registers.mar                                      = cs->registers.ac;
    cs->registers.mdr                                      = cs_read_input(cs, cs->registers.mar);
    *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs->registers.mdr;
    return CS_OP_DO_FETCH;
}

int cs2010_op_lds_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.ac;
            return CS_OP_DO_MICROFETCH;
        case 2:
            cs->registers.mdr = cs_read_input(cs, cs->registers.mar);
            return CS_OP_DO_MICROFETCH;
        case 3:
        default:
            *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs->registers.mdr;
            return CS_OP_DO_FETCH;
    }
}

/* CS2010 CALL */
int cs2010_op_call_stepper(cs_machine *cs) {
    cs->registers.mdr                 = cs->registers.pc;
    cs->registers.ac                  = CS_GET_ARG_B(cs->registers.ir);
    cs->registers.mar                 = cs->registers.sp--;
    cs->registers.pc                  = cs->registers.ac;
    cs->memory.ram[cs->registers.mar] = cs->registers.mdr;
    return CS_OP_DO_FETCH;
}

int cs2010_op_call_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.mdr = cs->registers.pc;
            cs->registers.ac  = CS_GET_ARG_B(cs->registers.ir);
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.sp--;
            return CS_OP_DO_MICROFETCH;
        case 2:
        default:
            cs->registers.pc                  = cs->registers.ac;
            cs->memory.ram[cs->registers.mar] = cs->registers.mdr;
            return CS_OP_DO_FETCH;
    }
}

/* CS2010 RET */
int cs2010_op_ret_stepper(cs_machine *cs) {
    cs->registers.mar = ++cs->registers.sp;
    cs->registers.mdr = cs->memory.ram[cs->registers.mar];
    cs->registers.pc  = cs->registers.mdr;
    return CS_OP_DO_FETCH;
}

int cs2010_op_ret_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.sp++;
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.sp;
            return CS_OP_DO_MICROFETCH;
        case 2:
            cs->registers.mdr = cs->memory.ram[cs->registers.mar];
            return CS_OP_DO_MICROFETCH;
        case 3:
        default:
            cs->registers.pc = cs->registers.mdr;
            return CS_OP_DO_FETCH;
    }
}

/* CS2010 CLC */
int cs2010_op_clc_stepper(cs_machine *cs) {
    cs->registers.sr &= ~(CS_SR_C);
    return CS_OP_DO_FETCH;
}

/* CS2010 SEC */
int cs2010_op_sec_stepper(cs_machine *cs) {
    cs->registers.sr |= CS_SR_C;
    return CS_OP_DO_FETCH;
}

/* CS2010 ROR */
static void cs_op_set_ror_flags(cs_machine *cs, unsigned char r, unsigned char a_7, unsigned char a_0,
                                unsigned char c_in) {
    /* bit 76543210
    SR  =  0000VNZC   where
    V: 2's complement overflow
    N: negative sign
    Z: zero
    C: carry (unsigned overflow) */

    cs->registers.sr = (a_7 ^ c_in) << CS_SR_V_OFFSET |
                       c_in << CS_SR_N_OFFSET | /* same as BIT_AT(r, 7) << CS_SR_N_OFFSET */
                       (!r) << CS_SR_Z_OFFSET | a_0 << CS_SR_C_OFFSET;
}

int cs2010_op_ror_stepper(cs_machine *cs) {
    unsigned char *dst_register = cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
    unsigned char  a_7          = BIT_AT(*dst_register, 7);
    unsigned char  a_0          = BIT_AT(*dst_register, 0);
    unsigned char  c_in         = BIT_AT(cs->registers.sr, CS_SR_C_OFFSET);
    cs->registers.ac            = (*dst_register >> 1) | (c_in << 7);
    cs_op_set_ror_flags(cs, cs->registers.ac, a_7, a_0, c_in);
    *dst_register = cs->registers.ac;
    return CS_OP_DO_FETCH;
}

int cs2010_op_ror_microstepper(cs_machine *cs) {
    unsigned char *dst_register = cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
    unsigned char  a_7;
    unsigned char  a_0;
    unsigned char  c_in;

    switch (cs->microop) {
        case 0:
            a_7              = BIT_AT(*dst_register, 7);
            a_0              = BIT_AT(*dst_register, 0);
            c_in             = BIT_AT(cs->registers.sr, CS_SR_C_OFFSET);
            cs->registers.ac = (*dst_register >> 1) | (c_in << 7);
            cs_op_set_ror_flags(cs, cs->registers.ac, a_7, a_0, c_in);
            return CS_OP_DO_MICROFETCH;
        case 1:
        default:
            *dst_register = cs->registers.ac;
            return CS_OP_DO_FETCH;
    }
}

/* CS2010 ROL */
static void cs_op_set_rol_flags(cs_machine *cs, unsigned char r, unsigned char a_7, unsigned char a_6,
                                unsigned char c_in) {
    (void)c_in;

    /* bit 76543210
    SR  =  0000VNZC   where
    V: 2's complement overflow
    N: negative sign
    Z: zero
    C: carry (unsigned overflow) */

    cs->registers.sr = (a_7 ^ a_6) << CS_SR_V_OFFSET |
                       a_6 << CS_SR_N_OFFSET | /* same as BIT_AT(r, 7) << CS_SR_N_OFFSET */
                       (!r) << CS_SR_Z_OFFSET | a_7 << CS_SR_C_OFFSET;
}

int cs2010_op_rol_stepper(cs_machine *cs) {
    unsigned char *dst_register = cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
    unsigned char  a_7          = BIT_AT(*dst_register, 7);
    unsigned char  a_6          = BIT_AT(*dst_register, 6);
    unsigned char  c_in         = BIT_AT(cs->registers.sr, CS_SR_C_OFFSET);
    cs->registers.ac            = (*dst_register << 1) | c_in;
    cs_op_set_rol_flags(cs, cs->registers.ac, a_7, a_6, c_in);
    *dst_register = cs->registers.ac;
    return CS_OP_DO_FETCH;
}

int cs2010_op_rol_microstepper(cs_machine *cs) {
    unsigned char *dst_register = cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
    unsigned char  a_7;
    unsigned char  a_6;
    unsigned char  c_in;

    switch (cs->microop) {
        case 0:
            a_7              = BIT_AT(*dst_register, 7);
            a_6              = BIT_AT(*dst_register, 6);
            c_in             = BIT_AT(cs->registers.sr, CS_SR_C_OFFSET);
            cs->registers.ac = (*dst_register << 1) | c_in;
            cs_op_set_rol_flags(cs, cs->registers.ac, a_7, a_6, c_in);
            return CS_OP_DO_MICROFETCH;
        case 1:
        default:
            *dst_register = cs->registers.ac;
            return CS_OP_DO_FETCH;
    }
}

/* CS2010 ADDI */
int cs2010_op_addi_stepper(cs_machine *cs) {
    return cs_op_arithmetic_stepper(cs, cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)],
                                    CS_GET_ARG_B(cs->registers.ir), false);
}

int cs2010_op_addi_microstepper(cs_machine *cs) {
    return cs_op_arithmetic_microstepper(cs, cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)],
                                         CS_GET_ARG_B(cs->registers.ir), false);
}

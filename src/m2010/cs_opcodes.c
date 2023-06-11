/** @file cs_opcodes.c */

#include <stdint.h>

#include "../../include/asm2010.h"

#include "../utils.h"

#include "cs_instructions.h"

#include "cs_opcodes.h"

/* Shared I/O helpers */
unsigned char cs_read_input(cs_machine *cs, size_t offset) {
    size_t value = cs->io_read_fn(offset);
    if (value > UINT8_MAX) {
        value = cs->memory.ram[offset];
    }
    return value;
}

void cs_write_output(cs_machine *cs, size_t offset, unsigned char content) {
    if (!cs->io_write_fn(offset, content)) {
        cs->memory.ram[offset] = content;
    }
}

/* Shared BRXX */
static bool cs_op_is_jmp_condition_met(cs_machine *cs) {
    bool is_jmp_condition_met = false;
    switch (CS_GET_JMP_CONDITION(cs->registers.ir)) {
        case CS_JMP_COND_EQUAL:
            is_jmp_condition_met = cs->registers.sr & CS_SR_Z;
            break;
        case CS_JMP_COND_LOWER:
            is_jmp_condition_met = cs->registers.sr & CS_SR_C;
            break;
        case CS_JMP_COND_OVERFLOW:
            is_jmp_condition_met = cs->registers.sr & CS_SR_V;
            break;
        case CS_JMP_COND_SLOWER:
            is_jmp_condition_met = !!(cs->registers.sr & CS_SR_V) ^ !!(cs->registers.sr & CS_SR_N);
            break;
        default:
            break;
    }
    return is_jmp_condition_met;
}

int cs_op_brxx_stepper(cs_machine *cs) {
    if (cs_op_is_jmp_condition_met(cs)) {
        cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
        cs->registers.pc = cs->registers.ac;
    }
    return CS_OP_DO_FETCH;
}

int cs_op_brxx_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            if (cs_op_is_jmp_condition_met(cs)) {
                cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
                return CS_OP_DO_MICROFETCH;
            } else {
                return CS_OP_DO_FETCH;
            }
            break;
        case 1:
        default:
            cs->registers.pc = cs->registers.ac;
            return CS_OP_DO_FETCH;
    }
}

/* Shared JMP */
int cs_op_jmp_stepper(cs_machine *cs) {
    cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
    cs->registers.pc = cs->registers.ac;
    return CS_OP_DO_FETCH;
}

int cs_op_jmp_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
            return CS_OP_DO_MICROFETCH;
        case 1:
        default:
            cs->registers.pc = cs->registers.ac;
            return CS_OP_DO_FETCH;
    }
}

/* Shared arithmetic helpers */
static void cs_op_perform_arithmetic(cs_machine *cs, unsigned char a, unsigned char b, bool is_substracting) {
    /* bit 76543210
    SR  =  0000VNZC   where
    V: 2's complement overflow
    N: negative sign
    Z: zero
    C: carry (unsigned overflow)

    a: operand 1
    b: operand 2
    r: result */

    if (is_substracting) {
        b = -b;
    }
    cs->registers.ac = a + b;

    unsigned char a_7 = BIT_AT(a, 7);
    unsigned char b_7 = BIT_AT(b, 7);
    unsigned char c_7 = BIT_AT(cs->registers.ac, 7);
    cs->registers.sr  = (a_7 == b_7 && a_7 != c_7) << CS_SR_V_OFFSET | /* sign(a) == sign(b) AND sign(a) != sign(c) */
                       (c_7) << CS_SR_N_OFFSET | (!cs->registers.ac) << CS_SR_Z_OFFSET |
                       (is_substracting ? (cs->registers.ac >= a) : (cs->registers.ac < a)) << CS_SR_C_OFFSET;
}

int cs_op_arithmetic_stepper(cs_machine *cs, unsigned char *dst_register, unsigned char b, bool is_substracting) {
    unsigned char a = *dst_register;
    cs_op_perform_arithmetic(cs, a, b, is_substracting);
    *dst_register = cs->registers.ac;
    return CS_OP_DO_FETCH;
}

int cs_op_arithmetic_microstepper(cs_machine *cs, unsigned char *dst_register, unsigned char b, bool is_substracting) {
    unsigned char a;

    switch (cs->microop) {
        case 0:
            a = *dst_register;
            cs_op_perform_arithmetic(cs, a, b, is_substracting);
            return CS_OP_DO_MICROFETCH;
        case 1:
        default:
            *dst_register = cs->registers.ac;
            return CS_OP_DO_FETCH;
    }
}

/* Shared ADD */
int cs_op_add_stepper(cs_machine *cs) {
    return cs_op_arithmetic_stepper(cs, cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)],
                                    *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)], false);
}

int cs_op_add_microstepper(cs_machine *cs) {
    return cs_op_arithmetic_microstepper(cs, cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)],
                                         *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)], false);
}

/* Shared SUB */
int cs_op_sub_stepper(cs_machine *cs) {
    return cs_op_arithmetic_stepper(cs, cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)],
                                    *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)], true);
}

int cs_op_sub_microstepper(cs_machine *cs) {
    return cs_op_arithmetic_microstepper(cs, cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)],
                                         *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)], true);
}

/* Shared CP */
int cs_op_cp_stepper(cs_machine *cs) {
    unsigned char a = *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
    unsigned char b = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
    cs_op_perform_arithmetic(cs, a, b, true);
    return CS_OP_DO_FETCH;
}

/* Shared MOV */
int cs_op_mov_stepper(cs_machine *cs) {
    cs->registers.ac                                       = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
    *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs->registers.ac;
    return CS_OP_DO_FETCH;
}

int cs_op_mov_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 1:
        default:
            *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs->registers.ac;
            return CS_OP_DO_FETCH;
    }
}

/* Shared STOP */
int cs_op_stop_stepper(cs_machine *cs) {
    cs->stopped = true;
    return CS_OP_DO_NOTHING;
}

/* Shared SUBI */
int cs_op_subi_stepper(cs_machine *cs) {
    return cs_op_arithmetic_stepper(cs, cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)],
                                    CS_GET_ARG_B(cs->registers.ir), true);
}

int cs_op_subi_microstepper(cs_machine *cs) {
    return cs_op_arithmetic_microstepper(cs, cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)],
                                         CS_GET_ARG_B(cs->registers.ir), true);
}

/* Shared CPI */
int cs_op_cpi_stepper(cs_machine *cs) {
    unsigned char a = *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
    unsigned char b = CS_GET_ARG_B(cs->registers.ir);
    cs_op_perform_arithmetic(cs, a, b, true);
    return CS_OP_DO_FETCH;
}

/* Shared LDI */
int cs_op_ldi_stepper(cs_machine *cs) {
    cs->registers.ac                                       = CS_GET_ARG_B(cs->registers.ir);
    *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs->registers.ac;
    return CS_OP_DO_FETCH;
}

int cs_op_ldi_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
            return CS_OP_DO_MICROFETCH;
        case 1:
        default:
            *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs->registers.ac;
            return CS_OP_DO_FETCH;
    }
}

/* Shared no-op */
int cs_op_noop_stepper(cs_machine *cs) {
    (void)cs;
    return CS_OP_DO_FETCH;
}

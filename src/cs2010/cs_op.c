/** @file cs_op.c */

#include <stdbool.h>

#include "cs_instructions.h"
#include "cs_op.h"

inline void cs_op_ram_set(cs2010 *cs, uint8_t address, uint8_t value) {
    cs->mem.ram[address] = value;
    cs->last_ram_change_address = address;
}

void cs_op_st_stepper(cs2010 *cs) {
    cs_op_ram_set(cs, *cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)], *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)]);
    cs_fetch(cs);
}

void cs_op_st_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.ac = *cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
        cs_microfetch(cs);
        break;
    case 1:
        cs->reg.mar = cs->reg.ac;
        cs->reg.ac = *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
        cs_microfetch(cs);
        break;
    case 2:
        cs->reg.mdr = cs->reg.ac;
        cs_microfetch(cs);
        break;
    case 3:
    default:
        cs_op_ram_set(cs, cs->reg.mar, cs->reg.mdr);
        cs_fetch(cs);
        break;
    }
}

void cs_op_ld_stepper(cs2010 *cs) {
    *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->mem.ram[CS_GET_ARG_B(cs->reg.ir)];
    cs_fetch(cs);
}

void cs_op_ld_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.ac = *cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
        cs_microfetch(cs);
        break;
    case 1:
        cs->reg.mar = cs->reg.ac;
        cs_microfetch(cs);
        break;
    case 2:
        cs->reg.mdr = cs->mem.ram[cs->reg.mar];
        cs_microfetch(cs);
        break;
    case 3:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.mdr;
        cs_fetch(cs);
        break;
    }
}

void cs_op_sts_stepper(cs2010 *cs) {
    cs_op_ram_set(cs, CS_GET_ARG_B(cs->reg.ir), *cs->reg.regfile[CS_GET_ARG_A(cs->reg.ir)]);
    cs_fetch(cs);
}

void cs_op_sts_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.ac = CS_GET_ARG_B(cs->reg.ir);
        cs_microfetch(cs);
        break;
    case 1:
        cs->reg.mar = cs->reg.ac;
        cs->reg.ac = *cs->reg.regfile[CS_GET_ARG_A(cs->reg.ir)];
        cs_microfetch(cs);
        break;
    case 2:
        cs->reg.mdr = cs->reg.ac;
        cs_microfetch(cs);
        break;
    case 3:
    default:
        cs_op_ram_set(cs, cs->reg.mar, cs->reg.mdr);
        cs_fetch(cs);
        break;
    }
}

void cs_op_lds_stepper(cs2010 *cs) {
    *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->mem.ram[CS_GET_ARG_B(cs->reg.ir)];
    cs_fetch(cs);
}

void cs_op_lds_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.ac = CS_GET_ARG_B(cs->reg.ir);
        cs_microfetch(cs);
        break;
    case 1:
        cs->reg.mar = cs->reg.ac;
        cs_microfetch(cs);
        break;
    case 2:
        cs->reg.mdr = cs->mem.ram[cs->reg.mar];
        cs_microfetch(cs);
        break;
    case 3:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.mdr;
        cs_fetch(cs);
        break;
    }
}

void cs_op_call_stepper(cs2010 *cs) {
    cs_op_ram_set(cs, cs->reg.sp, cs->reg.pc);
    cs->reg.sp--;
    cs->reg.pc = CS_GET_ARG_B(cs->reg.ir);
    cs_fetch(cs);
}

void cs_op_call_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.mdr = cs->reg.pc;
        cs->reg.ac = CS_GET_ARG_B(cs->reg.ir);
        cs_microfetch(cs);
        break;
    case 1:
        cs->reg.mar = cs->reg.sp--;
        cs_microfetch(cs);
        break;
    case 2:
    default:
        cs->reg.pc = cs->reg.ac;
        cs_op_ram_set(cs, cs->reg.mar, cs->reg.mdr);
        cs_fetch(cs);
        break;
    }
}

void cs_op_ret_stepper(cs2010 *cs) {
    cs->reg.pc = cs->mem.ram[++cs->reg.sp];
    cs_fetch(cs);
}

void cs_op_ret_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.sp++;
        cs_microfetch(cs);
        break;
    case 1:
        cs->reg.mar = cs->reg.sp;
        cs_microfetch(cs);
        break;
    case 2:
        cs->reg.mdr = cs->mem.ram[cs->reg.mar];
        cs_microfetch(cs);
        break;
    case 3:
    default:
        cs->reg.pc = cs->reg.mdr;
        cs_fetch(cs);
        break;
    }
}

inline bool cs_op_check_jmp(cs2010 *cs) {
    bool jump = false;
    switch (CS_GET_JMP_CONDITION(cs->reg.ir)) {
    case CS_JMP_COND_EQUAL:
        jump = cs->reg.sr & CS_SR_Z;
        break;
    case CS_JMP_COND_LOWER:
        jump = cs->reg.sr & CS_SR_C;
        break;
    case CS_JMP_COND_OVERFLOW:
        jump = cs->reg.sr & CS_SR_V;
        break;
    case CS_JMP_COND_SLOWER:
        jump = !!(cs->reg.sr & CS_SR_V) ^ !!(cs->reg.sr & CS_SR_N);
        break;
    default:
        break;
    }
    return jump;
}

void cs_op_brxx_stepper(cs2010 *cs) {
    if (cs_op_check_jmp(cs)) {
        cs->reg.pc = CS_GET_ARG_B(cs->reg.ir);
    }
    cs_fetch(cs);
}

void cs_op_brxx_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        if (cs_op_check_jmp(cs)) {
            cs->reg.ac = CS_GET_ARG_B(cs->reg.ir);
            cs_microfetch(cs);
        } else {
            cs_fetch(cs);
        }
        break;
    case 1:
    default:
        cs->reg.pc = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

void cs_op_jmp_stepper(cs2010 *cs) {
    cs->reg.pc = CS_GET_ARG_B(cs->reg.ir);
    cs_fetch(cs);
}

void cs_op_jmp_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.ac = CS_GET_ARG_B(cs->reg.ir);
        cs_microfetch(cs);
        break;
    case 1:
    default:
        cs->reg.pc = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

inline void cs_op_set_arithmetic_flags(cs2010 *cs, uint8_t a, uint8_t b, uint8_t c) {
    /* bit 76543210
    SR  =  0000VNZC
    V: 2's complement overflow
    N: negative sign
    Z: zero
    C: carry (unsigned overflow) */
    uint8_t a_7 = a >> 7;
    uint8_t b_7 = b >> 7;
    uint8_t c_7 = c >> 7;
    cs->reg.sr =
        ((!(a_7 ^ b_7)) & (a_7 ^ c_7)) << 3 |   /* sign(a) == sign(b) AND sign(a) != sign(c) */
        (c_7) << 2 |                            /* (c >> 7) << 2 */
        (!c) << 1 |
        (c < a);
}

void cs_op_add_stepper(cs2010 *cs) {
    uint8_t *a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
    uint8_t *b = cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
    uint8_t c = *a + *b;
    cs_op_set_arithmetic_flags(cs, *a, *b, c);
    *a = c;
    cs_fetch(cs);
}

void cs_op_add_microstepper(cs2010 *cs) {
    uint8_t *a;
    uint8_t *b;
    uint8_t c;

    switch (cs->microop) {
    case 0:
        a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
        b = cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
        c = *a + *b;
        cs_op_set_arithmetic_flags(cs, *a, *b, c);
        cs->reg.ac = c;
        cs_microfetch(cs);
    case 1:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

void cs_op_sub_stepper(cs2010 *cs) {
    uint8_t *a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
    uint8_t b = -*cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
    uint8_t c = *a + b;
    cs_op_set_arithmetic_flags(cs, *a, b, c);
    *a = c;
    cs_fetch(cs);
}

void cs_op_sub_microstepper(cs2010 *cs) {
    uint8_t *a;
    uint8_t b;
    uint8_t c;

    switch (cs->microop) {
    case 0:
        a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
        b = -*cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
        c = *a + b;
        cs_op_set_arithmetic_flags(cs, *a, b, c);
        cs->reg.ac = c;
        cs_microfetch(cs);
    case 1:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

void cs_op_cp_stepper(cs2010 *cs) {
    uint8_t *a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
    uint8_t b = -*cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
    uint8_t c = *a + b;
    cs_op_set_arithmetic_flags(cs, *a, b, c);
    cs_fetch(cs);
}

void cs_op_mov_stepper(cs2010 *cs) {
    *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = *cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
    cs_fetch(cs);
}

void cs_op_mov_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.ac = *cs->reg.regfile[CS_GET_REG_B(cs->reg.ir)];
        cs_microfetch(cs);
        break;
    case 1:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

void cs_op_clc_stepper(cs2010 *cs) {
    cs->reg.sr &= ~(CS_SR_C);
    cs_fetch(cs);
}

void cs_op_sec_stepper(cs2010 *cs) {
    cs->reg.sr |= CS_SR_C;
    cs_fetch(cs);
}

inline void cs_op_set_ror_flags(cs2010 *cs, uint8_t r, uint8_t r_7, uint8_t r_0, uint8_t c_in) {
    cs->reg.sr =
        (r_7 ^ c_in) << 3 |
        c_in << 2 | /* same as !!(r & (1 << 7)) << 2 */
        (!r) << 1 |
        r_0;
}

void cs_op_ror_stepper(cs2010 *cs) {
    uint8_t *r = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
    uint8_t r_7 = *r >> 7;
    uint8_t r_0 = *r & 1u;
    uint8_t c_in = !!(cs->reg.sr & CS_SR_C);
    *r = (*r >> 1) | (c_in << 7);
    cs_op_set_ror_flags(cs, *r, r_7, r_0, c_in);
    cs_fetch(cs);
}

void cs_op_ror_microstepper(cs2010 *cs) {
    uint8_t *r;
    uint8_t r_7;
    uint8_t r_0;
    uint8_t c_in;

    switch (cs->microop) {
    case 0:
        r = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
        r_7 = *r >> 7;
        r_0 = *r & 1u;
        c_in = !!(cs->reg.sr & CS_SR_C);
        cs->reg.ac = (*r >> 1) | (c_in << 7);
        cs_op_set_ror_flags(cs, *r, r_7, r_0, c_in);
        cs_microfetch(cs);
        break;
    case 1:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

inline void cs_op_set_rol_flags(cs2010 *cs, uint8_t r, uint8_t r_7, uint8_t r_6, uint8_t r_0, uint8_t c_in) {
    cs->reg.sr =
        (r_7 ^ r_6) << 3 |
        r_6 << 2 | /* same as !!(r & (1 << 6)) << 2 */
        (!r) << 1 |
        r_7;
}

void cs_op_rol_stepper(cs2010 *cs) {
    uint8_t *r = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
    uint8_t r_7 = *r >> 7;
    uint8_t r_6 = !!(*r & (1u << 6));
    uint8_t r_0 = *r & 1u;
    uint8_t c_in = !!(cs->reg.sr & CS_SR_C);
    *r = (*r << 1) | c_in;
    cs_op_set_rol_flags(cs, *r, r_7, r_6, r_0, c_in);
    cs_fetch(cs);
}

void cs_op_rol_microstepper(cs2010 *cs) {
    uint8_t *r;
    uint8_t r_7;
    uint8_t r_6;
    uint8_t r_0;
    uint8_t c_in;

    switch (cs->microop) {
    case 0:
        r = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
        r_7 = *r >> 7;
        r_6 = !!(*r & (1u << 6));
        r_0 = *r & 1u;
        c_in = !!(cs->reg.sr & CS_SR_C);
        cs->reg.ac = (*r << 1) | c_in;
        cs_op_set_rol_flags(cs, *r, r_7, r_6, r_0, c_in);
        cs_microfetch(cs);
        break;
    case 1:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

void cs_op_stop_stepper(cs2010 *cs) {
    cs->stopped = true;
}

void cs_op_addi_stepper(cs2010 *cs) {
    uint8_t *a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
    uint8_t b = CS_GET_ARG_B(cs->reg.ir);
    uint8_t c = *a + b;
    cs_op_set_arithmetic_flags(cs, *a, b, c);
    *a = c;
    cs_fetch(cs);
}

void cs_op_addi_microstepper(cs2010 *cs) {
    uint8_t *a;
    uint8_t b;
    uint8_t c;

    switch (cs->microop) {
    case 0:
        a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
        b = CS_GET_ARG_B(cs->reg.ir);
        c = *a + b;
        cs_op_set_arithmetic_flags(cs, *a, b, c);
        cs->reg.ac = c;
        cs_microfetch(cs);
        break;
    case 1:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

void cs_op_subi_stepper(cs2010 *cs) {
    uint8_t *a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
    uint8_t b = CS_GET_ARG_B(cs->reg.ir);
    uint8_t c = *a - b;
    cs_op_set_arithmetic_flags(cs, *a, b, c);
    *a = c;
    cs_fetch(cs);
}

void cs_op_subi_microstepper(cs2010 *cs) {
    uint8_t *a;
    uint8_t b;
    uint8_t c;

    switch (cs->microop) {
    case 0:
        a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
        b = CS_GET_ARG_B(cs->reg.ir);
        c = *a - b;
        cs_op_set_arithmetic_flags(cs, *a, b, c);
        cs->reg.ac = c;
        cs_microfetch(cs);
        break;
    case 1:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}

void cs_op_cpi_stepper(cs2010 *cs) {
    uint8_t *a = cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
    uint8_t b = CS_GET_ARG_B(cs->reg.ir);
    uint8_t c = *a - b;
    cs_op_set_arithmetic_flags(cs, *a, b, c);
    cs_fetch(cs);
}

void cs_op_ldi_stepper(cs2010 *cs) {
    *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = CS_GET_ARG_B(cs->reg.ir);
    cs_fetch(cs);
}

void cs_op_ldi_microstepper(cs2010 *cs) {
    switch (cs->microop) {
    case 0:
        cs->reg.ac = CS_GET_ARG_B(cs->reg.ir);
        cs_microfetch(cs);
        break;
    case 1:
    default:
        *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->reg.ac;
        cs_fetch(cs);
        break;
    }
}
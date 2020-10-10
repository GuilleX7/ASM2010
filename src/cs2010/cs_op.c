/** @file cs_op.c */

#include "cs_instructions.h"
#include "cs_op.h"

void cs_op_st_stepper(cs2010 *cs) {
    cs->mem.ram[CS_GET_ARG_B(cs->reg.ir)] = *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)];
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
        cs->mem.ram[cs->reg.mar] = cs->reg.mdr;
        cs_fetch(cs);
        break;
    }
}

void cs_op_ld_stepper(cs2010 *cs) {
    *cs->reg.regfile[CS_GET_REG_A(cs->reg.ir)] = cs->mem.ram[CS_GET_ARG_B(cs->reg.ir)];
    cs_fetch(cs);
}

void cs_op_ld_microstepper(cs2010 *cs) {

}

void cs_op_sts_stepper(cs2010 *cs) {

}

void cs_op_sts_microstepper(cs2010 *cs) {

}

void cs_op_lds_stepper(cs2010 *cs) {

}

void cs_op_lds_microstepper(cs2010 *cs) {

}

void cs_op_call_stepper(cs2010 *cs) {

}

void cs_op_call_microstepper(cs2010 *cs) {

}

void cs_op_ret_stepper(cs2010 *cs) {

}

void cs_op_ret_microstepper(cs2010 *cs) {

}

void cs_op_brxx_stepper(cs2010 *cs) {

}

void cs_op_brxx_microstepper(cs2010 *cs) {

}

void cs_op_jmp_stepper(cs2010 *cs) {

}

void cs_op_jmp_microstepper(cs2010 *cs) {

}

void cs_op_add_stepper(cs2010 *cs) {

}

void cs_op_add_microstepper(cs2010 *cs) {

}

void cs_op_sub_stepper(cs2010 *cs) {

}

void cs_op_sub_microstepper(cs2010 *cs) {

}

void cs_op_cp_stepper(cs2010 *cs) {

}

void cs_op_cp_microstepper(cs2010 *cs) {

}

void cs_op_mov_stepper(cs2010 *cs) {

}

void cs_op_mov_microstepper(cs2010 *cs) {

}

void cs_op_clc_stepper(cs2010 *cs) {

}

void cs_op_clc_microstepper(cs2010 *cs) {

}

void cs_op_sec_stepper(cs2010 *cs) {

}

void cs_op_sec_microstepper(cs2010 *cs) {

}

void cs_op_ror_stepper(cs2010 *cs) {

}

void cs_op_ror_microstepper(cs2010 *cs) {

}

void cs_op_rol_stepper(cs2010 *cs) {

}

void cs_op_rol_microstepper(cs2010 *cs) {

}

void cs_op_stop_stepper(cs2010 *cs) {

}

void cs_op_stop_microstepper(cs2010 *cs) {

}

void cs_op_addi_stepper(cs2010 *cs) {

}

void cs_op_addi_microstepper(cs2010 *cs) {

}

void cs_op_subi_stepper(cs2010 *cs) {

}

void cs_op_subi_microstepper(cs2010 *cs) {

}

void cs_op_cpi_stepper(cs2010 *cs) {

}

void cs_op_cpi_microstepper(cs2010 *cs) {

}

void cs_op_ldi_stepper(cs2010 *cs) {

}

void cs_op_ldi_microstepper(cs2010 *cs) {

}
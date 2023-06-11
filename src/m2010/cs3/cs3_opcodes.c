/** @file cs3_opcodes.c */

#include "../../../include/asm2010.h"

#include "../cs_instructions.h"
#include "../cs_opcodes.h"

#include "cs3_opcodes.h"

/* CS3 ST */
int cs3_op_st_stepper(cs_machine *cs) {
    cs->registers.mar = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
    cs->registers.ac  = *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
    cs_write_output(cs, cs->registers.mar, cs->registers.ac);
    return CS_OP_DO_FETCH;
}

int cs3_op_st_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.ac;
            cs->registers.ac  = *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 2:
        default:
            cs_write_output(cs, cs->registers.mar, cs->registers.ac);
            return CS_OP_DO_FETCH;
    }
}

/* CS3 LD */
int cs3_op_ld_stepper(cs_machine *cs) {
    cs->registers.ac                                       = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
    cs->registers.mar                                      = cs->registers.ac;
    *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs_read_input(cs, cs->registers.mar);
    return CS_OP_DO_FETCH;
}

int cs3_op_ld_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = *cs->registers.regfile[CS_GET_REG_B(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.ac;
            return CS_OP_DO_MICROFETCH;
        case 2:
        default:
            *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs_read_input(cs, cs->registers.mar);
            return CS_OP_DO_FETCH;
    }
}

/* CS3 STS */
int cs3_op_sts_stepper(cs_machine *cs) {
    cs->registers.mar = CS_GET_ARG_B(cs->registers.ir);
    cs->registers.ac  = *cs->registers.regfile[CS_GET_ARG_A(cs->registers.ir)];
    cs_write_output(cs, cs->registers.mar, cs->registers.ac);
    return CS_OP_DO_FETCH;
}

int cs3_op_sts_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.ac;
            cs->registers.ac  = *cs->registers.regfile[CS_GET_ARG_A(cs->registers.ir)];
            return CS_OP_DO_MICROFETCH;
        case 2:
        default:
            cs_write_output(cs, cs->registers.mar, cs->registers.ac);
            return CS_OP_DO_FETCH;
    }
}

/* CS3 LDS */
int cs3_op_lds_stepper(cs_machine *cs) {
    cs->registers.ac                                       = CS_GET_ARG_B(cs->registers.ir);
    cs->registers.mar                                      = cs->registers.ac;
    *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs_read_input(cs, cs->registers.mar);
    return CS_OP_DO_FETCH;
}

int cs3_op_lds_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac = CS_GET_ARG_B(cs->registers.ir);
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.ac;
            return CS_OP_DO_MICROFETCH;
        case 2:
        default:
            *cs->registers.regfile[CS_GET_REG_A(cs->registers.ir)] = cs_read_input(cs, cs->registers.mar);
            return CS_OP_DO_FETCH;
    }
}

/* CS3 CALL */
int cs3_op_call_stepper(cs_machine *cs) {
    cs->registers.ac                  = CS_GET_ARG_B(cs->registers.ir);
    cs->registers.mar                 = cs->registers.sp--;
    cs->memory.ram[cs->registers.mar] = cs->registers.pc;
    cs->registers.pc                  = cs->registers.ac;
    return CS_OP_DO_FETCH;
}

int cs3_op_call_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.ac  = CS_GET_ARG_B(cs->registers.ir);
            cs->registers.mar = cs->registers.sp--;
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->memory.ram[cs->registers.mar] = cs->registers.pc;
            return CS_OP_DO_MICROFETCH;
        case 2:
        default:
            cs->registers.pc = cs->registers.ac;
            return CS_OP_DO_FETCH;
    }
}

/* CS3 RET */
int cs3_op_ret_stepper(cs_machine *cs) {
    cs->registers.mar = ++cs->registers.sp;
    cs->registers.pc  = cs->memory.ram[cs->registers.mar];
    return CS_OP_DO_FETCH;
}

int cs3_op_ret_microstepper(cs_machine *cs) {
    switch (cs->microop) {
        case 0:
            cs->registers.sp++;
            return CS_OP_DO_MICROFETCH;
        case 1:
            cs->registers.mar = cs->registers.sp;
            return CS_OP_DO_MICROFETCH;
        case 2:
        default:
            cs->registers.pc = cs->memory.ram[cs->registers.mar];
            return CS_OP_DO_FETCH;
    }
}

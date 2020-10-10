/** @file cs_op.h */

#ifndef CS_OP_H
#define CS_OP_H

#include "cs2010.h"

void cs_op_st_stepper(cs2010 *cs);

void cs_op_st_microstepper(cs2010 *cs);

void cs_op_ld_stepper(cs2010 *cs);

void cs_op_ld_microstepper(cs2010 *cs);

void cs_op_sts_stepper(cs2010 *cs);

void cs_op_sts_microstepper(cs2010 *cs);

void cs_op_lds_stepper(cs2010 *cs);

void cs_op_lds_microstepper(cs2010 *cs);

void cs_op_call_stepper(cs2010 *cs);

void cs_op_call_microstepper(cs2010 *cs);

void cs_op_ret_stepper(cs2010 *cs);

void cs_op_ret_microstepper(cs2010 *cs);

void cs_op_brxx_stepper(cs2010 *cs);

void cs_op_brxx_microstepper(cs2010 *cs);

void cs_op_jmp_stepper(cs2010 *cs);

void cs_op_jmp_microstepper(cs2010 *cs);

void cs_op_add_stepper(cs2010 *cs);

void cs_op_add_microstepper(cs2010 *cs);

void cs_op_sub_stepper(cs2010 *cs);

void cs_op_sub_microstepper(cs2010 *cs);

void cs_op_cp_stepper(cs2010 *cs);

void cs_op_mov_stepper(cs2010 *cs);

void cs_op_mov_microstepper(cs2010 *cs);

void cs_op_clc_stepper(cs2010 *cs);

void cs_op_sec_stepper(cs2010 *cs);

void cs_op_ror_stepper(cs2010 *cs);

void cs_op_ror_microstepper(cs2010 *cs);

void cs_op_rol_stepper(cs2010 *cs);

void cs_op_rol_microstepper(cs2010 *cs);

void cs_op_stop_stepper(cs2010 *cs);

void cs_op_addi_stepper(cs2010 *cs);

void cs_op_addi_microstepper(cs2010 *cs);

void cs_op_subi_stepper(cs2010 *cs);

void cs_op_subi_microstepper(cs2010 *cs);

void cs_op_cpi_stepper(cs2010 *cs);

void cs_op_ldi_stepper(cs2010 *cs);

void cs_op_ldi_microstepper(cs2010 *cs);

#endif /* CS_OP_H */
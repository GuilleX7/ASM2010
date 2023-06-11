/** @file cs2010_opcodes.h */

#ifndef CS2010_OPCODES_H
#define CS2010_OPCODES_H

#include "../cs.h"

/* CS2010 implementation-specific opcode */
int cs2010_op_st_stepper(cs_machine *cs);
int cs2010_op_st_microstepper(cs_machine *cs);
int cs2010_op_ld_stepper(cs_machine *cs);
int cs2010_op_ld_microstepper(cs_machine *cs);
int cs2010_op_sts_stepper(cs_machine *cs);
int cs2010_op_sts_microstepper(cs_machine *cs);
int cs2010_op_lds_stepper(cs_machine *cs);
int cs2010_op_lds_microstepper(cs_machine *cs);
int cs2010_op_call_stepper(cs_machine *cs);
int cs2010_op_call_microstepper(cs_machine *cs);
int cs2010_op_ret_stepper(cs_machine *cs);
int cs2010_op_ret_microstepper(cs_machine *cs);
/* CS2010 specific opcode */
int cs2010_op_clc_stepper(cs_machine *cs);
int cs2010_op_sec_stepper(cs_machine *cs);
int cs2010_op_ror_stepper(cs_machine *cs);
int cs2010_op_ror_microstepper(cs_machine *cs);
int cs2010_op_rol_stepper(cs_machine *cs);
int cs2010_op_rol_microstepper(cs_machine *cs);
int cs2010_op_addi_stepper(cs_machine *cs);
int cs2010_op_addi_microstepper(cs_machine *cs);

#endif /* CS2010_OPCODES_H */

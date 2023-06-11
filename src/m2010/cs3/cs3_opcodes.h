/** @file cs3_opcodes.h */

#ifndef CS3_OPCODES_H
#define CS3_OPCODES_H

#include "../cs.h"

/* CS3 implementation-specific opcode */
int cs3_op_st_stepper(cs_machine *cs);
int cs3_op_st_microstepper(cs_machine *cs);
int cs3_op_ld_stepper(cs_machine *cs);
int cs3_op_ld_microstepper(cs_machine *cs);
int cs3_op_sts_stepper(cs_machine *cs);
int cs3_op_sts_microstepper(cs_machine *cs);
int cs3_op_lds_stepper(cs_machine *cs);
int cs3_op_lds_microstepper(cs_machine *cs);
int cs3_op_call_stepper(cs_machine *cs);
int cs3_op_call_microstepper(cs_machine *cs);
int cs3_op_ret_stepper(cs_machine *cs);
int cs3_op_ret_microstepper(cs_machine *cs);

#endif /* CS3_OPCODES_H */

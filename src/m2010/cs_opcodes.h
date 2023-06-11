/** @file cs_opcodes.h */

#ifndef CS_OPCODES_H
#define CS_OPCODES_H

#include "cs.h"

#define CS_OP_DO_NOTHING    0
#define CS_OP_DO_FETCH      1
#define CS_OP_DO_MICROFETCH 2

#define CS_OP_NOOP                                                                                                     \
    {                                                                                                                  \
        cs_op_noop_stepper, cs_op_noop_stepper,                                                                        \
            {                                                                                                          \
                CS_SIGNALS_FETCH, CS_SIGNALS_NONE, CS_SIGNALS_NONE, CS_SIGNALS_NONE, CS_SIGNALS_NONE,                  \
            },                                                                                                         \
    }

/* Shared I/O helpers */
unsigned char cs_read_input(cs_machine *cs, size_t offset);
void          cs_write_output(cs_machine *cs, size_t offset, unsigned char content);

/* Shared arithmetic helpers */
int cs_op_arithmetic_stepper(cs_machine *cs, unsigned char *dst_register, unsigned char b, bool is_substracting);
int cs_op_arithmetic_microstepper(cs_machine *cs, unsigned char *dst_register, unsigned char b, bool is_substracting);

/* Shared opcode implementation */
int cs_op_brxx_stepper(cs_machine *cs);
int cs_op_brxx_microstepper(cs_machine *cs);
int cs_op_jmp_stepper(cs_machine *cs);
int cs_op_jmp_microstepper(cs_machine *cs);
int cs_op_add_stepper(cs_machine *cs);
int cs_op_add_microstepper(cs_machine *cs);
int cs_op_sub_stepper(cs_machine *cs);
int cs_op_sub_microstepper(cs_machine *cs);
int cs_op_cp_stepper(cs_machine *cs);
int cs_op_mov_stepper(cs_machine *cs);
int cs_op_mov_microstepper(cs_machine *cs);
int cs_op_stop_stepper(cs_machine *cs);
int cs_op_subi_stepper(cs_machine *cs);
int cs_op_subi_microstepper(cs_machine *cs);
int cs_op_cpi_stepper(cs_machine *cs);
int cs_op_ldi_stepper(cs_machine *cs);
int cs_op_ldi_microstepper(cs_machine *cs);

/* Shared no-op */
int cs_op_noop_stepper(cs_machine *cs);

#endif /* CS_OPCODES_H */

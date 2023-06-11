/** @file cs.h */

#ifndef CS_H
#define CS_H

#include <stddef.h>

#include "../utils.h"

#include "cs_memory.h"
#include "cs_platforms.h"
#include "cs_registers.h"
#include "cs_signals.h"

typedef struct cs_instruction_op cs_instruction_op;
typedef struct cs_machine        cs_machine;

/** @brief CS instruction opcode data */
struct cs_instruction_op {
    int (*stepper)(cs_machine *cs);
    int (*microstepper)(cs_machine *cs);
    cs_signals signals[5];
};

#endif /* CS_H */

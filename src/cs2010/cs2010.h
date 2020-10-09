/** @file cs2010.h */

#ifndef CS2010_H
#define CS2010_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "cs_memory.h"
#include "cs_registers.h"
#include "cs_instructions.h"

#define CS_CLEAR_RAM (1 << 0)
#define CS_CLEAR_ROM (1 << 1)

struct cs2010 {
    cs_memory mem;
    cs_registers reg;
    bool stopped;
    uint8_t microop;
};
typedef struct cs2010 cs2010;

bool cs_init(cs2010 *cs);

void cs_reset(cs2010 *cs);

void cs_clear_memory(cs2010 *cs, uint8_t flags);

void cs_reset_registers(cs2010 *cs);

void cs_microstep(cs2010 *cs, size_t microsteps);

void cs_step(cs2010 *cs, size_t steps);

void cs_blockstep(cs2010 *cs);

void cs_free(cs2010 *cs);

#endif /* CS2010_H */
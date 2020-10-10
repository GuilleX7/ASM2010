/** @file cs2010.h */

#ifndef CS2010_H
#define CS2010_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "cs_memory.h"
#include "cs_registers.h"

#define CS_CLEAR_RAM (1 << 0)
#define CS_CLEAR_ROM (1 << 1)

#define CS_SUCCESS 0
#define CS_FAILED 1
#define CS_NOT_ENOUGH_ROM 2
#define CS_INVALID_INSTRUCTIONS 3

struct cs2010 {
    cs_memory mem;
    cs_registers reg;
    uint8_t microop;
    bool stopped;
};
typedef struct cs2010 cs2010;

bool cs_init(cs2010 *cs);

void cs_hard_reset(cs2010 *cs);

void cs_soft_reset(cs2010 *cs);

void cs_clear_memory(cs2010 *cs, uint8_t flags);

void cs_reset_registers(cs2010 *cs, bool clear_signals);

int cs_load_and_check(cs2010 *cs, uint16_t *sentences, size_t sentences_length);

void cs_microfetch(cs2010 *cs);

void cs_fetch(cs2010 *cs);

void cs_microstep(cs2010 *cs);

void cs_step(cs2010 *cs);

void cs_fullstep(cs2010 *cs);

void cs_blockstep(cs2010 *cs);

void cs_free(cs2010 *cs);

#endif /* CS2010_H */
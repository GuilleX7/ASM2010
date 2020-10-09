/** @file cs2010.c */

#include "cs2010.h"

bool cs_init(cs2010 *cs) {
    cs->memory.rom = malloc(sizeof * cs->memory.rom * CS_ROM_SIZE);
    if (!cs->memory.rom) {
        return false;
    }

    cs->memory.ram = malloc(sizeof * cs->memory.ram * CS_RAM_SIZE);
    if (!cs->memory.ram) {
        free(cs->memory.rom);
        return false;
    }

    cs_reset(cs);
}

void cs_reset(cs2010 *cs) {
    cs_clear_memory(cs, CS2010_CLEAR_RAM, CS2010_CLEAR_ROM)
    cs_reset_registers(cs, true);
    cs->microop = 0;
    cs->stopped = false;
}

void cs_clear_memory(cs2010 *cs, uint8_t flags) {
    if (flags & CS_CLEAR_ROM) {
        memset(cs->mem.rom, 0, CS_ROM_SIZE * sizeof *cs->mem.rom);
    }
    if (flags & CS_CLEAR_RAM) {
        memset(cs->mem.ram, 0, CS_RAM_SIZE * sizeof *cs->mem.ram);
    }
}

void cs_reset_registers(cs2010 *cs, bool clear_signals) {
    cs->reg.ac = 0;
    cs->reg.ir = 0;
    cs->reg.mar = 0;
    cs->reg.mdr = 0;
    cs->reg.pc = 0;
    cs->reg.r0 = 0;
    cs->reg.r1 = 0;
    cs->reg.r2 = 0;
    cs->reg.r3 = 0;
    cs->reg.r4 = 0;
    cs->reg.r5 = 0;
    cs->reg.r6 = 0;
    cs->reg.r7 = 0;
    cs->reg.sr = 0;

    if (clear_signals) {
        cs->reg.signals = CS_SIGNALS_NONE;
    }
}
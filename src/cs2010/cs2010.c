/** @file cs2010.c */

#include "cs2010.h"

bool cs_init(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs->memory.rom = malloc(sizeof * cs->memory.rom * CS_ROM_SIZE);
    if (!cs->memory.rom) {
        return false;
    }

    cs->memory.ram = malloc(sizeof * cs->memory.ram * CS_RAM_SIZE);
    if (!cs->memory.ram) {
        free(cs->memory.rom);
        return false;
    }

    cs_hard_reset(cs);
}

void cs_hard_reset(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs_clear_memory(cs, CS2010_CLEAR_RAM, CS2010_CLEAR_ROM)
    cs_reset_registers(cs, true);
    cs->stopped = false;
    cs_fetch(cs);
}

void cs_soft_reset(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs->reg.pc = 0;
    cs->reg.sp = UINT8_MAX;
    cs->stopped = false;
    cs_fetch(cs);
}

void cs_clear_memory(cs2010 *cs, uint8_t flags) {
    if (!cs) {
        return;
    }

    if (flags & CS_CLEAR_ROM) {
        memset(cs->mem.rom, 0, CS_ROM_SIZE * sizeof *cs->mem.rom);
    }
    if (flags & CS_CLEAR_RAM) {
        memset(cs->mem.ram, 0, CS_RAM_SIZE * sizeof *cs->mem.ram);
    }
}

void cs_reset_registers(cs2010 *cs, bool clear_signals) {
    if (!cs) {
        return;
    }

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
    cs->reg.sp = UINT8_MAX;
    if (clear_signals) {
        cs->reg.signals = CS_SIGNALS_NONE;
    }
}

int cs_load_and_check(cs2010 *cs, uint16_t *sentences, size_t sentences_length) {
    if (!cs || !sentences) {
        return CS_FAILED;
    }
    
    if (sentences_length > CS_ROM_SIZE) {
        return CS_NOT_ENOUGH_ROM;
    }

    for (size_t i = 0; i < sentences_length; i++) {
        if (!cs_ins_list[CS_GET_OPCODE(sentence[i])].name) {
            return CS_INVALID_INSTRUCTIONS;
        }
        cs->mem.rom[i] = sentences[i];
    }

    return CS_SUCCESS;
}

void cs_fetch(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs->reg.ir = cs->mem.rom[cs->reg.pc];
    cs->reg.pc++;
    cs->microop = 0;
}

void cs_microstep(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs_ins_list[CS_GET_OPCODE(cs->reg.ir)].microstepper(cs);
}

void cs_step(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs_ins_list[CS_GET_OPCODE(cs->reg.ir)].stepper(cs);
}

void cs_fullstep(cs2010 *cs) {
    if (!cs) {
        return;
    }

    if (!cs->microop) {
        cs_step(cs);
    } else {
        do {
            cs_microstep();
        } while (cs->microop);
    }
}

void cs_blockstep(cs2010 *cs) {
    if (!cs) {
        return;
    }
}

void cs_free(cs2010 *cs) {
    if (!cs) {
        return;
    }

    if (cs->mem.rom) {
        free(cs->mem.rom);
    }

    if (cs->mem.ram) {
        free(cs->mem.ram);
    }
}
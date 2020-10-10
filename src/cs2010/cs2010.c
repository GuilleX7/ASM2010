/** @file cs2010.c */

#include <stdlib.h>
#include <string.h>

#include "cs2010.h"
#include "cs_instructions.h"

bool cs_init(cs2010 *cs) {
    if (!cs) {
        return false;
    }

    cs->mem.rom = malloc(sizeof * cs->mem.rom * CS_ROM_SIZE);
    if (!cs->mem.rom) {
        return false;
    }

    cs->mem.ram = malloc(sizeof * cs->mem.ram * CS_RAM_SIZE);
    if (!cs->mem.ram) {
        free(cs->mem.rom);
        return false;
    }

    cs_hard_reset(cs);
    return true;
}

void cs_hard_reset(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs_clear_memory(cs, CS_CLEAR_RAM | CS_CLEAR_ROM);
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
        if (!cs_ins_list[CS_GET_OPCODE(sentences[i])].name) {
            return CS_INVALID_INSTRUCTIONS;
        }
        cs->mem.rom[i] = sentences[i];
    }

    return CS_SUCCESS;
}

void cs_microfetch(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs->microop++;
    cs->reg.signals = cs_ins_list[CS_GET_OPCODE(cs->reg.ir)].signals[cs->microop];
}

void cs_fetch(cs2010 *cs) {
    if (!cs) {
        return;
    }

    cs->microop = 0;
    cs->reg.ir = cs->mem.rom[cs->reg.pc];
    cs->reg.signals = cs_ins_list[CS_GET_OPCODE(cs->reg.ir)].signals[cs->microop];
    cs->reg.pc++;
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
            cs_microstep(cs);
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
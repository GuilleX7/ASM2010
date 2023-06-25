/** @file cs.c */

#include <stdlib.h>
#include <string.h>

#include "../../include/asm2010.h"

#include "cs_instructions.h"
#include "cs_opcodes.h"
#include "cs_platforms.h"

#include "cs2010/cs2010_platform.h"
#include "cs3/cs3_platform.h"

#include "cs.h"

cs_machine *cs_create() {
    return malloc(sizeof(cs_machine));
}

static int cs_init_platform(cs_machine *cs, cs_platform platform) {
    switch (platform) {
        case CS_PLATFORM_2010:
            cs->opcodes = cs2010_platform_opcodes;
            return CS_INIT_OK;
        case CS_PLATFORM_3:
            cs->opcodes = cs3_platform_opcodes;
            return CS_INIT_OK;
        default:
            return CS_INIT_INVALID_PLATFORM;
    }
}

int cs_init(cs_machine *cs, cs_platform platform) {
    if (!cs) {
        return CS_INIT_NOT_ENOUGH_MEMORY;
    }

    cs->memory.rom = malloc(sizeof *cs->memory.rom * CS_ROM_SIZE);
    if (!cs->memory.rom) {
        return CS_INIT_NOT_ENOUGH_MEMORY;
    }

    cs->memory.ram = malloc(sizeof *cs->memory.ram * CS_RAM_SIZE);
    if (!cs->memory.ram) {
        free(cs->memory.rom);
        return CS_INIT_NOT_ENOUGH_MEMORY;
    }

    cs->registers.regfile[0] = &cs->registers.r0;
    cs->registers.regfile[1] = &cs->registers.r1;
    cs->registers.regfile[2] = &cs->registers.r2;
    cs->registers.regfile[3] = &cs->registers.r3;
    cs->registers.regfile[4] = &cs->registers.r4;
    cs->registers.regfile[5] = &cs->registers.r5;
    cs->registers.regfile[6] = &cs->registers.r6;
    cs->registers.regfile[7] = &cs->registers.r7;

    cs->io_read_fn  = cs_io_read_stub;
    cs->io_write_fn = cs_io_write_stub;

    return cs_init_platform(cs, platform);
}

void cs_set_io_functions(cs_machine *cs, cs_io_read_fn io_read_fn, cs_io_write_fn io_write_fn) {
    cs->io_read_fn  = io_read_fn;
    cs->io_write_fn = io_write_fn;
}

static void cs_fetch(cs_machine *cs) {
    cs->registers.ir = cs->memory.rom[cs->registers.pc++];
    cs->microop      = 0;
    cs->signals      = cs->opcodes[CS_GET_OPCODE(cs->registers.ir)].signals[cs->microop];
}

static void cs_microfetch(cs_machine *cs) {
    cs->signals = cs->opcodes[CS_GET_OPCODE(cs->registers.ir)].signals[++cs->microop];
}

int cs_load_machine_code(struct cs_machine *cs, struct cs_as_machine_code *machine_code) {
    return cs_load_machine_instructions(cs, machine_code->machine_instructions,
                                        machine_code->machine_instructions_amount);
}

int cs_load_machine_instructions(cs_machine *cs, unsigned short *machine_instructions,
                                 size_t machine_instructions_amount) {
    size_t        i;
    unsigned char opcode;

    if (!machine_instructions) {
        return CS_LOAD_FAILED;
    }

    if (machine_instructions_amount > CS_ROM_SIZE) {
        return CS_LOAD_NOT_ENOUGH_ROM;
    }

    for (i = 0; i < machine_instructions_amount; i++) {
        opcode = CS_GET_OPCODE(machine_instructions[i]);
        if (!cs->opcodes[opcode].stepper || !cs->opcodes[opcode].microstepper) {
            return CS_LOAD_ROM_INVALID_INSTRUCTIONS;
        }
    }

    cs_clear_memory(cs, true, true);
    cs_reset_registers(cs);
    memcpy(cs->memory.rom, machine_instructions, machine_instructions_amount * sizeof(*machine_instructions));
    cs_fetch(cs);
    return CS_LOAD_OK;
}

void cs_microstep(cs_machine *cs) {
    switch (cs->opcodes[CS_GET_OPCODE(cs->registers.ir)].microstepper(cs)) {
        case CS_OP_DO_FETCH:
            cs_fetch(cs);
            break;
        case CS_OP_DO_MICROFETCH:
            cs_microfetch(cs);
            break;
        case CS_OP_DO_NOTHING:
        default:
            break;
    }
}

static void cs_step(cs_machine *cs) {
    switch (cs->opcodes[CS_GET_OPCODE(cs->registers.ir)].stepper(cs)) {
        case CS_OP_DO_FETCH:
            cs_fetch(cs);
            break;
        case CS_OP_DO_MICROFETCH:
            cs_microfetch(cs);
            break;
        case CS_OP_DO_NOTHING:
        default:
            break;
    }
}

void cs_fullstep(cs_machine *cs) {
    if (!cs->microop) {
        cs_step(cs);
    } else {
        do {
            cs_microstep(cs);
        } while (cs->microop);
    }
}

bool cs_blockstep(cs_machine *cs, size_t max_instructions) {
    unsigned char opcode;
    size_t        remaining_instructions = max_instructions;

    if (!remaining_instructions) {
        return false;
    }

    do {
        cs_fullstep(cs);
        opcode = CS_GET_OPCODE(cs->registers.ir);
        remaining_instructions--;
    } while (remaining_instructions > 0 && opcode != CS_INS_I_JMP && opcode != CS_INS_I_BRXX &&
             opcode != CS_INS_I_CALL && !cs->stopped);

    return remaining_instructions != 0;
}

void cs_hard_reset(cs_machine *cs, bool clear_rom) {
    cs_clear_memory(cs, clear_rom, true);
    cs_reset_registers(cs);
    cs_fetch(cs);
}

void cs_soft_reset(cs_machine *cs) {
    cs->registers.pc = 0;
    cs->registers.sp = 0xFF;
    cs->stopped      = false;
    cs_fetch(cs);
}

void cs_clear_memory(cs_machine *cs, unsigned char clear_rom, unsigned char clear_ram) {
    if (clear_rom) {
        memset(cs->memory.rom, 0, CS_ROM_SIZE * sizeof *cs->memory.rom);
    }
    if (clear_ram) {
        memset(cs->memory.ram, 0, CS_RAM_SIZE * sizeof *cs->memory.ram);
    }
}

void cs_reset_registers(cs_machine *cs) {
    cs->registers.ir  = 0;
    cs->registers.r0  = 0;
    cs->registers.r1  = 0;
    cs->registers.r2  = 0;
    cs->registers.r3  = 0;
    cs->registers.r4  = 0;
    cs->registers.r5  = 0;
    cs->registers.r6  = 0;
    cs->registers.r7  = 0;
    cs->registers.sp  = 0xFF;
    cs->registers.pc  = 0;
    cs->registers.ac  = 0;
    cs->registers.sr  = 0;
    cs->registers.mdr = 0;
    cs->registers.mar = 0;
    cs->stopped       = false;
}

void cs_free(cs_machine *cs) {
    if (!cs) {
        return;
    }

    if (cs->memory.rom) {
        free(cs->memory.rom);
    }
    if (cs->memory.ram) {
        free(cs->memory.ram);
    }

    free(cs);
}

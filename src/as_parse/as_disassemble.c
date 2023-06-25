#include "as_disassemble.h"

#include <stdlib.h>
#include <string.h>

#include "../../include/asm2010.h"

#include "../m2010/cs3/cs3_registers.h"
#include "../m2010/cs_instructions.h"
#include "../m2010/cs_platforms.h"
#include "../m2010/cs_registers.h"

static char *_as_disassemble_instruction(unsigned short machine_instruction, cs_platform platform,
                                         char *output_buffer) {
    cs_instruction const *instruction = cs_ins_get_from_sentence(machine_instruction);
    unsigned char         arg_a       = CS_GET_ARG_A(machine_instruction);
    unsigned char         arg_b       = CS_GET_ARG_B(machine_instruction);
    bool                  valid       = true;

    if (!CS_PLATFORM_IS_VALID(platform) || !instruction->name ||
        !CS_PLATFORM_IS_AVAILABLE(instruction->platforms, platform)) {
        return 0;
    }

    switch (instruction->format) {
        case CS_INS_FORMAT_A:
            if (platform == CS_PLATFORM_2010) {
                if (arg_b < CS_REGISTERS_RMIN || arg_b > CS_REGISTERS_RMAX) {
                    // We need a register here
                    valid = false;
                    break;
                }
            } else if (platform == CS_PLATFORM_3) {
                if ((instruction->index == CS_INS_I_ST || instruction->index == CS_INS_I_LD) &&
                    (arg_b < CS3_REGISTERS_INDIRECT_RMIN || arg_b > CS3_REGISTERS_INDIRECT_RMAX)) {
                    // More complex scenario. CS3's ST & LD only allow Y or Z
                    // (R6 or R7) at arg_b
                    valid = false;
                    break;
                } else if (arg_b < CS_REGISTERS_RMIN || arg_b > CS_REGISTERS_RMAX) {
                    // Like CS2010, other instructions require a register here
                    valid = false;
                    break;
                }
            }

            if (instruction->index == CS_INS_I_LD && platform == CS_PLATFORM_2010) {
                snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s R%u, (R%u)", instruction->name, arg_a,
                         arg_b);
            } else if (instruction->index == CS_INS_I_LD && platform == CS_PLATFORM_3) {
                snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s R%u, %c", instruction->name, arg_a,
                         CS3_REGISTER_GET_INDIRECT_NAME(arg_b));
            } else if (instruction->index == CS_INS_I_ST && platform == CS_PLATFORM_2010) {
                snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s (R%u), R%u", instruction->name, arg_b,
                         arg_a);
            } else if (instruction->index == CS_INS_I_ST && platform == CS_PLATFORM_3) {
                snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s %c, R%u", instruction->name,
                         CS3_REGISTER_GET_INDIRECT_NAME(arg_b), arg_a);
            } else {
                snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s R%u, R%u", instruction->name, arg_a, arg_b);
            }
            break;
        case CS_INS_FORMAT_B:
            if (instruction->index == CS_INS_I_STS) {
                snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s " HEX8_X_FORMAT ", R%u", instruction->name,
                         arg_b, arg_a);
            } else {
                snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s R%u, " HEX8_X_FORMAT, instruction->name,
                         arg_a, arg_b);
            }
            break;
        case CS_INS_FORMAT_C:
            snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s " HEX8_X_FORMAT, instruction->name, arg_b);
            break;
        case CS_INS_FORMAT_D:
            snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s R%u", instruction->name, arg_a);
            break;
        case CS_INS_FORMAT_E:
            snprintf(output_buffer, AS_MAX_DISASSEMBLY_LENGTH - 1, "%s", instruction->name);
            break;
        default:
            valid = false;
            break;
    }

    if (!valid) {
        return 0;
    }

    return output_buffer;
}

char *cs_as_disassemble_instruction(unsigned short machine_instruction, cs_platform platform) {
    char *disassembly_line = malloc(sizeof(char) * AS_MAX_DISASSEMBLY_LENGTH);
    if (!disassembly_line) {
        return 0;
    }

    if (!_as_disassemble_instruction(machine_instruction, platform, disassembly_line)) {
        free(disassembly_line);
        disassembly_line = 0;
    }

    return disassembly_line;
}

char *cs_as_disassemble_instructions(unsigned short *machine_instructions, size_t machine_instructions_amount,
                                     cs_platform platform) {
    size_t i;
    char  *disassembly_line;
    char  *disassembly_code;

    disassembly_line = malloc(sizeof(char) * AS_MAX_DISASSEMBLY_LENGTH);
    if (!disassembly_line) {
        return 0;
    }

    disassembly_code = malloc(sizeof(char) * AS_MAX_DISASSEMBLY_LENGTH * machine_instructions_amount);
    if (!disassembly_code) {
        free(disassembly_line);
        return 0;
    }
    disassembly_code[0] = '\0';

    for (i = 0; i < machine_instructions_amount; i++) {
        if (_as_disassemble_instruction(machine_instructions[i], platform, disassembly_line)) {
            strncat(disassembly_code, disassembly_line, AS_MAX_DISASSEMBLY_LENGTH - 1);
        }
        strncat(disassembly_code, "\n", 1);
    }

    return disassembly_code;
}

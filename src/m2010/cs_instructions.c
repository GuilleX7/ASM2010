/** @file cs_instructions.c */

#include "../../include/asm2010.h"

#include "../hash_table.h"

#include "cs_opcodes.h"
#include "cs_platforms.h"
#include "cs_registers.h"

#include "cs_instructions.h"

cs_instruction const cs_ins_list[] = {[CS_INS_I_ST] =
                                          {
                                              CS_INS_I_ST,
                                              "ST",
                                              CS_INS_FORMAT_A,
                                              0x0,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_LD] =
                                          {
                                              CS_INS_I_LD,
                                              "LD",
                                              CS_INS_FORMAT_A,
                                              0x1,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_STS] =
                                          {
                                              CS_INS_I_STS,
                                              "STS",
                                              CS_INS_FORMAT_B,
                                              0x2,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_LDS] =
                                          {
                                              CS_INS_I_LDS,
                                              "LDS",
                                              CS_INS_FORMAT_B,
                                              0x3,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_CALL] =
                                          {
                                              CS_INS_I_CALL,
                                              "CALL",
                                              CS_INS_FORMAT_C,
                                              0x4,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_RET] =
                                          {
                                              CS_INS_I_RET,
                                              "RET",
                                              CS_INS_FORMAT_E,
                                              0x5,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_BRXX] =
                                          {
                                              CS_INS_I_BRXX,
                                              0,
                                              CS_INS_FORMAT_C,
                                              0x6,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_JMP] =
                                          {
                                              CS_INS_I_JMP,
                                              "JMP",
                                              CS_INS_FORMAT_C,
                                              0x7,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_ADD] =
                                          {
                                              CS_INS_I_ADD,
                                              "ADD",
                                              CS_INS_FORMAT_A,
                                              0x8,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_SUB] =
                                          {
                                              CS_INS_I_SUB,
                                              "SUB",
                                              CS_INS_FORMAT_A,
                                              0xa,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_CP] =
                                          {
                                              CS_INS_I_CP,
                                              "CP",
                                              CS_INS_FORMAT_A,
                                              0xb,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_MOV] =
                                          {
                                              CS_INS_I_MOV,
                                              "MOV",
                                              CS_INS_FORMAT_A,
                                              0xf,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_CLC] =
                                          {
                                              CS_INS_I_CLC,
                                              "CLC",
                                              CS_INS_FORMAT_E,
                                              0x12,
                                              CS_PLATFORM_2010,
                                          },
                                      [CS_INS_I_SEC] =
                                          {
                                              CS_INS_I_SEC,
                                              "SEC",
                                              CS_INS_FORMAT_E,
                                              0x13,
                                              CS_PLATFORM_2010,
                                          },
                                      [CS_INS_I_ROR] =
                                          {
                                              CS_INS_I_ROR,
                                              "ROR",
                                              CS_INS_FORMAT_D,
                                              0x14,
                                              CS_PLATFORM_2010,
                                          },
                                      [CS_INS_I_ROL] =
                                          {
                                              CS_INS_I_ROL,
                                              "ROL",
                                              CS_INS_FORMAT_D,
                                              0x15,
                                              CS_PLATFORM_2010,
                                          },
                                      [CS_INS_I_STOP] =
                                          {
                                              CS_INS_I_STOP,
                                              "STOP",
                                              CS_INS_FORMAT_E,
                                              0x17,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_ADDI] =
                                          {
                                              CS_INS_I_ADDI,
                                              "ADDI",
                                              CS_INS_FORMAT_B,
                                              0x18,
                                              CS_PLATFORM_2010,
                                          },
                                      [CS_INS_I_SUBI] =
                                          {
                                              CS_INS_I_SUBI,
                                              "SUBI",
                                              CS_INS_FORMAT_B,
                                              0x1a,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_CPI] =
                                          {
                                              CS_INS_I_CPI,
                                              "CPI",
                                              CS_INS_FORMAT_B,
                                              0x1b,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_LDI] =
                                          {
                                              CS_INS_I_LDI,
                                              "LDI",
                                              CS_INS_FORMAT_B,
                                              0x1f,
                                              CS_PLATFORM_ALL,
                                          },
                                      /* BR-like instructions */
                                      [CS_INS_I_BRZS] =
                                          {
                                              CS_INS_I_BRZS,
                                              "BRZS",
                                              CS_INS_FORMAT_C,
                                              0x6,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_BREQ] =
                                          {
                                              CS_INS_I_BREQ,
                                              "BREQ",
                                              CS_INS_FORMAT_C,
                                              0x6,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_BRCS] =
                                          {
                                              CS_INS_I_BRCS,
                                              "BRCS",
                                              CS_INS_FORMAT_C,
                                              0x6,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_BRLO] =
                                          {
                                              CS_INS_I_BRLO,
                                              "BRLO",
                                              CS_INS_FORMAT_C,
                                              0x6,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_BRVS] =
                                          {
                                              CS_INS_I_BRVS,
                                              "BRVS",
                                              CS_INS_FORMAT_C,
                                              0x6,
                                              CS_PLATFORM_ALL,
                                          },
                                      [CS_INS_I_BRLT] = {
                                          CS_INS_I_BRLT,
                                          "BRLT",
                                          CS_INS_FORMAT_C,
                                          0x6,
                                          CS_PLATFORM_ALL,
                                      }};

static hash_table ins_lookup_table           = {0};
static bool       ins_lookup_table_initiated = false;

bool cs_ins_search_start(void) {
    size_t i = 0;
    if (!ins_lookup_table_initiated) {
        if (!hash_table_init(&ins_lookup_table)) {
            return false;
        }
        for (; i < CS_INS_LENGTH; i++) {
            if (cs_ins_list[i].name) {
                if (!hash_table_put(&ins_lookup_table, cs_ins_list[i].name, &i)) {
                    hash_table_free(&ins_lookup_table);
                    return false;
                }
            }
        }
        ins_lookup_table_initiated = true;
    }
    return true;
}

void cs_ins_search_stop(void) {
    if (ins_lookup_table_initiated) {
        hash_table_free(&ins_lookup_table);
        ins_lookup_table_initiated = false;
    }
}

cs_instruction const *cs_ins_search_by_name(char *name) {
    void                 *search = hash_table_get(&ins_lookup_table, name);
    cs_instruction const *result = {0};
    if (search) {
        result = &cs_ins_list[*(int *)search];
    }
    return result;
}

cs_instruction const *cs_ins_get_from_sentence(unsigned short sentence) {
    unsigned char         opcode         = CS_GET_OPCODE(sentence);
    cs_instruction const *cs_instruction = &cs_ins_list[opcode];

    /* Check for BR-like instruction */
    unsigned char jmp_condition = 0;
    if (opcode == cs_ins_list[CS_INS_I_BRXX].opcode) {
        jmp_condition = CS_GET_JMP_CONDITION(sentence);
        switch (jmp_condition) {
            case CS_JMP_COND_EQUAL:
                cs_instruction = &cs_ins_list[CS_INS_I_BREQ];
                break;
            case CS_JMP_COND_LOWER:
                cs_instruction = &cs_ins_list[CS_INS_I_BRCS];
                break;
            case CS_JMP_COND_OVERFLOW:
                cs_instruction = &cs_ins_list[CS_INS_I_BRVS];
                break;
            case CS_JMP_COND_SLOWER:
                cs_instruction = &cs_ins_list[CS_INS_I_BRLT];
                break;
        }
    }

    /* Check for a valid instruction */
    if (!cs_instruction->name) {
        cs_instruction = 0;
    }
    return cs_instruction;
}

unsigned char cs_ins_get_jmp_condition(cs_instruction const *const cs_instruction) {
    switch (cs_instruction->index) {
        case CS_INS_I_BRCS:
        case CS_INS_I_BRLO:
            return CS_JMP_COND_LOWER;
        case CS_INS_I_BRVS:
            return CS_JMP_COND_OVERFLOW;
        case CS_INS_I_BRLT:
            return CS_JMP_COND_SLOWER;
        case CS_INS_I_BRZS:
        case CS_INS_I_BREQ:
        default:
            return CS_JMP_COND_EQUAL;
    }
}

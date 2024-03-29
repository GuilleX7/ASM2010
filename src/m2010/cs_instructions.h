/** @file cs_instructions.h */

#ifndef CS_INSTRUCTIONS_H
#define CS_INSTRUCTIONS_H

#include "../utils.h"

#define CS_INS_FORMAT_A 0
#define CS_INS_FORMAT_B 1
#define CS_INS_FORMAT_C 2
#define CS_INS_FORMAT_D 3
#define CS_INS_FORMAT_E 4

#define CS_INS_NAME_MAX_LENGTH 4
#define CS_INS_INM_MAX_VALUE   0xFFu

#define CS_INS_OPCODE_OFFSET 11
#define CS_INS_ARG_A_OFFSET  8
#define CS_INS_ARG_B_OFFSET  0

#define CS_JMP_COND_EQUAL    0u
#define CS_JMP_COND_LOWER    1u
#define CS_JMP_COND_OVERFLOW 2u
#define CS_JMP_COND_SLOWER   3u

#define CS_GET_OPCODE(sentence)                    ((sentence & 0xF800u) >> CS_INS_OPCODE_OFFSET)
#define CS_GET_ARG_A(sentence)                     ((sentence & 0x700u) >> CS_INS_ARG_A_OFFSET)
#define CS_GET_ARG_B(sentence)                     ((sentence & 0xFFu) >> CS_INS_ARG_B_OFFSET)
#define CS_GET_REG_A(sentence)                     CS_GET_ARG_A(sentence)
#define CS_GET_REG_B(sentence)                     (CS_GET_ARG_B(sentence) & 0x7u)
#define CS_GET_JMP_CONDITION(sentence)             CS_GET_ARG_A(sentence)
#define CS_GET_MACHINE_INSTRUCTION(lsbyte, msbyte) (((unsigned short)msbyte << 8) + (unsigned short)lsbyte)

/** @brief List of instructions */
enum cs_instruction_idx {
    CS_INS_I_ST   = 0x0,
    CS_INS_I_LD   = 0x1,
    CS_INS_I_STS  = 0x2,
    CS_INS_I_LDS  = 0x3,
    CS_INS_I_CALL = 0x4,
    CS_INS_I_RET  = 0x5,
    CS_INS_I_BRXX = 0x6,
    CS_INS_I_JMP  = 0x7,
    CS_INS_I_ADD  = 0x8,
    CS_INS_I_SUB  = 0xa,
    CS_INS_I_CP   = 0xb,
    CS_INS_I_MOV  = 0xf,
    CS_INS_I_CLC  = 0x12,
    CS_INS_I_SEC  = 0x13,
    CS_INS_I_ROR  = 0x14,
    CS_INS_I_ROL  = 0x15,
    CS_INS_I_STOP = 0x17,
    CS_INS_I_ADDI = 0x18,
    CS_INS_I_SUBI = 0x1a,
    CS_INS_I_CPI  = 0x1b,
    CS_INS_I_LDI  = 0x1f,
    /* BR-like instructions */
    CS_INS_I_BRZS,
    CS_INS_I_BREQ,
    CS_INS_I_BRCS,
    CS_INS_I_BRLO,
    CS_INS_I_BRVS,
    CS_INS_I_BRLT,
    /* Array length */
    CS_INS_LENGTH
};
typedef enum cs_instruction_idx cs_instruction_idx;

/** @brief CS instruction data */
struct cs_instruction {
    cs_instruction_idx index;
    char              *name;
    int                format;
    unsigned char      opcode;
    unsigned char      platforms;
};
typedef struct cs_instruction cs_instruction;

/** @brief Name and opcode for each instruction */
extern cs_instruction const cs_ins_list[CS_INS_LENGTH];

/**
        @brief Initializes the hash table. Searching names will be allowed from
        this point onwards.
*/
bool cs_ins_search_start(void);

/**
 *  @brief Destroys the hash table. Searching names will be forbbiden from
 *  this point onwards.
 */
void cs_ins_search_stop(void);

/**
 *  @brief Searchs for a instruction name an returns the corresponding index
 *  @param name Name of the instruction
 *  @return Pointer to instruction structure if found, null pointer otherwise
 */
cs_instruction const *cs_ins_search_by_name(char *name);

/**
 * @brief Returns the corresponding instruction in a given sentence
 * @param sentence The given sentence where instruction will be looked for
 * @return Pointer to instruction structure if found, null pointer otherwise
 */
cs_instruction const *cs_ins_get_from_sentence(unsigned short sentence);

/**
 * @brief Returns the corresponding jump condition for a given instruction
 * @param instruction Pointer to given instruction structure
 * @return Jump condition code if any associated, 0 otherwise
 */
unsigned char cs_ins_get_jmp_condition(cs_instruction const *const instruction);

#endif /* CS_INSTRUCTIONS_H */

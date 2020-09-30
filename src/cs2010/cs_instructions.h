/** @file cs_instructions.h */

#ifndef CS_INSTRUCTIONS_H
#define CS_INSTRUCTIONS_H

#include <stdint.h>
#include <stdbool.h>

#include "../hash_table.h"

#define CS_INS_NAME_MAX_LENGTH 4

#define CS_JMP_COND_EQUAL 0
#define CS_JMP_COND_LOWER 1
#define CS_JMP_COND_OVERFLOW 2
#define CS_JMP_COND_SLOWER 3

#define CS_GET_OPCODE(sentence) ((sentence & 0xF800u) >> 11)
#define CS_GET_JMP_CONDITION(sentence) ((sentence & 0x700u) >> 8)

/** @brief List of instructions' indexes */
enum cs_instruction_idx {
	/* Instructions */
	CS_INS_I_ST = 0x0,
	CS_INS_I_LD = 0x1,
	CS_INS_I_STS = 0x2,
	CS_INS_I_LDS = 0x3,
	CS_INS_I_CALL = 0x4,
	CS_INS_I_RET = 0x5,
	CS_INS_I_JMP = 0x7,
	CS_INS_I_ADD = 0x8,
	CS_INS_I_SUB = 0xa,
	CS_INS_I_CP = 0xb,
	CS_INS_I_MOV = 0xf,
	CS_INS_I_CLC = 0x12,
	CS_INS_I_SEC = 0x13,
	CS_INS_I_ROR = 0x14,
	CS_INS_I_ROL = 0x15,
	CS_INS_I_STOP = 0x17,
	CS_INS_I_ADDI = 0x18,
	CS_INS_I_SUBI = 0x1a,
	CS_INS_I_CPI = 0x1b,
	CS_INS_I_LDI = 0x1f,
	/* BR-like instructions */
	CS_INS_I_BRZS,
	CS_INS_I_BREQ,
	CS_INS_I_BRCS,
	CS_INS_I_BRLO,
	CS_INS_I_BRVS,
	CS_INS_I_BRLT,
	/* Instruction length */
	CS_INS_LEN
};
typedef enum cs_instruction_idx cs_instruction_idx;

/** @brief List of instructions' formats */
enum cs_instruction_format {
	CS_INS_FORMAT_A, CS_INS_FORMAT_B, CS_INS_FORMAT_C, CS_INS_FORMAT_D, CS_INS_FORMAT_E
};
typedef enum cs_instruction_format cs_instruction_format;

/** @brief CS2010 instruction data */
struct cs_instruction {
	cs_instruction_idx index;
	char *name;
	uint8_t opcode;
	cs_instruction_format format;
	uint8_t jmp_condition;
};
typedef struct cs_instruction cs_instruction;

/** @brief Name and opcodes for each instruction */
cs_instruction const CS_INS_LIST[CS_INS_LEN];

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
cs_instruction const *cs_ins_get_from_sentence(uint16_t sentence);

/**
 * @brief Returns the corresponding jump condition for a given instruction
 * @param instruction Pointer to given instruction structure
 * @return Jump condition code if any associated, 0 otherwise
*/
uint8_t cs_ins_get_jmp_condition(cs_instruction const *const instruction);

#endif /* CS_INSTRUCTIONS_H */

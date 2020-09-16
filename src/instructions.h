#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h> /* uint16_t */
#include <stdbool.h> /* bool */
#include "hash_table.h" /* hash_table */

#define INSTRUCTION_MAX_LENGTH 4

/** @brief List of instructions' indexes */
enum instruction_index {
	/* Instructions */
	INS_I_ST = 0x0,
	INS_I_LD = 0x1,
	INS_I_STS = 0x2,
	INS_I_LDS = 0x3,
	INS_I_CALL = 0x4,
	INS_I_RET = 0x5,
	INS_I_JMP = 0x7,
	INS_I_ADD = 0x8,
	INS_I_SUB = 0xa,
	INS_I_CP = 0xb,
	INS_I_MOV = 0xf,
	INS_I_CLC = 0x12,
	INS_I_SEC = 0x13,
	INS_I_ROR = 0x14,
	INS_I_ROL = 0x15,
	INS_I_STOP = 0x17,
	INS_I_ADDI = 0x18,
	INS_I_SUBI = 0x1a,
	INS_I_CPI = 0x1b,
	INS_I_LDI = 0x1f,
	/* BR-like instructions */
	INS_I_BRZS,
	INS_I_BREQ,
	INS_I_BRCS,
	INS_I_BRLO,
	INS_I_BRVS,
	INS_I_BRLT,
	/* Instruction length */
	INS_LEN
};
typedef enum instruction_index instruction_index;

/** @brief List of instructions' formats */
enum instruction_format {
	INS_FORMAT_A, INS_FORMAT_B, INS_FORMAT_C, INS_FORMAT_D, INS_FORMAT_E
};
typedef enum instruction_format instruction_format;

/** @brief CS2010 instruction data */
struct instruction {
	instruction_index index;
	char *name;
	uint8_t opcode;
	instruction_format format;
};
typedef struct instruction instruction;

/** @brief Name and opcodes for each instruction */
extern instruction const INS_LIST[];

/**
	@brief Initializes the hash table. Searching names will be allowed from
	this point onwards.
*/
void ins_search_start(void);

/**
 *  @brief Destroys the hash table. Searching names will be forbbiden from
 *  this point onwards.
*/
void ins_search_stop(void);

/**
 *  @brief Searchs for a instruction name an returns the corresponding index
 *  @param name Name of the instruction
 *  @return Pointer to instruction structure if found, null pointer otherwise
*/
instruction const *ins_search_by_name(char *name);

/**
 * @brief Returns the corresponding instruction in a given sentence
 * @param sentence The given sentence where instruction will be looked for
 * @return Pointer to instruction structure if found, null pointer otherwise
*/
instruction const *ins_get_from_sentence(uint16_t sentence);

/**
 * @brief Returns the corresponding jump condition for a given instruction
 * @param instruction Pointer to given instruction structure
 * @return Jump condition code if any associated, 0 otherwise
*/
uint8_t ins_get_jmp_condition(instruction const *const instruction);

#endif /* INSTRUCTIONS_H */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h> /* uint16_t */
#include <stdbool.h> /* bool */
#include "hash_table.h" /* hash_table */

#define INSTRUCTION_MAX_LENGTH 4

/** @brief List of instructions' indexes */
enum ins_index {
	INS_I_ST, INS_I_LD, INS_I_STS, INS_I_LDS, INS_I_CALL,
	INS_I_RET, INS_I_BRZS, INS_I_BREQ, INS_I_BRCS, INS_I_BRLO,
	INS_I_BRVS, INS_I_BRLT, INS_I_JMP, INS_I_ADD, INS_I_SUB,
	INS_I_CP, INS_I_MOV, INS_I_CLC, INS_I_SEC, INS_I_ROR,
	INS_I_ROL, INS_I_STOP, INS_I_ADDI, INS_I_SUBI, INS_I_CPI,
	INS_I_LDI, INS_LEN
};
typedef enum ins_index ins_index;

/** @brief List of instructions' formats */
enum ins_format {
	INS_FORMAT_A, INS_FORMAT_B, INS_FORMAT_C, INS_FORMAT_D, INS_FORMAT_E
};
typedef enum ins_format ins_format;

/** @brief Base for our instruction hashtable.
	We save both name and opcode, so we can access
	them having the instruction index in the enum */
struct ins_data {
	ins_index index;
	char *name;
	uint8_t opcode;
	ins_format format;
};
typedef struct ins_data ins_data;

/** @brief Name and opcodes for each instruction */
extern ins_data const INS_LIST[];

/**
	@brief Initializes the hash table. Searching will be allowed from
	this point onwards.
*/
void ins_search_start(void);

/**
	@brief Destroys the hash table. Searching will be forbbiden from
	this point onwards.
*/
void ins_search_stop(void);

/**
	@brief Searchs for a instruction name an returns the corresponding index
	@param name Name of the instruction
	@return Pointer to ins_data structure if found, null pointer otherwise
*/
ins_data const *ins_search(char *name);

#endif /* INSTRUCTIONS_H */

/** @file as_parse.h */

#ifndef AS_PARSE_H
#define AS_PARSE_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <inttypes.h>

#include "../cs2010/cs_instructions.h"
#include "../cs2010/cs_memory.h"

#include "../hash_table.h"
#include "../trace_log.h"

#define AS_MAX_SOURCE_LINES 65535
#define AS_MAX_LINE_LENGTH 256
#define AS_MAX_TRACE_LENGTH 256
#define AS_MAX_LOG_LENGTH (AS_MAX_TRACE_LENGTH * 4)
#define AS_MAX_EQU_LENGTH 64	

/* The longest line, so 16 bytes is enough
	1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
	O  O  O  O     0  x  F  F  ,     0  X  F  F /0 */
#define AS_MAX_DISASSEMBLY_LENGTH 16

#define AS_ARGUMENT_TYPE_INM 0
#define AS_ARGUMENT_TYPE_EQU 1
#define AS_ARGUMENT_TYPE_INVALID 2

#define AS_PARSE_OK 0
#define AS_PARSE_WARNING 1
#define AS_PARSE_ERROR 2

struct as_parse_argument {
	union {
		uint8_t inm;
		char *equ_key;
	} value;
	uint8_t type;
};
typedef struct as_parse_argument as_parse_argument;

struct as_parse_sentence {
	cs_instruction const *instruction;
	as_parse_argument arg_a;
	as_parse_argument arg_b;
};
typedef struct as_parse_sentence as_parse_sentence;

/**
	@brief Structure containing all the information needed to assemble
	a CS2010 program after parsing the assembly code
*/
struct as_parse_info {
	/** @brief Index of current parsing line */
	size_t parsing_line_index;
	/** @brief Index of current sentence */
	size_t sentence_index;
	/** @brief Hash table containig all equ replacements */
	hash_table equs_ht;
	/** @brief Trace log for the assembly proccess */
	trace_log log;
	/** @brief Array containing all valid parsed sentences */
	as_parse_sentence sentences[CS_ROM_SIZE];
	/** @brief Array containing assembled machine code */
	uint16_t machine_code[CS_ROM_SIZE];
};
typedef struct as_parse_info as_parse_info;

/**
 * @brief Initializes a parse struct
 * @param pinfo Pointer to as_parse_info struct to be initialized
 * @return true if success, false otherwise
*/
bool as_parse_init(as_parse_info *pinfo);

/**
 * @brief Parses one line of CS2010 assembly code.
 *		This function must be called for every line of the assembly
 *		in order before calling parse_assemble.
 * @param pinfo Pointer to the parse struct that keeps track of
 *				parsing proccess
 * @param line Pointer to the line string containing the assembler
 * @return AS_PARSE_OK if line was parsed successfully,
 *			AS_PARSE_WARNING if line was ignored or
 *			AS_PARSE_ERROR if parsing should be aborted
*/
int as_parse_line(as_parse_info *pinfo, char const *line);

/**
 * @brief Assembles the parse struct (it contains the previously
		parsed assembly lines) into binary machine code.
 * @param pinfo Pointer to as_parse_info struct that keeps tract of
 *		parsing proccess
 * @return AS_PARSE_OK if machine code was assembled successfully,
 *			AS_PARSE_ERROR otherwise
*/
int as_parse_assemble(as_parse_info *pinfo);

/**
 * @brief Frees as_parse_info struct and associated memory
 * @param pinfo Pointer to as_parse_info structure
*/
void as_parse_free(as_parse_info *pinfo);

/**
 * @brief Disassembles a given raw, binary sentence of CS2010 machine
 *		code into human-readable CS2010 assembler. The returned string
 *		must be freed by the caller.
 * @param raw_sentence Binary CS2010 machine code sentence
 * @return Pointer to string containing the dissasembly if success,
 *		a null pointer otherwise (invalid binary code)
*/
char *as_disassemble_sentence(uint16_t raw_sentence);

#endif /* AS_PARSE_H */

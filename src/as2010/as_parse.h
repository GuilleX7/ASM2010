/** @file as_parse.h */

#ifndef AS_PARSE_H
#define AS_PARSE_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../cs2010/cs_instructions.h"
#include "../cs2010/cs_memory.h"

#include "../hash_table.h"

#define AS_MAX_SOURCE_LINES 65535
#define AS_MAX_LINE_LENGTH 256
#define AS_MAX_TRACE_LENGTH 512
#define AS_MAX_EQU_LENGTH 64

#define AS_ARGUMENT_TYPE_INM 0
#define AS_ARGUMENT_TYPE_EQU 1
#define AS_ARGUMENT_TYPE_INVALID 2

#define AS_PARSER_NAME "AS2010"
#define AS_PARSER_MAJOR_VERSION 0
#define AS_PARSER_MINOR_VERSION 2
#define AS_PARSER_PATCH_VERSION 0

struct as_parse_argument {
	union {
		uint8_t inm;
		char *equ_key;
	} value;
	uint8_t type;
};
typedef struct as_parse_argument as_parse_argument;

struct as_parse_sentence {
	cs_instruction const *cs_instruction;
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
	/** @brief Trace buffer */
	char *trace_buffer;
	/** @brief Trace string */
	char *trace;
	/** @brief Array containing all valid parsed sentences */
	as_parse_sentence sentences[CS_ROM_SIZE];
	/** @brief Array containing assembled machine code */
	uint16_t machine_code[CS_ROM_SIZE];
};
typedef struct as_parse_info as_parse_info;

enum as_parse_status {
	PARSE_OK, PARSE_WARNING, PARSE_ERROR
};
typedef enum as_parse_status as_parse_status;

/**
	@brief Initiliazes a as_parse_info struct
	@param pinfo Pointer to as_parse_info struct to be initialized
	@param traceout Pointer to tracing function
	@return true if success, false otherwise
*/
bool as_parse_init(as_parse_info *pinfo);

/**
	@brief Parses one line of the assembly code.
	This function must be called for every line of the assembly
	before calling parse_assemble.
	@param pinfo Pointer to as_parse_info struct that keeps track of
				parsing proccess
	@param line Pointer to the line string
	@return PARSE_OK if line was analysed successfully,
			PARSE_WARNING if line was ignored or
			PARSE_ERROR if analysis should be aborted
*/
as_parse_status as_parse_line(as_parse_info *pinfo, char const *line);

/**
	@brief Assembles the parsed lines of the assembly code into
		  binary machine code.
	@param pinfo Pointer to as_parse_info struct that keeps track of
				parsing proccess
	@return PARSE_OK if machine code was assembled successfully,
			PARSE_ERROR otherwise
*/
as_parse_status as_parse_assemble(as_parse_info *pinfo);

/**
	@brief Frees as_parse_info associated memory
	@param pinfo Pointer to as_parse_info structure
*/
void as_parse_free(as_parse_info *pinfo);

#endif /* AS_PARSE_H */

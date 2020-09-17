#ifndef AS2010_PARSE_H
#define AS2010_PARSE_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdio.h> /* sprintf */
#include <stdlib.h> /* malloc, free */
#include <stdint.h> /* int types */
#include <string.h> /* strlen */
#include <ctype.h> /* isalnum */
#include "../instructions.h" /* ins_list */
#include "../hash_table.h" /* hash_table */

#define MAX_SOURCE_LINES 65535
#define MAX_LINE_LENGTH 256
#define MAX_TRACE_LENGTH 512
#define MAX_EQU_LENGTH 64
#define ROM_SIZE 256
#define INM_VALUE_MAX 255

#define ARGUMENT_TYPE_INM 0
#define ARGUMENT_TYPE_EQU 1
#define ARGUMENT_TYPE_INVALID 2

struct parse_argument {
	union {
		uint8_t inm;
		char *equ_key;
	} value;
	uint8_t type;
};
typedef struct parse_argument parse_argument;

struct parse_sentence {
	instruction const *instruction;
	parse_argument arg_a;
	parse_argument arg_b;
};
typedef struct parse_sentence parse_sentence;

/**
	@brief Structure containing all the information needed to assemble
	a CS2010 program after parsing the assembly code
*/
struct parse_info {
	/** @brief Index of current parsing line */
	size_t parsing_line_index;
	/** @brief Index of current sentence */
	size_t sentence_index;
	/** @brief Hash table containig all equ replacements */
	hash_table equs_ht;
	/** @brief Trace buffer */
	char trace_buf[MAX_TRACE_LENGTH];
	/** @brief Trace string */
	char trace[MAX_TRACE_LENGTH * 2];
	/** @brief Array containing all valid parsed sentences */
	parse_sentence sentences[ROM_SIZE];
	/** @brief Array containing assembled binary code */
	uint16_t bincode[ROM_SIZE];
};
typedef struct parse_info parse_info;

enum parse_status {
	PARSE_OK, PARSE_WARNING, PARSE_ERROR
};
typedef enum parse_status parse_status;

/**
	@brief Initiliazes a parse_info struct
	@param pinfo Pointer to parse_info struct to be initialized
	@param traceout Pointer to tracing function
*/
void parse_init(parse_info *pinfo);

/**
	@brief Parses one line of the assembly code.
	This function must be called for every line of the assembly
	before calling parse_assemble.
	@param pinfo Pointer to parse_info struct that keeps track of
				parsing proccess
	@param line Pointer to the line string
	@return PARSE_OK if line was analysed successfully,
			PARSE_WARNING if line was ignored or
			PARSE_ERROR if analysis should be aborted
*/
parse_status parse_line(parse_info *pinfo, char const *line);

/**
	@brief Assembles the parsed lines of the assembly code into
		  binary machine code.
	@param pinfo Pointer to parse_info struct that keeps track of
				parsing proccess
	@return PARSE_OK if machine code was assembled successfully,
			PARSE_ERROR otherwise
*/
parse_status parse_assemble(parse_info *pinfo);

/**
	@brief Frees parse_info associated memory
	@param pinfo Pointer to parse_info structure
*/
void parse_free(parse_info *pinfo);

#endif /* AS2010_PARSE_H */

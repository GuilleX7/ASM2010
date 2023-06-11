/** @file as_parse.h */

#ifndef AS_PARSE_H
#define AS_PARSE_H

#include <stddef.h>

#include "../m2010/cs_instructions.h"
#include "../m2010/cs_platforms.h"

#include "../hash_table.h"
#include "../trace_log.h"
#include "../utils.h"

#include "../../include/asm2010.h"

#define AS_MAX_TRACE_LENGTH 256
#define AS_MAX_LOG_LENGTH   (AS_MAX_TRACE_LENGTH * 5)
#define AS_MAX_EQU_LENGTH   128

#define AS_ARGUMENT_TYPE_INM     0
#define AS_ARGUMENT_TYPE_EQU     1
#define AS_ARGUMENT_TYPE_INVALID 2
#define AS_ARGUMENT_DEFINE_OF_TYPE(type)                                                                               \
    { {0}, type }

typedef struct cs_as_machine_code cs_as_machine_code;

struct cs_as_parse_argument {
    union {
        size_t inm;
        char  *equ_key;
    } value;
    int type;
};
typedef struct cs_as_parse_argument cs_as_parse_argument;

struct cs_as_parse_sentence {
    cs_as_parse_argument  arg_a;
    cs_as_parse_argument  arg_b;
    cs_instruction const *instruction;
    size_t                parsing_source_line_index;
};
typedef struct cs_as_parse_sentence cs_as_parse_sentence;

/**
 * @brief Structure containing all the information needed to parse and
 *      assemble a CS program
 */
struct cs_as_parse_info {
    /** @brief Hash table containing all equ replacements */
    hash_table equs_ht;
    /** @brief Trace log for the assembly proccess */
    trace_log log;
    /** @brief Index of current parsing line */
    size_t source_assembly_line_index;
    /** @brief Maximum number of sentences allowed to parse */
    size_t max_allowed_parsed_sentences_amount;
    /** @brief Array containing all valid parsed sentences */
    cs_as_parse_sentence *parsed_sentences;
    /** @brief Amount of sentences */
    size_t parsed_sentences_amount;
    /** @brief Output machine code */
    cs_as_machine_code machine_code;
    /** @brief CS platform for which the machine code will be generated */
    cs_platform platform;
};
typedef struct cs_as_parse_info cs_as_parse_info;

#endif /* AS_PARSE_H */

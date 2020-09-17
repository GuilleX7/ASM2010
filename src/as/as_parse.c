#include "as_parse.h"

#define EQU_KEYWORD ".EQU "
#define EQU_KEYWORD_LENGTH strlen(EQU_KEYWORD)
#define EQU_LINE_MARK ':'
#define EQU_DEFINE_MARK '='
#define ARG_SEP_MARK ','
#define COMMENT_MARK ';'

#define trace(...) \
do {\
sprintf(pinfo->trace_buf, __VA_ARGS__);\
strcat(pinfo->trace, pinfo->trace_buf);\
} while (0);\

enum parse_line_status {
	PARSE_LINE_KEEP = 0, PARSE_LINE_WARNING, PARSE_LINE_ERROR, PARSE_LINE_END
};
typedef enum parse_line_status parse_line_status;

enum retrieve_value_status {
	RETRIEVE_VALUE_OK, RETRIEVE_VALUE_INVALID, RETRIEVE_VALUE_OUTOFBOUNDS
};
typedef enum retrieve_value_status retrieve_value_status;

/**
	@brief Computes the absolute value of its argument x.
	@param x input value.
	@return absolute value of \a x.
*/
static inline bool check_uint8_wrap(uint8_t addend1, uint8_t addend2, uint8_t divisor, uint8_t max) {
	if (!divisor) divisor = 1;
	return addend1 > (max - addend2) / divisor;
}

/**
	@brief Skips contiguous space characters
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_END if line ends with spaces,
			PARSE_LINE_KEEP otherwise
*/
static inline parse_line_status skip_spaces(char const **lineptr) {
	while (isspace((unsigned char) **lineptr) && **lineptr != '\n') (*lineptr)++;
	if (!**lineptr || **lineptr == '\n') return PARSE_LINE_END;
	return PARSE_LINE_KEEP;
}

/**
	@brief Skips commentaries
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_END if a comment is found,
			PARSE_LINE_KEEP otherwise
*/
static inline parse_line_status search_comment(char const **lineptr) {
	if (**lineptr == COMMENT_MARK) return PARSE_LINE_END;
	return PARSE_LINE_KEEP;
}

/**
	@brief Searchs for a line end or a commentary
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_END if line ends,
			PARSE_LINE_KEEP otherwise
*/
static inline parse_line_status search_line_end(char const **lineptr) {
	if (skip_spaces(lineptr) == PARSE_LINE_KEEP &&
		search_comment(lineptr) == PARSE_LINE_KEEP)
		return PARSE_LINE_KEEP;

	return PARSE_LINE_END;
}

/**
	@brief Retrieves the longest alpha-numeric identifier from the
	pointer
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@param max_len Maximum number of allowed characters
	@return Pointer to a new alpha-numeric identifier if possible,
			null pointer otherwise
*/
static char *retrieve_alnum_identifier(parse_info *pinfo, char const **lineptr, size_t max_len) {
	char *identifier;

	if (search_line_end(lineptr) == PARSE_LINE_END)
		return 0;

	if (!isalpha((unsigned char) **lineptr))
		return 0;

	identifier = malloc((max_len + 1) * sizeof(char));
	if (!identifier) {
		trace("[Memory] running out of memory, errors may appear after this");
		return 0;
	}

	size_t i = 0;
	while ((isalnum((unsigned char) **lineptr) || **lineptr == '_') && i < max_len) {
		*(identifier + i) = **lineptr;
		(*lineptr)++;
		i++;
	}
	*(identifier + i) = 0;

	if (i == 0 || (i == max_len && isalnum((unsigned char) **lineptr))) {
		/* Either we didn't capture characters at all nor we didn't capture
		the entire identifier, throw an error */
		free(identifier);
		identifier = 0;
	}

	return identifier;
}

/**
	@brief Gets a valid hexadecimal value that fits in a uint8_t.
	@param lineptr Pointer to pointer to the line string
	@param status Pointer to retrieve_value_status that will be set
				to RETRIEVE_VALUE_OK if success, RETRIEVE_VALUE_OUTOFBOUNDS
				if value was out of bounds or RETRIEVE_VALUE_INVALID if
				couldn't find any value
	@return Value if status is OK, garbage value otherwise
*/
static uint8_t retrieve_value_hexadecimal(char const **lineptr, retrieve_value_status *status) {
	uint8_t value = { 0 };
	uint8_t ch_value = 0;
	size_t valid_characters = 0;

	while (isxdigit((unsigned char) **lineptr)) {
		ch_value = isdigit(**lineptr) ? **lineptr - '0' : **lineptr - 'A' + 10;

		if (check_uint8_wrap(value, ch_value, 16, INM_VALUE_MAX)) {
			*status = RETRIEVE_VALUE_OUTOFBOUNDS;
			return value;
		}

		value = value * 16 + ch_value;
		(*lineptr)++;
		valid_characters++;
	}

	*status = (valid_characters) ? RETRIEVE_VALUE_OK : RETRIEVE_VALUE_INVALID;
	return value;
}

/**
	@brief Gets a valid binary value that fits in a uint8_t.
	@param lineptr Pointer to pointer to the line string
	@param status Pointer to retrieve_value_status that will be set
					to RETRIEVE_VALUE_OK if success, RETRIEVE_VALUE_OUTOFBOUNDS
					if value was out of bounds or RETRIEVE_VALUE_INVALID if
					couldn't find any value
	@return Value if status is OK, garbage value otherwise
*/
static uint8_t retrieve_value_binary(char const **lineptr, retrieve_value_status *status) {
	uint8_t value = { 0 };
	size_t valid_characters = 0;

	while (**lineptr == '0' || **lineptr == '1') {
		if (check_uint8_wrap(value, **lineptr - '0', 2, INM_VALUE_MAX)) {
			*status = RETRIEVE_VALUE_OUTOFBOUNDS;
			return value;
		}

		value = value * 2 + (**lineptr - '0');
		(*lineptr)++;
		valid_characters++;
	}

	*status = (valid_characters) ? RETRIEVE_VALUE_OK : RETRIEVE_VALUE_INVALID;
	return value;
}

/**
	@brief Gets a valid decimal value that fits in a uint8_t.
	Input can be unsigned (range [0, MAX_VALUE]) or signed
	(range [-MAX_VALUE/2, MAX_VALUE/2-1], but output will
	always be uint8_t, therefore unsigned
	@param lineptr Pointer to pointer to the line string
	@param status Pointer to retrieve_value_status that will be set
					to RETRIEVE_VALUE_OK if success, RETRIEVE_VALUE_OUTOFBOUNDS
					if value was out of bounds or RETRIEVE_VALUE_INVALID if
					couldn't find any value
	@return Value if status is OK, garbage value otherwise
*/
static uint8_t retrieve_value_decimal(char const **lineptr, retrieve_value_status *status) {
	uint8_t value = { 0 };
	size_t valid_characters = 0;
	bool negative = false;
	uint8_t max_value = INM_VALUE_MAX;
	if (**lineptr == '-') {
		negative = true;
		max_value = max_value / 2 + 1;
		(*lineptr)++;
	}

	while (isdigit((unsigned char) **lineptr)) {
		if (check_uint8_wrap(value, **lineptr - '0', 10, max_value)) {
			*status = RETRIEVE_VALUE_OUTOFBOUNDS;
			return (negative) ? -value : value;
		}

		value = value * 10 + (**lineptr - '0');
		(*lineptr)++;
		valid_characters++;
	}

	*status = (valid_characters) ? RETRIEVE_VALUE_OK : RETRIEVE_VALUE_INVALID;
	return (negative) ? -value : value;
}

/**
	@brief Gets either a valid decimal, hexadecimal or binary value
	that fits in a uint8_t.
	Input can be unsigned (range [0, MAX_VALUE]) or signed
	(range [-MAX_VALUE/2, MAX_VALUE/2-1], but output will
	always be uint8_t, therefore unsigned. It also can be
	hexadecimal starting by '0X' or binary starting by '0B'.
	@param lineptr Pointer to pointer to the line string
	@param status Pointer to retrieve_value_status that will be set
					to RETRIEVE_VALUE_OK if success, RETRIEVE_VALUE_OUTOFBOUNDS
					if value was out of bounds or RETRIEVE_VALUE_INVALID if
					couldn't find any value
	@return Value if status is OK, garbage value otherwise
*/
static uint8_t retrieve_value(char const **lineptr, retrieve_value_status *status) {
	uint8_t value = { 0 };

	if (search_line_end(lineptr) == PARSE_LINE_END) {
		*status = RETRIEVE_VALUE_INVALID;
		return value;
	}

	if (**lineptr == '0' && *(*lineptr + 1) == 'X') {
		/* Hexadecimal */
		(*lineptr) += 2;
		value = retrieve_value_hexadecimal(lineptr, status);
	} else if (**lineptr == '0' && *(*lineptr + 1) == 'B') {
		/* Binary */
		(*lineptr) += 2;
		value = retrieve_value_binary(lineptr, status);
	} else {
		/* Decimal */
		value = retrieve_value_decimal(lineptr, status);
	}

	return value;
}

/**
	@brief Searchs for the given the given symbol and forwards
	the pointer to the string
	@param lineptr Pointer to pointer to the line string
	@param ch Character to be searched for
	@return PARSE_LINE_KEEP if character is found and there's text
			after it or
			PARSE_LINE_END if either the character wasn't found or
			there is no text left after it.
*/
static inline parse_line_status search_character(char const **lineptr, char ch) {
	if (skip_spaces(lineptr) == PARSE_LINE_END)
		return PARSE_LINE_END;

	if (**lineptr == ch)
		(*lineptr)++;
	else
		return PARSE_LINE_END;

	return skip_spaces(lineptr);
}

/**
	@brief Search for user-defined equ
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_KEEP if no equ found,
			PARSE_LINE_END if either an equ was found or not, but
			line ended,
			PARSE_LINE_WARNING if equ declaration was found but ignored
			due to malforming or
			PARSE_LINE_ERROR if parsing should be aborted
*/
static parse_line_status search_defined_equ(parse_info *pinfo, char const **lineptr) {
	if (strncmp(*lineptr, EQU_KEYWORD, EQU_KEYWORD_LENGTH)) return PARSE_LINE_KEEP;
	(*lineptr) += EQU_KEYWORD_LENGTH;

	char *identifier = retrieve_alnum_identifier(pinfo, lineptr, MAX_EQU_LENGTH);
	if (!identifier) {
		trace("[Error] invalid identifier for equ at line %zu\n", pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	if (hash_table_exists(&pinfo->equs_ht, identifier)) {
		trace("[Warning] identifier '%s' declared at line %zu already exists, redeclaration "\
			"will be ignored\n", identifier, pinfo->parsing_line_index);
		free(identifier);
		return PARSE_LINE_WARNING;
	}

	if (search_character(lineptr, EQU_DEFINE_MARK) != PARSE_LINE_KEEP) {
		trace("[Error] token '%c' not found for identifier '%s' or value not specified at line %zu\n", EQU_DEFINE_MARK, identifier, pinfo->parsing_line_index);
		free(identifier);
		return PARSE_LINE_ERROR;
	}

	retrieve_value_status status;
	uint8_t value = retrieve_value(lineptr, &status);
	switch (status) {
	case RETRIEVE_VALUE_INVALID:
		trace("[Error] invalid value specified for equ identifier '%s' at line %zu\n", identifier, pinfo->parsing_line_index);
		free(identifier);
		return PARSE_LINE_ERROR;
	case RETRIEVE_VALUE_OUTOFBOUNDS:
		trace("[Error] value specified for equ identifier '%s' at line %zu is out of bounds\n", identifier, pinfo->parsing_line_index);
		free(identifier);
		return PARSE_LINE_ERROR;
	default:
		break;
	}

	if (search_line_end(lineptr) == PARSE_LINE_KEEP) {
		trace("[Error] expected end of line after equ definition at line %zu\n", pinfo->parsing_line_index);
		free(identifier);
		return PARSE_LINE_ERROR;
	}

	hash_table_put(&pinfo->equs_ht, identifier, &value);
	free(identifier);
	return PARSE_LINE_END;
}

/**
	@brief Search for line equ
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_KEEP if no equ found,
			PARSE_LINE_WARNING if equ declaration was found but ignored
			due to malforming or
			PARSE_LINE_ERROR if parsing should be aborted
*/
static parse_line_status search_line_equ(parse_info *pinfo, char const **lineptr) {
	char *comment_mark_ptr = strchr(*lineptr, COMMENT_MARK);
	char *equ_mark_ptr = strchr(*lineptr, EQU_LINE_MARK);
	if (!equ_mark_ptr || (comment_mark_ptr && comment_mark_ptr - equ_mark_ptr < 0))
		return PARSE_LINE_KEEP;

	char *identifier = retrieve_alnum_identifier(pinfo, lineptr, MAX_EQU_LENGTH);
	if (!identifier) {
		trace("[Error] invalid equ identifier at line %zu (perhaps not alphanumerical?)\n", pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	if (search_character(lineptr, EQU_LINE_MARK) == PARSE_LINE_END) {
		trace("[Error] token '%c' not found at equ definition or no instruction follows "\
			"the token at line %zu\n", EQU_LINE_MARK, pinfo->parsing_line_index);
		free(identifier);
		return PARSE_LINE_ERROR;
	}

	if (hash_table_exists(&pinfo->equs_ht, identifier)) {
		trace("[Warning] identifier '%s' declared at line %zu already exists, redeclaration "\
			"will be ignored\n", identifier, pinfo->parsing_line_index);
		free(identifier);
		return PARSE_LINE_WARNING;
	}

	/* Everything's seems alright, but we can't guarantee that we are pointing to
	   a valid sentence. Don't worry, instruction parsing stage will throw an error. */
	hash_table_put(&pinfo->equs_ht, identifier, &pinfo->sentence_index);
	free(identifier);
	return PARSE_LINE_KEEP;
}

/**
	@brief Searchs for a inmediate argument or an equ identifier
	@param lineptr Pointer to pointer to the line string
	@param status Pointer to a status to indicate the status of the
				value retrieval when argument is any type different
				from ARGUMENT_TYPE_EQU
	@return A valid parse argument if found,
			a parse argument with ARGUMENT_TYPE_INVALID type otherwise
*/
static parse_argument search_inm_equ(parse_info *pinfo, char const **lineptr, retrieve_value_status *status) {
	parse_argument argument = { .type = ARGUMENT_TYPE_INVALID };

	char *identifier = retrieve_alnum_identifier(pinfo, lineptr, MAX_EQU_LENGTH);
	if (identifier) {
		argument.type = ARGUMENT_TYPE_EQU;
		argument.value.equ_key = identifier;
	} else {
		argument.value.inm = retrieve_value(lineptr, status);
		if (*status == RETRIEVE_VALUE_OK)
			argument.type = ARGUMENT_TYPE_INM;
	}

	return argument;
}

/**
	@brief Searchs for a register argument
	@param lineptr Pointer to pointer to the line string
	@return A valid parse argument with type ARGUMENT_TYPE_INM if found,
			a parse argument with ARGUMENT_TYPE_INVALID type otherwise
*/
static parse_argument search_register(char const **lineptr, bool allow_indirect_syntax) {
	parse_argument argument = { .type = ARGUMENT_TYPE_INVALID };
	bool indirect = false;

	if (**lineptr == '(') {
		if (allow_indirect_syntax) {
			indirect = true;
			(*lineptr)++;
		} else {
			return argument;
		}
	}

	skip_spaces(lineptr);
	if (**lineptr == 'R' && *(*lineptr + 1) >= '0' && *(*lineptr + 1) <= '8') {
		argument.type = ARGUMENT_TYPE_INM;
		argument.value.inm = *(*lineptr + 1) - '0';
		*lineptr += 2;
	}
	skip_spaces(lineptr);

	if ((indirect && **lineptr != ')') || (!indirect && **lineptr == ')'))
		argument.type = ARGUMENT_TYPE_INVALID;
	else if (**lineptr == ')')
		(*lineptr)++;

	return argument;
}

/**
	@brief Frees a given parse argument
	@param arg_ptr Pointer to the parse argument to be freed
*/
static inline void free_argument(parse_argument *arg_ptr) {
	if (arg_ptr->type == ARGUMENT_TYPE_EQU &&
		arg_ptr->value.equ_key)
		free(arg_ptr->value.equ_key);
}

/**
	@brief Parses A-format instruction
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_KEEP if instruction was found,
			PARSE_LINE_ERROR if parsing should be aborted
*/
static parse_line_status parse_instruction_fa(parse_info *pinfo, char const **lineptr) {
	parse_sentence *sentence = &pinfo->sentences[pinfo->sentence_index];
	parse_argument *reg_dst = &sentence->arg_a;
	parse_argument *reg_src = &sentence->arg_b;

	//For some reason this is inverted...
	if (sentence->instruction->index == INS_I_ST) {
		reg_dst = &sentence->arg_b;
		reg_src = &sentence->arg_a;
	}

	*reg_dst = search_register(lineptr, (sentence->instruction->index == INS_I_ST) ? true : false);
	if (reg_dst->type == ARGUMENT_TYPE_INVALID) {
		trace("[Error] invalid register as parameter 1 of instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	if (search_character(lineptr, ARG_SEP_MARK) != PARSE_LINE_KEEP) {
		trace("[Error] invalid format for instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	*reg_src = search_register(lineptr, (sentence->instruction->index == INS_I_LD) ? true : false);
	if (reg_src->type == ARGUMENT_TYPE_INVALID) {
		trace("[Error] invalid register as parameter 2 of instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	return PARSE_LINE_KEEP;
}

/**
	@brief Parses B-format instruction
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_KEEP if instruction was found,
			PARSE_LINE_ERROR if parsing should be aborted
*/
static parse_line_status parse_instruction_fb(parse_info *pinfo, char const **lineptr) {
	parse_sentence *sentence = &pinfo->sentences[pinfo->sentence_index];
	parse_argument *reg_dst = &sentence->arg_a;
	parse_argument *inm_address = &sentence->arg_b;

	if (sentence->instruction->index == INS_I_STS) {
		retrieve_value_status status = { 0 };
		*inm_address = search_inm_equ(pinfo, lineptr, &status);
		if (inm_address->type == ARGUMENT_TYPE_INVALID) {
			trace("[Error] invalid value/address as parameter 1 of instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
			return PARSE_LINE_ERROR;
		}
	} else {
		*reg_dst = search_register(lineptr, false);
		if (reg_dst->type == ARGUMENT_TYPE_INVALID) {
			trace("[Error] invalid register as parameter 1 of instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
			return PARSE_LINE_ERROR;
		}
	}

	if (search_character(lineptr, ARG_SEP_MARK) != PARSE_LINE_KEEP) {
		trace("[Error] invalid format for instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	if (sentence->instruction->index == INS_I_STS) {
		*reg_dst = search_register(lineptr, false);
		if (reg_dst->type == ARGUMENT_TYPE_INVALID) {
			trace("[Error] invalid register as parameter 2 of instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
			return PARSE_LINE_ERROR;
		}
	} else {
		retrieve_value_status status = { 0 };
		*inm_address = search_inm_equ(pinfo, lineptr, &status);
		if (inm_address->type == ARGUMENT_TYPE_INVALID) {
			trace("[Error] invalid value/address as parameter 2 of instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
			return PARSE_LINE_ERROR;
		}
	}

	return PARSE_LINE_KEEP;
}

/**
	@brief Parses C-format instruction
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_KEEP if instruction was found,
			PARSE_LINE_ERROR if parsing should be aborted
*/
static parse_line_status parse_instruction_fc(parse_info *pinfo, char const **lineptr) {
	parse_sentence *sentence = &pinfo->sentences[pinfo->sentence_index];
	parse_argument *jump_condition = &sentence->arg_a;
	parse_argument *inm_address = &sentence->arg_b;

	jump_condition->type = ARGUMENT_TYPE_INM;
	jump_condition->value.inm = ins_get_jmp_condition(sentence->instruction);

	retrieve_value_status status = { 0 };
	*inm_address = search_inm_equ(pinfo, lineptr, &status);
	if (inm_address->type == ARGUMENT_TYPE_INVALID) {
		trace("[Error] invalid address as parameter 1 of instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	return PARSE_LINE_KEEP;
}

/**
	@brief Parses D-format instruction
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_KEEP if instruction was found,
			PARSE_LINE_ERROR if parsing should be aborted
*/
static parse_line_status parse_instruction_fd(parse_info *pinfo, char const **lineptr) {
	parse_sentence *sentence = &pinfo->sentences[pinfo->sentence_index];
	parse_argument *reg_dst = &sentence->arg_a;
	sentence->arg_b.type = ARGUMENT_TYPE_INM;
	sentence->arg_b.value.inm = 0;

	*reg_dst = search_register(lineptr, false);
	if (reg_dst->type == ARGUMENT_TYPE_INVALID) {
		trace("[Error] invalid register as parameter 1 of instruction '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	return PARSE_LINE_KEEP;
}

/**
	@brief Parses E-format instruction
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_KEEP if instruction was found,
			PARSE_LINE_ERROR if parsing should be aborted
*/
static parse_line_status parse_instruction_fe(parse_info *pinfo) {
	parse_sentence *sentence = &pinfo->sentences[pinfo->sentence_index];
	sentence->arg_a.type = ARGUMENT_TYPE_INM;
	sentence->arg_a.value.inm = 0;
	sentence->arg_b.type = ARGUMENT_TYPE_INM;
	sentence->arg_b.value.inm = 0;

	return PARSE_LINE_KEEP;
}

/**
	@brief Search for instruction
	@param pinfo Pointer to parse_info struct
	@param lineptr Pointer to pointer to the line string
	@return PARSE_LINE_KEEP if instruction was found,
			PARSE_LINE_ERROR if parsing should be aborted
*/
static parse_line_status search_instruction(parse_info *pinfo, char const **lineptr) {
	char *instruction_name = retrieve_alnum_identifier(pinfo, lineptr, INSTRUCTION_MAX_LENGTH);
	if (!instruction_name) {
		trace("[Error] invalid instruction at line %zu\n", pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	parse_sentence *sentence = &pinfo->sentences[pinfo->sentence_index];
	sentence->instruction = ins_search_by_name(instruction_name);
	if (!sentence->instruction) {
		trace("[Error] unknown instruction '%s' at line %zu\n", instruction_name, pinfo->parsing_line_index);
		free(instruction_name);
		return PARSE_LINE_ERROR;
	}
	free(instruction_name);

	skip_spaces(lineptr);
	parse_line_status result = { 0 };
	switch (sentence->instruction->format) {
	case INS_FORMAT_A:
		result = parse_instruction_fa(pinfo, lineptr);
		break;
	case INS_FORMAT_B:
		result = parse_instruction_fb(pinfo, lineptr);
		break;
	case INS_FORMAT_C:
		result = parse_instruction_fc(pinfo, lineptr);
		break;
	case INS_FORMAT_D:
		result = parse_instruction_fd(pinfo, lineptr);
		break;
	case INS_FORMAT_E:
		result = parse_instruction_fe(pinfo);
		break;
	default:
		trace("[Error] unknown instruction type for '%s' at line %zu\n", sentence->instruction->name, pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	switch (result) {
	case PARSE_LINE_ERROR:
		return PARSE_LINE_ERROR;
	default:
		break;
	}

	if (search_line_end(lineptr) == PARSE_LINE_KEEP) {
		trace("[Error] expected end of the line after instruction arguments at line %zu\n", pinfo->parsing_line_index);
		return PARSE_LINE_ERROR;
	}

	if (pinfo->sentence_index + 1 >= ROM_SIZE) {
		trace("[Error] maximum sentence lines reached (%u)\n", ROM_SIZE);
		return PARSE_LINE_ERROR;
	}
	pinfo->sentence_index++;
	return PARSE_LINE_END;
}

void parse_init(parse_info *pinfo) {
	pinfo->parsing_line_index = 0;
	pinfo->sentence_index = 0;
	hash_table_init(&pinfo->equs_ht);
}

void parse_free(parse_info *pinfo) {
	hash_table_free(&pinfo->equs_ht);
	for (size_t i = 0; i < pinfo->sentence_index; i++) {
		free_argument(&pinfo->sentences[i].arg_a);
		free_argument(&pinfo->sentences[i].arg_b);
	}
}

parse_status parse_line(parse_info *pinfo, char const *line) {
	char const **lineptr = &line;
	pinfo->trace_buf[0] = '\0';
	pinfo->trace[0] = '\0';

	/* Check lines */
	if (pinfo->parsing_line_index > MAX_SOURCE_LINES) {
		trace("[Error] maximum source lines reached (%u)\n", MAX_SOURCE_LINES);
		return PARSE_ERROR;
	}
	pinfo->parsing_line_index++;

	/* Check line length */
	if (strlen(line) > MAX_LINE_LENGTH + 1) {
		trace("[Error] invalid line length at line %zu\n", pinfo->parsing_line_index);
		return PARSE_ERROR;
	}

	/* Skip commentaries */
	if (search_line_end(lineptr))
		return PARSE_OK;

	/* Search user-defined equs */
	switch (search_defined_equ(pinfo, lineptr)) {
	case PARSE_LINE_END:
		return PARSE_OK;
	case PARSE_LINE_ERROR:
		return PARSE_ERROR;
	case PARSE_LINE_WARNING:
		return PARSE_WARNING;
	default:
		break;
	}

	/* Search line equs */
	switch (search_line_equ(pinfo, lineptr)) {
	case PARSE_LINE_ERROR:
		return PARSE_ERROR;
	case PARSE_LINE_WARNING:
		return PARSE_WARNING;
	default:
		break;
	}

	/* Search instruction */
	switch (search_instruction(pinfo, lineptr)) {
	case PARSE_LINE_END:
		return PARSE_OK;
	case PARSE_LINE_ERROR:
		return PARSE_ERROR;
	default:
		break;
	}

	return PARSE_OK;
}

parse_status parse_assemble(parse_info *pinfo) {
	parse_sentence *sentence = { 0 };
	uint16_t *bincode = { 0 };
	uint8_t *value_ptr = { 0 };
	uint8_t value = 0;
	
	pinfo->trace_buf[0] = '\0';
	pinfo->trace[0] = '\0';

	if (pinfo->sentence_index < 1) {
		trace("[Error] at least one valid sentence is required\n");
		return PARSE_ERROR;
	}

	for (size_t i = 0; i < pinfo->sentence_index; i++) {
		sentence = &pinfo->sentences[i];
		bincode = &pinfo->bincode[i];

		*bincode = 0 | ((unsigned) sentence->instruction->opcode << 11);

		if (sentence->arg_a.type == ARGUMENT_TYPE_EQU) {
			value_ptr = hash_table_get(&pinfo->equs_ht, sentence->arg_a.value.equ_key);
			if (!value_ptr) {
				trace("[Error] Equ '%s' of instruction '%s' at instruction line %zu couldn't be resolved\n",
					sentence->arg_a.value.equ_key, sentence->instruction->name, i + 1);
				return PARSE_ERROR;
			} else {
				value = *value_ptr;
			}
		} else {
			value = sentence->arg_a.value.inm;
		}
		*bincode |= ((unsigned) value & 0x1F) << 8;

		if (sentence->arg_b.type == ARGUMENT_TYPE_EQU) {
			value_ptr = hash_table_get(&pinfo->equs_ht, sentence->arg_b.value.equ_key);
			if (!value_ptr) {
				trace("[Error] Equ '%s' of instruction '%s' at instruction line %zu couldn't be resolved\n",
					sentence->arg_b.value.equ_key, sentence->instruction->name, i + 1);
				return PARSE_ERROR;
			} else {
				value = *value_ptr;
			}
		} else {
			value = sentence->arg_b.value.inm;
		}
		*bincode |= (unsigned) value;
	}

	return PARSE_OK;
}

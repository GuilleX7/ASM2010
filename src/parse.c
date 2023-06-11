/** @file parse.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include "parse.h"

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define HEXADECIMAL_RADIX 16
#define DECIMAL_RADIX     10
#define BINARY_RADIX      2

bool read_upper_line(char *line, size_t max_length, char const *const str, size_t *offset) {
    size_t i = 0;
    while (str[*offset] != '\0' && str[*offset] != '\n' && i < max_length - 1) {
        line[i] = toupper((unsigned)str[*offset]);
        (*offset)++;
        i++;
    }
    line[i] = '\0';

    // Consume the characters that don't fit in the buffer
    for (; str[*offset] != '\0' && str[*offset] != '\n'; (*offset)++)
        ;

    if (str[*offset] == '\n') {
        (*offset)++;
        return true;
    } else if (str[*offset] == '\0' && i > 0) {
        return true;
    } else {
        return false;
    }
}

int skip_spaces(char const **line_tracker) {
    while (isspace((unsigned char)**line_tracker) && **line_tracker != '\n') {
        (*line_tracker)++;
    }
    if (!**line_tracker || **line_tracker == '\n') {
        return PARSE_LINE_END;
    }
    return PARSE_LINE_KEEP;
}

int search_character(char const **line_tracker, char ch, int skip_spaces(char const **)) {
    if (skip_spaces(line_tracker) == PARSE_LINE_END) {
        return PARSE_LINE_END;
    }

    if (**line_tracker == ch) {
        (*line_tracker)++;
    } else {
        return PARSE_LINE_END;
    }

    return skip_spaces(line_tracker);
}

char *retrieve_alnum_identifier(char const **line_tracker, size_t max_length, int skip_spaces(char const **)) {
    char  *identifier;
    size_t i = 0;

    if (skip_spaces(line_tracker) == PARSE_LINE_END) {
        return 0;
    }

    if (!isalpha((unsigned char)**line_tracker)) {
        return 0;
    }

    identifier = malloc((max_length + 1) * sizeof(char));
    if (!identifier) {
        return 0;
    }

    while ((isalnum((unsigned char)**line_tracker) || **line_tracker == '_') && i < max_length) {
        *(identifier + i) = **line_tracker;
        (*line_tracker)++;
        i++;
    }
    *(identifier + i) = 0;

    if (i == 0 || (i == max_length && (isalnum((unsigned char)**line_tracker) || **line_tracker == '_'))) {
        /* Either we didn't capture characters at all or we didn't capture
        the entire identifier, throw an error */
        free(identifier);
        identifier = 0;
    }

    return identifier;
}

size_t retrieve_value(char const **line_tracker, int *status, size_t max_value, int skip_spaces(char const **)) {
    size_t value = 0;

    if (skip_spaces(line_tracker) == PARSE_LINE_END) {
        *status = RETRIEVE_VALUE_INVALID;
        return value;
    }

    if (**line_tracker == '0' && *(*line_tracker + 1) == 'X') {
        /* Hexadecimal using 0x prefix */
        (*line_tracker) += 2;
        value = retrieve_value_hexadecimal(line_tracker, status, max_value);
    } else if (**line_tracker == '$') {
        /* Hexadecimal using $ prefix */
        (*line_tracker)++;
        value = retrieve_value_hexadecimal(line_tracker, status, max_value);
    } else if (**line_tracker == '0' && *(*line_tracker + 1) == 'B') {
        /* Binary */
        (*line_tracker) += 2;
        value = retrieve_value_binary(line_tracker, status, max_value);
    } else {
        /* Decimal */
        value = retrieve_value_decimal(line_tracker, status, max_value);
    }

    return value;
}

static int is_binary_character(int character) {
    return character == '0' || character == '1';
}

static unsigned char get_numeric_value_from_character(unsigned char character) {
    return character - '0';
}

static unsigned char get_hexadecimal_value_from_character(unsigned char character) {
    if (isdigit(character)) {
        return get_numeric_value_from_character(character);
    } else if (isupper(character)) {
        return character - 'A' + 10;
    } else {
        return character - 'a' + 10;
    }
}

static size_t _retrieve_value(char const **line_tracker, int *status, size_t max_value, size_t radix,
                              int           is_character_valid(int character),
                              unsigned char get_value_from_character(unsigned char character), bool allow_signedness) {
    size_t        value            = 0;
    unsigned char character_value  = 0;
    size_t        valid_characters = 0;
    bool          negative         = false;

    if (allow_signedness) {
        if (**line_tracker == '-') {
            negative  = true;
            max_value = max_value / 2 + 1;
            (*line_tracker)++;
        } else if (**line_tracker == '+') {
            (*line_tracker)++;
        }
    }

    while (is_character_valid(**line_tracker)) {
        character_value = get_value_from_character(**line_tracker);

        if (check_unsigned_multiply_wrap(value, radix)) {
            *status = RETRIEVE_VALUE_OUTOFBOUNDS;
            return value;
        }
        value *= radix;
        if (check_unsigned_add_wrap(value, character_value)) {
            *status = RETRIEVE_VALUE_OUTOFBOUNDS;
            return value;
        }
        value += character_value;
        if (value > max_value) {
            *status = RETRIEVE_VALUE_OUTOFBOUNDS;
            return value;
        }

        (*line_tracker)++;
        valid_characters++;
    }

    *status = (valid_characters) ? RETRIEVE_VALUE_OK : RETRIEVE_VALUE_INVALID;
    return (negative) ? -value : value;
}

size_t retrieve_value_hexadecimal(char const **line_tracker, int *status, size_t max_value) {
    return _retrieve_value(line_tracker, status, max_value, HEXADECIMAL_RADIX, isxdigit,
                           get_hexadecimal_value_from_character, false);
}

size_t retrieve_value_binary(char const **line_tracker, int *status, size_t max_value) {
    return _retrieve_value(line_tracker, status, max_value, BINARY_RADIX, is_binary_character,
                           get_numeric_value_from_character, false);
}

size_t retrieve_value_decimal(char const **line_tracker, int *status, size_t max_value) {
    return _retrieve_value(line_tracker, status, max_value, DECIMAL_RADIX, isdigit, get_numeric_value_from_character,
                           true);
}

bool check_unsigned_add_wrap(size_t a, size_t b) {
    return SIZE_MAX - a < b;
}

bool check_unsigned_multiply_wrap(size_t a, size_t b) {
    return a ? SIZE_MAX / a < b : false;
}

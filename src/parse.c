/** @file parse.c */

#include "parse.h"

int skip_spaces(char const **lineptr) {
    while (isspace((unsigned char) **lineptr) && **lineptr != '\n') {
        (*lineptr)++;
    }
    if (!**lineptr || **lineptr == '\n') {
        return PARSE_LINE_END;
    }
    return PARSE_LINE_KEEP;
}

int search_character(char const **lineptr, char ch, int skip_spaces(char const**)) {
    if (skip_spaces(lineptr) == PARSE_LINE_END) {
        return PARSE_LINE_END;
    }

    if (**lineptr == ch) {
        (*lineptr)++;
    } else {
        return PARSE_LINE_END;
    }

    return skip_spaces(lineptr);
}

char *retrieve_alnum_identifier(char const **lineptr, size_t max_len, int skip_spaces(char const **)) {
    char *identifier;

    if (skip_spaces(lineptr) == PARSE_LINE_END)
        return 0;

    if (!isalpha((unsigned char) **lineptr))
        return 0;

    identifier = malloc((max_len + 1) * sizeof(char));
    if (!identifier) {
        return 0;
    }

    size_t i = 0;
    while ((isalnum((unsigned char) **lineptr) || **lineptr == '_') && i < max_len) {
        *(identifier + i) = **lineptr;
        (*lineptr)++;
        i++;
    }
    *(identifier + i) = 0;

    if (i == 0 || (i == max_len && (isalnum((unsigned char) **lineptr) || **lineptr == '_'))) {
        /* Either we didn't capture characters at all nor we didn't capture
        the entire identifier, throw an error */
        free(identifier);
        identifier = 0;
    }

    return identifier;
}

size_t retrieve_value(char const **lineptr, int *status, size_t max_value, int skip_spaces(char const **)) {
    size_t value = { 0 };

    if (skip_spaces(lineptr) == PARSE_LINE_END) {
        *status = RETRIEVE_VALUE_INVALID;
        return value;
    }

    if (**lineptr == '0' && *(*lineptr + 1) == 'X') {
        /* Hexadecimal */
        (*lineptr) += 2;
        value = retrieve_value_hexadecimal(lineptr, status, max_value);
    } else if (**lineptr == '0' && *(*lineptr + 1) == 'B') {
        /* Binary */
        (*lineptr) += 2;
        value = retrieve_value_binary(lineptr, status, max_value);
    } else {
        /* Decimal */
        value = retrieve_value_decimal(lineptr, status, max_value);
    }

    return value;
}

size_t retrieve_value_hexadecimal(char const **lineptr, int *status, size_t max_value) {
    size_t value = { 0 };
    unsigned char ch_value = 0;
    size_t valid_characters = 0;

    while (isxdigit((unsigned char) **lineptr)) {
        if (isdigit((unsigned char) **lineptr)) {
            ch_value = **lineptr - '0';
        } else if (isupper(**lineptr)) {
            ch_value = **lineptr - 'A' + 10;
        } else {
            ch_value = **lineptr - 'a' + 10;
        }

        if (check_unsigned_wrap(value, ch_value, 16, max_value)) {
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

size_t retrieve_value_binary(char const **lineptr, int *status, size_t max_value) {
    size_t value = { 0 };
    size_t valid_characters = 0;

    while (**lineptr == '0' || **lineptr == '1') {
        if (check_unsigned_wrap(value, (size_t) **lineptr - '0', 2, max_value)) {
            *status = RETRIEVE_VALUE_OUTOFBOUNDS;
            return value;
        }

        value = value * 2 + ((size_t) **lineptr - '0');
        (*lineptr)++;
        valid_characters++;
    }

    *status = (valid_characters) ? RETRIEVE_VALUE_OK : RETRIEVE_VALUE_INVALID;
    return value;
}

size_t retrieve_value_decimal(char const **lineptr, int *status, size_t max_value) {
    size_t value = { 0 };
    size_t valid_characters = 0;
    bool negative = false;
    if (**lineptr == '-') {
        negative = true;
        max_value = max_value / 2 + 1;
        (*lineptr)++;
    }

    while (isdigit((unsigned char) **lineptr)) {
        if (check_unsigned_wrap(value, (size_t) **lineptr - '0', 10, max_value)) {
            *status = RETRIEVE_VALUE_OUTOFBOUNDS;
            return (negative) ? max_value - value : value;
        }

        value = value * 10 + ((size_t) **lineptr - '0');
        (*lineptr)++;
        valid_characters++;
    }

    *status = (valid_characters) ? RETRIEVE_VALUE_OK : RETRIEVE_VALUE_INVALID;
    return (negative) ? max_value - value : value;
}

bool check_unsigned_wrap(size_t accumulator, size_t addend, size_t multiplier, size_t max) {
    if (!multiplier) multiplier = 1;
    return accumulator > (max - addend) / multiplier;
}
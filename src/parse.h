#ifndef PARSE_H
#define PARSE_H

#include <stdbool.h>
#include <stddef.h>

#define PARSE_LINE_KEEP 0
#define PARSE_LINE_WARNING 1
#define PARSE_LINE_ERROR 2
#define PARSE_LINE_END 3

#define RETRIEVE_VALUE_OK 0
#define RETRIEVE_VALUE_INVALID 1
#define RETRIEVE_VALUE_OUTOFBOUNDS 2

/**
 * @brief Skips contiguous space characters
 * @param lineptr Pointer to pointer to the line string
 * @return PARSE_LINE_END if line ends with spaces,
 *          PARSE_LINE_KEEP otherwise
*/
int skip_spaces(char const **lineptr);

/**
 * @brief Searchs for a given symbol and skips it
 * @param lineptr Pointer to pointer to the line string
 * @param ch Character to be looked for
 * @param skip_spaces Pointer to the space skipping function
 * @return PARSE_LINE_KEEP if character is found and there's text
 *          after it or
 *          PARSE_LINE_END if either the character wasn't found or
 *          there is no text left after it
*/
int search_character(char const **lineptr, char ch, int skip_spaces(char const **));

/**
 * @brief Retrieves the longest alpha-numeric identifier from the
 *      pointer
 * @param lineptr Pointer to pointer to the line string 
 * @param max_length Maximum number of allowed characters
 * @param skip_spaces Pointer to the space skipping function
 * @return Pointer to a new alpha-numeric identifier if possible,
 *          null pointer otherwise
*/
char *retrieve_alnum_identifier(char const **lineptr, size_t max_length, int skip_spaces(char const **));

/**
 * @brief Gets either a valid decimal, hexadecimal or binary value
 *        that fits in the given range.
 *        Input can be unsigned (range [0, MAX_VALUE]), or signed
 *        (range [-MAX_VALUE/2, MAX_VALUE/2-1], but output will always be
 *        size_t, therefore unsigned. It also can be hexadecimal
 *        starting by '0X' or binary starting by '0B'.
 * @param lineptr Pointer to pointer to the line string
 * @param status Pointer to the status that will be set to
 *              RETRIEVE_VALUE_OK if success,
 *              RETRIEVE_VALUE_OUTOFBOUNDS if value was out of range or
 *              RETRIEVE_VALUE_INVALID if couldn't find any value
 * @param max_value Maximum value allowed
 * @param skip_spaces Pointer to the space skipping function
 * @return Retrieved value if status is OK, garbage value otherwise
*/
size_t retrieve_value(char const **lineptr, int *status, size_t max_value, int skip_spaces(char const **));

/**
    @brief Gets a valid hexadecimal value that fits in a size_t.
    @param lineptr Pointer to pointer to the line string
    @param status Pointer to int that will be set
                to RETRIEVE_VALUE_OK if success, RETRIEVE_VALUE_OUTOFBOUNDS
                if value was out of bounds or RETRIEVE_VALUE_INVALID if
                couldn't find any value
    @return Value if status is OK, garbage value otherwise
*/

/**
 * @brief Gets a valid hexadecimal value that fits in the given range.
 * @param lineptr Pointer to pointer to the line string
 * @param status Pointer to the status that will be set to
 *              RETRIEVE_VALUE_OK if success,
 *              RETRIEVE_VALUE_OUTOFBOUNDS if value was out of range or
 *              RETRIEVE_VALUE_INVALID if couldn't find any value
 * @param max_value Maximum value allowed
 * @return Retrieved value if status is OK, garbage value otherwise
*/
size_t retrieve_value_hexadecimal(char const **lineptr, int *status, size_t max_value);

/**
 * @brief Gets a valid binary value that fits in the given range.
 * @param lineptr Pointer to pointer to the line string
 * @param status Pointer to the status that will be set to
 *              RETRIEVE_VALUE_OK if success,
 *              RETRIEVE_VALUE_OUTOFBOUNDS if value was out of range or
 *              RETRIEVE_VALUE_INVALID if couldn't find any value
 * @param max_value Maximum value allowed
 * @return Retrieved value if status is OK, garbage value otherwise
*/
size_t retrieve_value_binary(char const **lineptr, int *status, size_t max_value);

/**
 * @brief Gets a valid decimal value that fits in the given range.
 *        Input can be unsigned (range [0, MAX_VALUE]), or signed
 *        (range [-MAX_VALUE/2, MAX_VALUE/2-1], but output will always be
 *        size_t, therefore unsigned.
 * @param lineptr Pointer to pointer to the line string
 * @param status Pointer to the status that will be set to
 *              RETRIEVE_VALUE_OK if success,
 *              RETRIEVE_VALUE_OUTOFBOUNDS if value was out of range or
 *              RETRIEVE_VALUE_INVALID if couldn't find any value
 * @param max_value Maximum value allowed
 * @return Retrieved value if status is OK, garbage value otherwise
*/
size_t retrieve_value_decimal(char const **lineptr, int *status, size_t max_value);

/**
 * @brief Checks whether a given unsigned number (accumulator) would wrap
 *      after multiplying it by a number and adding another number.
 * @param accumulator Value accumulated (to be multiplied and added)
 * @param addend Value to be added to accumulator
 * @param multiplier Multiplier value
 * @param max Maximum value for accumulator that won't wrap after
 *          carrying out the operations
 * @return true if the accumulator would wrap, false otherwise
*/
bool check_unsigned_wrap(size_t accumulator, size_t addend, size_t multiplier, size_t max);

#endif /* PARSE_H */
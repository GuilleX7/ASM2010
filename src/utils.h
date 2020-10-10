/** @file utils.h */

#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

#define STRGIFY(a) #a
#define STRINGIFY(a) STRGIFY(a)

#define COUNT_DIGITS(n, digits) \
size_t digits = 1;\
do {\
size_t number = n;\
while (number > 9) {\
number /= 10;\
digits++;\
}\
} while (0);\

#ifdef _WIN32
#  ifdef _WIN64
#    define PRI_SIZET PRIu64
#  else
#    define PRI_SIZET PRIu32
#  endif /* _WIN64 */
#else
#  define PRI_SIZET "zu"
#endif /* _WIN32 */

#define UINT8_HEX_FORMAT "%02X"
#define UINT16_HEX_FORMAT "%04X"
#define UINT32_HEX_FORMAT "%08X"
#define UINT64_HEX_FORMAT "%016X"

/**
 * @brief Saves the given text to a file
 * @param filepath Path to the file
 * @param str Pointer to text to be saved
 * @return true if file was successfully saves,
 *          else otherwise
*/
bool save_file(char const *filepath, char const *str);

/**
 * @brief Reads the entire content of a file
 * @param filepath Path to the file
 * @return Dynamically allocated array of char containing the
 *          entire text
*/
char *read_file(char const *filepath);

/**
 * @brief Gets a file name from its path
 * @param filepath Pointer to file path
 * @return A pointer starting at the file name (without directories)
*/
char const *get_file_name(char const *filepath);

/**
 * @brief Gets a file path and returns a new allocated
 *		  string with the new extension
 * @param filepath Pointer to file name
 * @param new_extension Pointer to new extension (including dot)
 * @return A pointer to new allocated string containing the new file
 *		   path with new extension
*/
char *change_path_extension(char const *filepath, char const *new_extension);

/**
 * @brief Reads a line of a given string in uppercase
 * @param line Pointer to the buffer used to store the line
 * @param max_length Maximum length of the line to be buffered
 * @param str Pointer to the string to be readed from
 * @param offset Should be a pointer to 0-integer first time this
 *              function is called. Successive calls needs the
 *              same integer object to be passed in order to keep
 *              track of the position
 * @return false if there are no more lines left,
 *         true otherwise
*/
bool read_upper_line(char *line, size_t max_length, char const *const str, size_t *offset);

#endif /* UTILS_H */

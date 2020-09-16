#ifndef FILE_H
#define FILE_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#define EXPORT_SUCCESS 0
#define EXPORT_FILE_ERROR 1
#define EXPORT_ERROR 2

#define EXPORT_FORMAT_BIN 0
#define EXPORT_FORMAT_HEX 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @brief Gets a file name from its path
 * @param file_path Pointer to file path
 * @return A pointer starting at the file name (without directories)
*/
const char *get_file_name(const char *file_path);

/**
 * @brief Gets a file path and returns a new allocated
 *		  string with the new extension
 * @param file_name Pointer to file name
 * @param new_extension Pointer to new extension (including dot)
 * @return A pointer to new allocated string containing the new file
 *		   path with new extension
*/
char *change_path_extension(const char *file_path, const char *new_extension);

/**
 * @brief Exports a given parse_info struct to a file with
 *		  the given format
 * @param file_path File where pinfo will be exported to
 * @param pinfo Pointer to parse_info structure
 * @param export_format Hexadecimal or binary formats
 * @return 
*/
int export_code_to_file(const char *file_path, uint16_t *code, size_t code_size, int export_format);

#endif /* FILE_H */
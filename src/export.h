#ifndef EXPORT_H
#define EXPORT_H

#define EXPORT_SUCCESS 0
#define EXPORT_FILE_ERROR 1
#define EXPORT_ERROR 2

#define EXPORT_FORMAT_BIN 0
#define EXPORT_FORMAT_HEX 1

#include "parse.h"

/**
 * @brief Gets a file name from its URI
 * @param file_path Pointer to file URI
 * @return A pointer starting at the file name (without directories)
*/
const char *get_file_name(const char *file_path);

/**
 * @brief Gets a file name (or URI) and returns a new allocated
 *		  string with the new extension
 * @param file_name Pointer to file name
 * @param new_extension Pointer to new extension (including dot)
 * @return A pointer to new allocated string containing the new file
 *		   name with new extension
*/
char *change_file_extension(const char *file_name, const char *new_extension);

/**
 * @brief Exports a given parse_info struct to a file with
 *		  the given format
 * @param file_path File where pinfo will be exported to
 * @param pinfo Pointer to parse_info structure
 * @param export_format Hexadecimal or binary formats
 * @return 
*/
int export_pinfo_file(const char *file_path, parse_info *pinfo, int export_format);

#endif /* EXPORT_H */
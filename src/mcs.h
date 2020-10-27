/** @file mcs.h */

#ifndef MCS_H
#define MCS_H

#include <stddef.h>

#define MCS_EXPORT_SUCCESS 0
#define MCS_EXPORT_FILE_ERROR 1
#define MCS_EXPORT_ERROR 2

#define MCS_IMPORT_SUCCESS 0
#define MCS_IMPORT_FILE_ERROR 1
#define MCS_IMPORT_ERROR 2

#define MCS_FILE_EXT ".hex"

/**
 * @brief Exports a MachineCS file
 * @param filepath Output file
 * @param code Array of machine code
 * @param code_size Size of the machine code (16-bit words)
 * @return MCS_EXPORT_SUCCESS if succeded,
 *         MCS_EXPORT_FILE_ERROR if couldn't open the file or
 *         MCS_EXPORT_ERROR if some error happened while writing the file
*/
int mcs_export_file(char const *filepath, unsigned short *code, size_t code_size);

/**
 * @brief Imports a MachineCS file
 * @param fìlepath Input file
 * @param code Will point to an array containing the machine code,
 *              or null pointer if couldn't load the code
 * @param code_size Size of the loaded machine code (16-bit words)
 * @return MCS_IMPORT_SUCCESS if succeded,
 *         MCS_IMPORT_FILE_ERROR if couldn't open the file or
 *         MCS_IMPORT_ERROR if file wasn't a valid MCS file
*/
int mcs_import_file(char const *filepath, unsigned short **code, size_t *code_size);

#endif /* MCS_H */

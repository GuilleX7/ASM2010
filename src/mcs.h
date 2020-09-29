/** @file mcs.h */

#ifndef MCS_H
#define MCS_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#define MCS_EXPORT_SUCCESS 0
#define MCS_EXPORT_FILE_ERROR 1
#define MCS_EXPORT_ERROR 2

#define MCS_FORMAT_BIN 0
#define MCS_FORMAT_HEX 1

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Exports a MachineCS file
 * @param filepath Output file
 * @param code Array of machine code
 * @param code_size Size of the machine code (in 16-bit words)
 * @param mcs_format MCS output format
 * @return 
*/
int mcs_export_file(char const *filepath, uint16_t *code, size_t code_size, int mcs_format);

#endif /* MCS_H */

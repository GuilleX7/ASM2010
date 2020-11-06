/** @file mcs.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "parse.h"
#include "mcs.h"

#define CODE_MAX_VALUE 0xFFFF
#define MAX_LINE_LENGTH 16 /* 16 bytes should be more than enough */

static char const *UPPER_MCS_SIGNATURE = "V2.0 RAW";
static char const *LOWER_MCS_SIGNATURE = "v2.0 raw";

int mcs_export_file(char const *filepath, unsigned short *code, size_t code_size) {
    size_t i = 0;
    FILE *file = fopen(filepath, "w");
    int status = MCS_EXPORT_FILE_ERROR;
    if (!file) {
        return status;
    }

    fprintf(file, "%s\n", LOWER_MCS_SIGNATURE);
    for (i = 0; i < code_size; i++) {
        fprintf(file, "%x\n", code[i] & CODE_MAX_VALUE);
    }

    status = (ferror(file)) ? MCS_EXPORT_ERROR : MCS_EXPORT_SUCCESS;
    fclose(file);
    return status;
}

int mcs_import_file(char const *filepath, unsigned short **code, size_t *code_size) {
    char *str = read_file(filepath);
    char line[MAX_LINE_LENGTH];
    char const *lineptr = 0;
    size_t offset = 0;
    size_t code_offset = 0;
    size_t i = 0;
    int status;
    *code = 0;

    if (!str) {
        return MCS_IMPORT_FILE_ERROR;
    }
    if (!read_upper_line(line, MAX_LINE_LENGTH, str, &offset) ||
        strcmp(line, UPPER_MCS_SIGNATURE)) {
        free(str);
        return MCS_IMPORT_ERROR;
    }

    /* Valid signature, save the code offset */
    code_offset = offset;
    /* Check size */
    *code_size = 0;
    while (read_upper_line(line, MAX_LINE_LENGTH, str, &offset)) {
        (*code_size)++;
    }
    if (*code_size < 1) {
        free(str);
        return MCS_IMPORT_ERROR;
    }
    /* Read code */
    *code = malloc(sizeof(**code) * *code_size);
    if (!code) {
        free(str);
        return MCS_IMPORT_ERROR;
    }
    for (i = 0; read_upper_line(line, MAX_LINE_LENGTH, str, &code_offset); i++) {
        lineptr = line;
        (*code)[i] = (unsigned short) retrieve_value_hexadecimal(&lineptr, &status, CODE_MAX_VALUE);
        if (status != RETRIEVE_VALUE_OK) {
            free(*code);
            free(str);
            *code = 0;
            return MCS_IMPORT_ERROR;
        }
    }

    return MCS_IMPORT_SUCCESS;
}
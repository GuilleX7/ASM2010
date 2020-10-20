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

static char const *MCS_SIGNATURE = "v2.0 raw";

int mcs_export_file(char const *filepath, unsigned short *code, size_t code_size) {
    FILE *file = fopen(filepath, "w");
    int status = MCS_EXPORT_FILE_ERROR;
    if (!file) {
        return status;
    }

    fprintf(file, "%s\n", MCS_SIGNATURE);
    for (size_t i = 0; i < code_size; i++) {
        fprintf(file, "%x\n", code[i] & CODE_MAX_VALUE);
    }

    status = (ferror(file)) ? MCS_EXPORT_ERROR : MCS_EXPORT_SUCCESS;
    fclose(file);
    return status;
}

int mcs_import_file(char const *filepath, unsigned short **code, size_t *code_size) {
    FILE *file = { 0 };
    long int file_marker = 0;
    int status = 0;
    char buffer[16];
    char *buffer_ptr = { 0 };

    file = fopen(filepath, "r");
    if (!file) {
        return MCS_IMPORT_FILE_ERROR;
    }

    /* 16 bytes should be enough, 2^32 can be codificated with
        8 characters plus the new line and the null character */
    if (!ufgets(buffer, 16, file) || strncmp(MCS_SIGNATURE, buffer, strlen(MCS_SIGNATURE))) {
        fclose(file);
        return MCS_IMPORT_ERROR;
    }

    /* Valid MCS file */
    /* Get line count; 1 line = 1 sentence */
    *code_size = 0;
    file_marker = ftell(file);
    while (ufgets(buffer, 16, file)) {
        (*code_size)++;
    }
    if (*code_size < 1) {
        fclose(file);
        return MCS_IMPORT_ERROR;
    }

    *code = malloc(sizeof **code * *code_size);
    if (!*code) {
        fclose(file);
        return MCS_IMPORT_ERROR;
    }
    fseek(file, file_marker, SEEK_SET);
    for (size_t i = 0; i < *code_size; i++) {
        ufgets(buffer, 16, file);
        buffer_ptr = buffer;
        (*code)[i] = retrieve_value_hexadecimal((const char **) &buffer_ptr, &status, CODE_MAX_VALUE) & CODE_MAX_VALUE;
        if (status != RETRIEVE_VALUE_OK) {
            fclose(file);
            free(*code);
            return MCS_IMPORT_ERROR;
        }
    }
    fclose(file);
    return MCS_IMPORT_SUCCESS;
}
/** @file mcs.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parse.h"
#include "mcs.h"

#define CODE_ALIGNMENT (sizeof(uint16_t))

static char const MCS_BIN_SIGNATURE[] = { 'C', 'S', '2', '0', '1', '0' };
static char const *MCS_HEX_SIGNATURE = "v2.0 raw\n";

int mcs_export_file(char const *filepath, uint16_t *code, size_t code_size, int mcs_format) {
    FILE *file = fopen(filepath, (mcs_format == MCS_FORMAT_BIN) ? "wb" : "w");
    if (!file) {
        return MCS_EXPORT_FILE_ERROR;
    }

    if (mcs_format == MCS_FORMAT_BIN) {
        fwrite(MCS_BIN_SIGNATURE, 1, sizeof(MCS_BIN_SIGNATURE) / sizeof(MCS_BIN_SIGNATURE[0]), file);
        for (size_t i = 0; i < code_size; i++) {
            fwrite(&code[i], sizeof code[i], 1, file);
        }
    } else {
        fputs(MCS_HEX_SIGNATURE, file);
        for (size_t i = 0; i < code_size; i++) {
            fprintf(file, "%x\n", code[i]);
        }
    }

    if (ferror(file)) {
        fclose(file);
        return MCS_EXPORT_ERROR;
    }

    fclose(file);
    return MCS_EXPORT_SUCCESS;
}

int mcs_import_file(char const *filepath, uint16_t **code, size_t *code_size, int *mcs_format) {
    FILE *file = fopen(filepath, "rb");
    long int file_marker = 0;
    int status = 0;
    char buffer[16];
    char *buffer_ptr = { 0 };

    if (!file) {
        return MCS_IMPORT_FILE_ERROR;
    }

    if (fread(buffer, 1, sizeof MCS_BIN_SIGNATURE, file) != sizeof MCS_BIN_SIGNATURE) {
        fclose(file);
        return MCS_IMPORT_FILE_ERROR;
    }

    if (!memcmp(MCS_BIN_SIGNATURE, buffer, sizeof MCS_BIN_SIGNATURE)) {
        /* Bin MCS file */
        fseek(file, 0, SEEK_END);
        /* Check code size */
        *code_size = ftell(file) - sizeof MCS_BIN_SIGNATURE;
        if (*code_size % 2 || *code_size < CODE_ALIGNMENT) {
            /* We expect the code to be 16 bits wide... */
            fclose(file);
            return MCS_IMPORT_ERROR;
        }
        fseek(file, sizeof MCS_BIN_SIGNATURE, SEEK_SET);
        *code = malloc(*code_size);
        if (!*code) {
            fclose(file);
            return MCS_IMPORT_FILE_ERROR;
        }
        if (fread(*code, 1, *code_size, file) != *code_size || ferror(file)) {
            fclose(file);
            free(*code);
            return MCS_IMPORT_FILE_ERROR;
        }
        *mcs_format = MCS_FORMAT_BIN;
        *code_size /= CODE_ALIGNMENT;
        return MCS_IMPORT_SUCCESS;
    }

    /* Another attempt */
    file = freopen(filepath, "r", file);
    if (!file) {
        return MCS_IMPORT_FILE_ERROR;
    }

    /* 16 bytes should be enough, 2^16 can be codificated with
        4 characters plus the new line */
    if (!fgets(buffer, 16, file)) {
        fclose(file);
        return MCS_IMPORT_ERROR;
    }

    if (!strcmp(MCS_HEX_SIGNATURE, buffer)) {
        /* Hex MCS file */
        *code_size = 0;
        file_marker = ftell(file);
        /* Get line count; 1 line = 1 sentence */
        while (fgets(buffer, 16, file)) {
            (*code_size)++;
        }
        if (*code_size < 1) {
            fclose(file);
            return MCS_IMPORT_ERROR;
        }

        fseek(file, file_marker, SEEK_SET);
        *code = malloc(CODE_ALIGNMENT * *code_size);
        if (!*code) {
            fclose(file);
            return MCS_IMPORT_FILE_ERROR;
        }
        for (size_t i = 0; fgets(buffer, 16, file); i++) {
            buffer_ptr = buffer;
            (*code)[i] = (uint16_t) retrieve_value_hexadecimal((const char**) &buffer_ptr, &status, UINT16_MAX);
            if (status != RETRIEVE_VALUE_OK) {
                fclose(file);
                free(*code);
                return MCS_IMPORT_ERROR;
            }
        }
        fclose(file);
        return MCS_IMPORT_SUCCESS;
    }

    fclose(file);
    return MCS_IMPORT_ERROR;
}
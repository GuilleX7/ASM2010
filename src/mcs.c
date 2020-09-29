/** @file mcs.c */

#include "mcs.h"

static char const MCS_BIN_SIGNATURE[] = { 'C', 'S', '2', '0', '1', '0' };
static char const *MCS_HEX_SIGNATURE = "v2.0 raw\n";

int mcs_export_file(char const *file_path, uint16_t *code, size_t code_size, int mcs_format) {
    FILE *file = fopen(file_path, (mcs_format == MCS_FORMAT_BIN) ? "wb" : "w");
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

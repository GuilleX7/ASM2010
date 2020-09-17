#include "file.h"

static char const cs2010_bin_signature[] = { 'C', 'S', '2', '0', '1', '0' };

const char *get_file_name(const char *file_path) {
    if (!file_path || !*file_path) return 0;
    size_t offset = strlen(file_path) - 1;
    while (offset) {
        if (file_path[offset] == '\\' || file_path[offset] == '/') {
            offset++;
            break;
        }
        offset--;
    }
    return file_path + offset;
}

char *change_path_extension(const char *file_path, const char *new_extension) {
    char *result = { 0 };
    if (!file_path || !*file_path) return 0;
    char *dot_ptr = strrchr(file_path, (unsigned char) '.');
    if (dot_ptr) {
        result = malloc(dot_ptr - file_path + strlen(new_extension) + 1);
        if (result) {
            strncpy(result, file_path, dot_ptr - file_path);
            result[dot_ptr - file_path] = '\0';
            strcat(result, new_extension);
        }
    } else {
        result = malloc(strlen(file_path) + strlen(new_extension) + 1);
        if (result) {
            strcpy(result, file_path);
            strcat(result, new_extension);
        }
    }
    return result;
}

int export_code_to_file(const char *file_path, uint16_t *code, size_t code_size, int export_format) {
    FILE *fp = fopen(file_path, (export_format == EXPORT_FORMAT_BIN) ? "wb" : "w");
    if (!fp) {
        return EXPORT_FILE_ERROR;
    }

    if (export_format == EXPORT_FORMAT_BIN) {
        fwrite(cs2010_bin_signature, 1, sizeof(cs2010_bin_signature) / sizeof(cs2010_bin_signature[0]), fp);
        for (size_t i = 0; i < code_size; i++) {
            fwrite(&code[i], sizeof code[i], 1, fp);
        }
    } else {
        fprintf(fp, "v2.0 raw\n");
        for (size_t i = 0; i < code_size; i++) {
            fprintf(fp, "%x\n", code[i]);
        }
    }

    if (ferror(fp)) {
        fclose(fp);
        return EXPORT_ERROR;
    }

    fclose(fp);
    return EXPORT_SUCCESS;
}
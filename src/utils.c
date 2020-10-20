/** @file utils.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

bool save_file(char const *filepath, char const *str) {
    FILE *file;
    bool status = false;

    if (!filepath) {
        return status;
    }
    file = fopen(filepath, "wb");
    if (!file) {
        return status;
    }
    fwrite(str, 1, strlen(str), file);
    if (!ferror(file)) {
        status = true;
    }
    fclose(file);
    return status;
}

char *read_file(char const *filepath) {
    size_t size;
    char *str;
    size_t i;
    FILE *file;

    if (!filepath) {
        return 0;
    }
    file = fopen(filepath, "rb");
    if (!file) {
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    str = malloc(size + 1);
    if (!str) {
        fclose(file);
        return 0;
    }

    if (fread(str, 1, size, file) != size || ferror(file)) {
        fclose(file);
        free(str);
        return 0;
    }
    str[size] = 0;

    /* Check for UTF-8 BOM and fix the string */
    if (size > 3 && (unsigned char) str[0] == 0xEFU && (unsigned char) str[1] == 0xBBU && (unsigned char) str[2] == 0xBFU) {
        for (i = 3; str[i] != '\0'; i++) {
            str[i - 3] = str[i];
        }
        str[i - 3] = '\0';
    }

    fclose(file);
    return str;
}

char const *get_file_name(char const *filepath) {
    size_t offset;

    if (!filepath || !*filepath) {
        return 0;
    }
    offset = strlen(filepath) - 1;
    while (offset) {
        if (filepath[offset] == '\\' || filepath[offset] == '/') {
            offset++;
            break;
        }
        offset--;
    }
    return filepath + offset;
}

char *change_path_extension(char const *file_path, char const *new_extension) {
    char *result = { 0 };
    char *dot_ptr = { 0 };

    if (!file_path || !*file_path || !new_extension || !*new_extension) {
        return result;
    }
    dot_ptr = strrchr(file_path, (unsigned char) '.');
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

bool read_upper_line(char *line, size_t max_length, char const *const str, size_t *offset) {
    size_t i = 0;
    while (str[*offset] != '\0' && str[*offset] != '\n' && i < max_length - 1) {
        line[i] = toupper((unsigned) str[*offset]);
        (*offset)++;
        i++;
    }
    line[i] = '\0';
    if (str[*offset] == '\n') (*offset)++;
    if (str[*offset] == '\0' && !i) return false;
    return true;
}

char *allocstrcpy(char const *const src) {
    char *dst = { 0 };
    size_t src_len = 0;
    if (!src) {
        return dst;
    }
    src_len = strlen(src);
    dst = malloc(src_len + 1);
    if (!dst) {
        return dst;
    }
    strncpy(dst, src, src_len);
    dst[src_len] = 0;
    return dst;
}

char *ufgets(char *s, int n, FILE *stream) {
    char *r = fgets(s, n, stream);
    if (r) {
        s[strcspn(s, "\n\r")] = '\0';
    }
    return r;
}
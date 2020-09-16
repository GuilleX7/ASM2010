#ifndef EXPORT_H
#define EXPORT_H

#define EXPORT_SUCCESS 0
#define EXPORT_FILE_ERROR 1
#define EXPORT_ERROR 2

#define EXPORT_FORMAT_BIN 0
#define EXPORT_FORMAT_HEX 1

#include "parse.h"

const char *get_file_name(const char *file_path);
char *change_file_extension(const char *file_name, const char *new_extension);
int export_pinfo_file(const char *file_path, parse_info *pinfo, int export_format);

#endif /* EXPORT_H */
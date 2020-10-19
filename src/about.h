/** @file about.h */

#ifndef ABOUT_H
#define ABOUT_H

#include "utils.h"

#define ASM2010_AUTHOR "GuilleX7"
#define ASM2010_EMAIL "guillermox7@gmail.com"
#define ASM2010_URL "https://github.com/GuilleX7/ASM2010"
#define ASM2010_MAJOR_VERSION 0
#define ASM2010_MINOR_VERSION 3
#define ASM2010_PATCH_VERSION 2

#define MAKE_ABOUT_TEXT(NAME, DESCRIPTION) \
NAME " v"\
STRINGIFY(ASM2010_MAJOR_VERSION) "."\
STRINGIFY(ASM2010_MINOR_VERSION) "."\
STRINGIFY(ASM2010_PATCH_VERSION)\
" - " DESCRIPTION "\n"\
"Developed by " ASM2010_AUTHOR " - " ASM2010_EMAIL"\n"\
ASM2010_URL

#endif /* ABOUT_H */
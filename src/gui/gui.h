#ifndef GUI_H
#define GUI_H

#ifdef _MSC_VER
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#define PROGRAM_TITLE "ASM2010"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>

#include "../export.h"
#include "../parse.h"

#include "./iup/iup.h"

#endif /* GUI_H */
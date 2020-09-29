/** @file m2010.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdio.h>
#include <locale.h>

#include <iup/iup.h>

int main(int argc, char **argv) {
    Ihandle *main_window;
    
    if (IupOpen(&argc, &argv) == IUP_ERROR) {
        puts("Error while opening GUI, aborting execution...\n");
    }
    IupSetGlobal("UTF8MODE", "YES");
    setlocale(LC_ALL, "");

    main_window = IupDialog(NULL);

    IupShowXY(main_window, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
}

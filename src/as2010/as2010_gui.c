#ifdef _MSC_VER
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#define PROGRAM_TITLE "AS2010"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>

#include <iup/iup.h>

#include "../file.h"

#include "as2010_parse.h"

/* Global variables */
static char *actual_source_file = { 0 };
static char *actual_export_file = { 0 };
static int actual_export_format = 0;
static bool dirty = false;

/* Callbacks */
static int on_exit_cb(Ihandle *self);
static int new_item_cb(Ihandle *self);
static int open_item_cb(Ihandle *self);
static int saveas_item_cb(Ihandle *self);
static int save_item_cb(Ihandle *self);
static int assemble_item_cb(Ihandle *self);
static int exportas_item_cb(Ihandle *self);
static int export_item_cb(Ihandle *self);
static int source_multitext_valuechanged_cb(Ihandle *self);

/**
 * @brief Reads the entire content of a file
 * @param filename Path to the file
 * @return Dynamically allocated array of char containing the
 *          entire text
*/
static char *read_file(const char *filename) {
    size_t size;
    char *str;
    int i;
    FILE *file = fopen(filename, "rb");
    if (!file) {
        IupMessagef("Error", "Couldn't open file: %s", filename);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    str = malloc(size + 1);
    if (!str) {
        IupMessagef("Error", "Failed when reading from file: %s", filename);
        fclose(file);
        return 0;
    }

    if (fread(str, 1, size, file) != size) {
        if (ferror(file)) {
            IupMessagef("Error", "Failed when reading from file: %s", filename);
        }
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

/**
 * @brief Saves the given text to a file 
 * @param filename Path to the file
 * @param str Pointer to text to be saved
 * @return true if file was successfully saves,
 *          else otherwise
*/
static bool save_file(char *filename, char *str) {
    FILE *file = fopen(filename, "wb");
    bool status = true;
    if (!file) {
        IupMessagef("Error", "Couldn't open file: %s", filename);
        return false;
    }

    fwrite(str, 1, strlen(str), file);

    if (ferror(file)) {
        IupMessagef("Error", "Failed when saving file: %s", filename);
        status = false;
    }

    fclose(file);
    return status;
}

/**
 * @brief Marks a given source file as opened.
 *          User won't be asked anymore until
 *          clear_actual is called
 * @param window Pointer to the window handler
 *              whose title will be changed
 * @param file Path to the file
*/
static void mark_actual_source(Ihandle *window, char *file) {
    if (actual_source_file) free(actual_source_file);
    actual_source_file = malloc(strlen(file) + 1);
    if (!actual_source_file) return;
    strcpy(actual_source_file, file);

    IupSetStrf(window, "TITLE", "%s - %s", get_file_name(file), PROGRAM_TITLE);
}

/**
 * @brief Marks a given export file as opened with
 *          a given exporting format. User won't
 *          be asked anymore until clear_actual is
 *          called
 * @param file Path to the file
 * @param export_format Exporting format for the file
*/
static void mark_actual_export(char *file, int export_format) {
    if (actual_export_file) free(actual_export_file);
    actual_export_file = malloc(strlen(file) + 1);
    if (!actual_export_file) return;
    strcpy(actual_export_file, file);

    actual_export_format = export_format;
}

/**
 * @brief Forgets about source and export files.
 *        Next time user will be asked again for
 *        those files
 * @param window 
*/
static void clear_actual(Ihandle *window) {
    if (actual_source_file) free(actual_source_file);
    actual_source_file = 0;
    if (actual_export_file) free(actual_export_file);
    actual_export_file = 0;
    actual_export_format = 0;

    IupSetAttribute(window, "TITLE", PROGRAM_TITLE);
}

/**
 * @brief Traces the given string at the given multitext
 *          element
 * @param multitext Pointer to the multitext handler 
 * @param str String to be traced
*/
static void trace(Ihandle *multitext, char *str) {
    char *new_str = malloc(IupGetInt(multitext, "COUNT") + strlen(str) + 1);
    if (!new_str) {
        IupMessage("Error", "Memory exhaustion detected: unable to operate");
        return;
    }
    sprintf(new_str, "%s%s", IupGetAttribute(multitext, "VALUE"), str);
    IupSetStrAttribute(multitext, "VALUE", new_str);
    free(new_str);
}

/**
 * @brief Reads a line of a given string in uppercase
 * @param line Pointer to the buffer used to store the line
 * @param max_length Maximum length of the line to be buffered
 * @param str Pointer to the string to be readed from
 * @param offset Should be a pointer to 0-integer first time this
 *              function is called. Successive calls needs the
 *              same integer object to be passed in order to keep
 *              track of the position
 * @return false if there are no more lines left,
 *         true otherwise
*/
static bool read_upper_line(char *line, size_t max_length, char const *const str, size_t *offset) {
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

/**
 * @brief Parses the source of a given window and logs the
 *        assembler result
 * @param window Pointer to the window handler
 * @param pinfo Pointer to the parse_info structure
 * @return PARSE_OK if assembly was successful,
 *          PARSE_ERROR otherwise
*/
static parse_status parse_source(Ihandle *window, parse_info *pinfo) {
    Ihandle *source_multitext = IupGetDialogChild(window, "SOURCE_MULTITEXT");
    Ihandle *status_multitext = IupGetChild(IupGetDialogChild(window, "STATUS_VBOX"), 0);
    IupSetAttribute(status_multitext, "VALUE", "");
    char *str = IupGetAttribute(source_multitext, "VALUE");
    char buf[MAX_LINE_LENGTH + 2];
    size_t offset = 0;

    parse_status status;
    while (read_upper_line(buf, MAX_LINE_LENGTH + 2, str, &offset)) {
        status = parse_line(pinfo, buf);
        trace(status_multitext, pinfo->trace_buf);
        if (status == PARSE_ERROR) {
            trace(status_multitext, "Aborting assembly...\n");
            return PARSE_ERROR;
        }
    }

    status = parse_assemble(pinfo);
    trace(status_multitext, pinfo->trace_buf);
    if (status == PARSE_ERROR) {
        trace(status_multitext, "Aborting assembly...\n");
        return PARSE_ERROR;
    }

    trace(status_multitext, "Successfully assembled!\n");
    return PARSE_OK;
}

/**
 * @brief Assembles source and exports it to a file
 * @param window Pointer to the window handler
*/
static void export_file(Ihandle *window) {
    Ihandle *status_multitext = IupGetChild(IupGetDialogChild(window, "STATUS_VBOX"), 0);
    parse_info pinfo = { 0 };
    parse_init(&pinfo);
    if (parse_source(window, &pinfo) == PARSE_OK) {
        switch (export_code_to_file(actual_export_file, pinfo.bincode, pinfo.sentence_index, actual_export_format)) {
        case EXPORT_FILE_ERROR:
            IupMessagef("Error", "Couldn't open file '%s'\n", actual_export_file);
            break;
        case EXPORT_ERROR:
            IupMessagef("Error", "Couldn't write file '%s'\n", actual_export_file);
            break;
        default:
            trace(status_multitext, "Successfully exported file!\n");
            break;
        }
    }
    parse_free(&pinfo);
    return;
}

/**
 * @brief Checks whether source has been saved before
 *        exiting
 * @param window Pointer to window handler
 * @return
*/
static bool save_check(Ihandle *window) {
    if (dirty) {
        switch (IupAlarm("Warning", "Source changes haven't been saved. Save it now?", "Save changes", "Ignore changes", "Cancel")) {
        case 1:  /* Save the changes and continue */
            save_item_cb(window);
            break;
        case 2:  /* Ignore the changes and continue */
            break;
        case 3:  /* Cancel */
            return false;
        }
    }
    return true;
}

static int on_exit_cb(Ihandle *self) {
    if (!save_check(self)) {
        return IUP_IGNORE;
    }
    ins_search_stop();
    return IUP_CLOSE;
}

static int new_item_cb(Ihandle *self) {
    Ihandle *main_window = IupGetDialog(self);
    Ihandle *source_multitext = IupGetDialogChild(main_window, "SOURCE_MULTITEXT");
    IupSetAttribute(source_multitext, "VALUE", "");
    clear_actual(main_window);
    dirty = false;
    return IUP_IGNORE;
}

static int open_item_cb(Ihandle *self) {
    Ihandle *main_window = IupGetDialog(self);
    Ihandle *source_multitext = IupGetDialogChild(main_window, "SOURCE_MULTITEXT");
    Ihandle *dlg = IupFileDlg();
    char *str = { 0 };
    char *file = { 0 };

    IupSetAttribute(dlg, "DIALOGTYPE", "OPEN");
    IupSetAttribute(dlg, "TITLE", "Open a file");
    IupSetAttribute(dlg, "EXTFILTER", "Assembly files|*.asm|All files|*.*|");
    IupSetAttributeHandle(dlg, "PARENTDIALOG", main_window);

    IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);

    if (IupGetInt(dlg, "STATUS") != -1) {
        file = IupGetAttribute(dlg, "VALUE");

        str = read_file(file);
        if (str) {
            IupSetAttribute(source_multitext, "VALUE", str);
            clear_actual(main_window);
            mark_actual_source(main_window, file);
            dirty = false;
            free(str);
        }
    }

    IupDestroy(dlg);
    return IUP_IGNORE;
}

static int saveas_item_cb(Ihandle *self) {
    Ihandle *main_window = IupGetDialog(self);
    Ihandle *dlg = IupFileDlg();
    char *file = { 0 };

    IupSetAttribute(dlg, "DIALOGTYPE", "SAVE");
    IupSetAttribute(dlg, "EXTFILTER", "Assembly files|*.asm|All files|*.*|");
    IupSetAttribute(dlg, "EXTDEFAULT", "asm");
    IupSetAttributeHandle(dlg, "PARENTDIALOG", main_window);
    if (actual_source_file)
        IupSetStrAttribute(dlg, "FILE", actual_source_file);

    IupPopup(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

    if (IupGetInt(dlg, "STATUS") != -1) {
        file = IupGetAttribute(dlg, "VALUE");

        if (save_file(file, IupGetAttribute(IupGetDialogChild(main_window, "SOURCE_MULTITEXT"), "VALUE"))) {
            dirty = false;
            mark_actual_source(main_window, file);
        }
    }

    IupDestroy(dlg);
    return IUP_DEFAULT;
}

static int save_item_cb(Ihandle *self) {
    Ihandle *main_window = IupGetDialog(self);
    if (actual_source_file) {
        save_file(actual_source_file, IupGetAttribute(IupGetDialogChild(main_window, "SOURCE_MULTITEXT"), "VALUE"));
        IupSetStrf(main_window, "TITLE", "%s - %s", get_file_name(actual_source_file), PROGRAM_TITLE);
        dirty = false;
    } else {
        saveas_item_cb(self);
    }
    return IUP_IGNORE;
}

static int assemble_item_cb(Ihandle *self) {
    parse_info pinfo = { 0 };
    parse_init(&pinfo);
    parse_source(IupGetDialog(self), &pinfo);
    parse_free(&pinfo);
    return IUP_IGNORE;
}

static int exportas_item_cb(Ihandle *self) {
    Ihandle *main_window = IupGetDialog(self);
    Ihandle *status_multitext = IupGetChild(IupGetDialogChild(main_window, "STATUS_VBOX"), 0);
    Ihandle *dlg;
    char *file = { 0 };
    int option = 0;
    char *possible_file = { 0 };

    if (!IupGetParam("Select exporting format...", 0, 0, "%o|Binary|Hexadecimal|\n", &option)) {
        trace(status_multitext, "Operation cancelled\n");
        return IUP_DEFAULT;
    }

    if (option == 0) {
        option = EXPORT_FORMAT_BIN;
    } else {
        option = EXPORT_FORMAT_HEX;
    }

    dlg = IupFileDlg();
    IupSetAttribute(dlg, "DIALOGTYPE", "SAVE");
    IupSetAttributeHandle(dlg, "PARENTDIALOG", main_window);
    if (option == EXPORT_FORMAT_BIN) {
        IupSetAttribute(dlg, "EXTFILTER", "Binary files|*.bin|");
        IupSetAttribute(dlg, "EXTDEFAULT", "bin");
    } else {
        IupSetAttribute(dlg, "EXTFILTER", "Hexadecimal files|*.hex|");
        IupSetAttribute(dlg, "EXTDEFAULT", "hex");
    }
    if (actual_export_file && option == actual_export_format) {
        IupSetAttribute(dlg, "FILE", actual_export_file);
    } else if (actual_source_file) {
        possible_file = change_path_extension(actual_source_file, (option == EXPORT_FORMAT_BIN) ? ".bin" : ".hex");
        if (possible_file) {
            IupSetStrAttribute(dlg, "FILE", possible_file);
            free(possible_file);
        }
    }

    IupPopup(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

    if (IupGetInt(dlg, "STATUS") != -1) {
        file = IupGetAttribute(dlg, "VALUE");
        mark_actual_export(file, option);
        export_file(main_window);
    } else {
        trace(status_multitext, "Operation cancelled\n");
    }

    IupDestroy(dlg);

    return IUP_DEFAULT;
}

static int export_item_cb(Ihandle *self) {
    Ihandle *window = IupGetDialog(self);
    if (actual_export_file) {
        export_file(IupGetDialog(window));
    } else {
        exportas_item_cb(self);
    }
    return IUP_DEFAULT;
}

static int source_multitext_valuechanged_cb(Ihandle *self) {
    if (!dirty && actual_source_file) {
        IupSetStrf(IupGetDialog(self), "TITLE", "%s* - %s", get_file_name(actual_source_file), PROGRAM_TITLE);
    }
    dirty = true;
    return IUP_DEFAULT;
}

int main(int argc, char **argv) {
    /* Main window */
    Ihandle *main_window, *main_vbox;
    /* Menu */
    Ihandle *menu;
    /* File menu */
    Ihandle *file_menu, *file_submenu, *new_item, *open_item, *save_item, *saveas_item, *exit_item;
    /* Assembler menu */
    Ihandle *assembler_menu, *assembler_submenu, *assemble_item, *export_item, *exportas_item;
    /* Source tab */
    Ihandle *source_multitext, *source_vbox, *status_multitext, *status_vbox, *split;
    /* Tabs */
    Ihandle *tabs;

    if (IupOpen(&argc, &argv) == IUP_ERROR) {
        puts("Error while opening GUI, aborting execution...\n");
    }
    IupSetGlobal("UTF8MODE", "YES");
    setlocale(LC_ALL, "");

    ins_search_start();

    new_item = IupItem("New\tCTRL+N", NULL);
    IupSetAttribute(new_item, "NAME", "NEW_ITEM");
    IupSetCallback(new_item, "ACTION", (Icallback) new_item_cb);
    open_item = IupItem("Open\tCTRL+O", NULL);
    IupSetAttribute(open_item, "NAME", "OPEN_ITEM");
    IupSetCallback(open_item, "ACTION", (Icallback) open_item_cb);
    save_item = IupItem("Save\tCTRL+S", NULL);
    IupSetAttribute(save_item, "NAME", "SAVE_ITEM");
    IupSetCallback(save_item, "ACTION", (Icallback) save_item_cb);
    saveas_item = IupItem("Save as...", NULL);
    IupSetAttribute(saveas_item, "NAME", "SAVEAS_ITEM");
    IupSetCallback(saveas_item, "ACTION", (Icallback) saveas_item_cb);
    exit_item = IupItem("Exit\tCTRL+Q", NULL);
    IupSetAttribute(exit_item, "NAME", "EXIT_ITEM");
    IupSetCallback(exit_item, "ACTION", (Icallback) on_exit_cb);
    file_menu = IupMenu(
        new_item,
        open_item,
        save_item,
        saveas_item,
        IupSeparator(),
        exit_item,
        NULL
    );
    file_submenu = IupSubmenu("File", file_menu);
    IupSetAttribute(file_submenu, "NAME", "FILE_SUBMENU");

    assemble_item = IupItem("Assemble source\tCTRL+I", NULL);
    IupSetAttribute(assemble_item, "NAME", "ASSEMBLE_ITEM");
    IupSetCallback(assemble_item, "ACTION", (Icallback) assemble_item_cb);
    export_item = IupItem("Assemble source and export", NULL);
    IupSetAttribute(export_item, "NAME", "EXPORT_ITEM");
    IupSetCallback(export_item, "ACTION", (Icallback) export_item_cb);
    exportas_item = IupItem("Assemble source and export as...", NULL);
    IupSetAttribute(exportas_item, "NAME", "EXPORTAS_ITEM");
    IupSetCallback(exportas_item, "ACTION", (Icallback) exportas_item_cb);
    assembler_menu = IupMenu(
        assemble_item,
        export_item,
        exportas_item,
        NULL
    );
    IupSetAttribute(assembler_menu, "NAME", "ASSEMBLER_MENU");
    assembler_submenu = IupSubmenu("Assembler", assembler_menu);
    IupSetAttribute(assembler_submenu, "NAME", "ASSEMBLER_SUBMENU");

    menu = IupMenu(
        file_submenu,
        assembler_submenu,
        NULL
    );
    IupSetAttribute(menu, "NAME", "MENU");

    source_multitext = IupText(NULL);
    IupSetAttribute(source_multitext, "NAME", "SOURCE_MULTITEXT");
    IupSetAttribute(source_multitext, "FONT", "Courier 11");
    IupSetAttribute(source_multitext, "MULTILINE", "YES");
    IupSetInt(source_multitext, "TABSIZE", 4);
    IupSetAttribute(source_multitext, "EXPAND", "YES");
    IupSetCallback(source_multitext, "VALUECHANGED_CB", (Icallback) source_multitext_valuechanged_cb);

    source_vbox = IupVbox(
        source_multitext,
        NULL
    );
    IupSetAttribute(source_vbox, "NAME", "SOURCE_VBOX");
    IupSetInt(source_vbox, "GAP", 4);

    status_multitext = IupText(NULL);
    IupSetAttribute(status_multitext, "NAME", "STATUS_MULTITEXT");
    IupSetAttribute(status_multitext, "MULTILINE", "YES");
    IupSetAttribute(status_multitext, "EXPAND", "YES");
    IupSetAttribute(status_multitext, "READONLY", "YES");

    status_vbox = IupVbox(
        status_multitext,
        NULL
    );
    IupSetAttribute(status_vbox, "NAME", "STATUS_VBOX");
    IupSetInt(status_vbox, "GAP", 4);

    tabs = IupTabs(
        status_vbox,
        NULL
    );
    IupSetAttribute(tabs, "NAME", "TABS");
    IupSetAttribute(tabs, "TABTITLE0", "Output");

    split = IupSplit(
        source_vbox,
        tabs
    );
    IupSetAttribute(split, "NAME", "SPLIT");
    IupSetAttribute(split, "ORIENTATION", "HORIZONTAL");
    IupSetInt(split, "VALUE", 700);
    IupSetAttribute(IupGetChild(split, 0), "STYLE", "EMPTY");

    main_vbox = IupVbox(
        split,
        NULL
    );
    IupSetAttribute(main_vbox, "NAME", "MAIN_VBOX");

    main_window = IupDialog(main_vbox);
    IupSetAttribute(main_window, "NAME", "MAIN_WINDOW");
    IupSetAttributeHandle(main_window, "MENU", menu);
    new_item_cb(main_window);
    IupSetAttribute(main_window, "SIZE", "QUARTERxHALF");

    IupSetCallback(main_window, "K_cN", (Icallback) new_item_cb);
    IupSetCallback(main_window, "K_cO", (Icallback) open_item_cb);
    IupSetCallback(main_window, "K_cS", (Icallback) save_item_cb);
    IupSetCallback(main_window, "K_cQ", (Icallback) on_exit_cb);
    IupSetCallback(main_window, "K_cI", (Icallback) assemble_item_cb);

    IupSetCallback(main_window, "CLOSE_CB", (Icallback) on_exit_cb);

    IupShowXY(main_window, IUP_CENTER, IUP_CENTER);
    IupSetAttribute(main_window, "SIZE", NULL);

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}
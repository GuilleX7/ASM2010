/** @file as_gui.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#include <iup/iup.h>

#include "../utils.h"
#include "../mcs.h"

#include "as_parse.h"

static char *actual_source_filepath = { 0 };
static char *actual_export_filepath = { 0 };
static int actual_export_format = 0;
static bool dirty = false;

/* They just needed declaration */
static int save_item_cb(Ihandle *self);
static void mark_actual_source(Ihandle *window, char const *filepath);
static void clear_actual(Ihandle *window);

/**
 * @brief Open and reads a file, marking it as source
 * @param window Pointer to window handler
 * @param filepath Path to the file
 * @return true if file was readed successfully, false otherwise
*/
static bool open_file(Ihandle *window, char const *filepath) {
	Ihandle *source_multitext = IupGetDialogChild(window, "SOURCE_MULTITEXT");
	char *str = read_file(filepath);
	if (str) {
		IupSetStrAttribute(source_multitext, "VALUE", str);
		clear_actual(window);
		mark_actual_source(window, filepath);
		dirty = false;
		free(str);
		return true;
	} else {
		IupMessagef("Error", "Failed when reading from file: %s", filepath);
	}
	return false;
}

/**
 * @brief Marks a given source file as opened.
 *          User won't be asked anymore until
 *          clear_actual is called
 * @param window Pointer to the window handler
 *              whose title will be changed
 * @param file Path to the file
*/
static void mark_actual_source(Ihandle *window, char const *filepath) {
	if (actual_source_filepath) free(actual_source_filepath);
	actual_source_filepath = malloc(strlen(filepath) + 1);
	if (!actual_source_filepath) return;
	strcpy(actual_source_filepath, filepath);

	IupSetStrf(window, "TITLE", "%s - %s", get_file_name(filepath), AS_PARSER_NAME);
}

/**
 * @brief Marks a given export file as opened with
 *          a given exporting format. User won't
 *          be asked anymore until clear_actual is
 *          called
 * @param file Path to the file
 * @param export_format Exporting format for the file
*/
static void mark_actual_export(char const *filepath, int export_format) {
	if (actual_export_filepath) free(actual_export_filepath);
	actual_export_filepath = malloc(strlen(filepath) + 1);
	if (!actual_export_filepath) return;
	strcpy(actual_export_filepath, filepath);

	actual_export_format = export_format;
}

/**
 * @brief Forgets about source and export files.
 *        Next time user will be asked again for
 *        those files
 * @param window
*/
static void clear_actual(Ihandle *window) {
	if (actual_source_filepath) free(actual_source_filepath);
	actual_source_filepath = 0;
	if (actual_export_filepath) free(actual_export_filepath);
	actual_export_filepath = 0;
	actual_export_format = 0;

	IupSetAttribute(window, "TITLE", AS_PARSER_NAME);
}

/**
 * @brief Checks whether source has been saved before
 *        exiting
 * @param window Pointer to window handler
 * @return true if user chooses to exit, false if user chooses to stay
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

/* Trace functions */

/**
 * @brief Traces the given string at the given multitext
 *          element
 * @param multitext Pointer to the multitext handler
 * @param str String to be traced
*/
static void trace(Ihandle *multitext, char const *str) {
	char *new_str = malloc(IupGetInt(multitext, "COUNT") + strlen(str) + 1);
	if (!new_str) {
		IupMessage("Error", "Memory exhaustion detected: unable to operate");
		return;
	}
	sprintf(new_str, "%s%s", IupGetAttribute(multitext, "VALUE"), str);
	IupSetStrAttribute(multitext, "VALUE", new_str);
	free(new_str);
}

/* Assembly functions */

/**
 * @brief Parses the source of a given window and logs the
 *        assembler result
 * @param window Pointer to the window handler
 * @param pinfo Pointer to the as_parse_info structure
 * @return PARSE_OK if assembly was successful,
 *          PARSE_ERROR otherwise
*/
static as_parse_status parse_source(Ihandle *window, as_parse_info *pinfo) {
	Ihandle *source_multitext = IupGetDialogChild(window, "SOURCE_MULTITEXT");
	Ihandle *status_multitext = IupGetChild(IupGetDialogChild(window, "STATUS_VBOX"), 0);
	IupSetAttribute(status_multitext, "VALUE", "");
	char *str = IupGetAttribute(source_multitext, "VALUE");
	char buf[AS_MAX_LINE_LENGTH + 2];
	size_t offset = 0;

	as_parse_status status;
	while (read_upper_line(buf, AS_MAX_LINE_LENGTH + 2, str, &offset)) {
		status = as_parse_line(pinfo, buf);
		trace(status_multitext, pinfo->trace_buffer);
		if (status == PARSE_ERROR) {
			trace(status_multitext, "Aborting assembly...\n");
			return PARSE_ERROR;
		}
	}

	status = as_parse_assemble(pinfo);
	trace(status_multitext, pinfo->trace_buffer);
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
	as_parse_info pinfo = { 0 };
	if (!as_parse_init(&pinfo)) {
		trace(status_multitext, "[Error] couldn't initialize assembly due to memory exhaustion\n");
		return;
	}

	if (parse_source(window, &pinfo) == PARSE_OK) {
		switch (mcs_export_file(actual_export_filepath, pinfo.machine_code, pinfo.sentence_index, actual_export_format)) {
		case MCS_EXPORT_FILE_ERROR:
			IupMessagef("Error", "Couldn't open file '%s'\n", actual_export_filepath);
			break;
		case MCS_EXPORT_ERROR:
			IupMessagef("Error", "Couldn't write file '%s'\n", actual_export_filepath);
			break;
		default:
			trace(status_multitext, "Successfully exported file!\n");
			break;
		}
	}
	as_parse_free(&pinfo);
	return;
}

static int on_exit_cb(Ihandle *self) {
	if (!save_check(self)) {
		return IUP_IGNORE;
	}
	return IUP_CLOSE;
}

static int new_item_cb(Ihandle *self) {
	Ihandle *main_window = IupGetDialog(self);
	Ihandle *source_multitext = IupGetDialogChild(main_window, "SOURCE_MULTITEXT");

	if (!save_check(main_window)) {
		return IUP_IGNORE;
	}

	IupSetAttribute(source_multitext, "VALUE", "");
	clear_actual(main_window);
	dirty = false;
	return IUP_IGNORE;
}

static int open_item_cb(Ihandle *self) {
	Ihandle *main_window = IupGetDialog(self);
	Ihandle *dlg;
	char *file = { 0 };

	if (!save_check(main_window)) {
		return IUP_IGNORE;
	}

	dlg = IupFileDlg();

	IupSetAttribute(dlg, "DIALOGTYPE", "OPEN");
	IupSetAttribute(dlg, "TITLE", "Open a file");
	IupSetAttribute(dlg, "EXTFILTER", "Assembly files|*.asm|All files|*.*|");
	IupSetAttributeHandle(dlg, "PARENTDIALOG", main_window);

	IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);

	if (IupGetInt(dlg, "STATUS") != -1) {
		file = IupGetAttribute(dlg, "VALUE");
		open_file(main_window, file);
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
	if (actual_source_filepath)
		IupSetStrAttribute(dlg, "FILE", actual_source_filepath);

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
	if (actual_source_filepath) {
		save_file(actual_source_filepath, IupGetAttribute(IupGetDialogChild(main_window, "SOURCE_MULTITEXT"), "VALUE"));
		IupSetStrf(main_window, "TITLE", "%s - %s", get_file_name(actual_source_filepath), AS_PARSER_NAME);
		dirty = false;
	} else {
		saveas_item_cb(self);
	}
	return IUP_IGNORE;
}

static int assemble_item_cb(Ihandle *self) {
	Ihandle *main_window = IupGetDialog(self);
	Ihandle *status_multitext = IupGetChild(IupGetDialogChild(main_window, "STATUS_VBOX"), 0);
	as_parse_info pinfo = { 0 };
	if (!as_parse_init(&pinfo)) {
		trace(status_multitext, "[Error] couldn't initialize assembly due to memory exhaustion\n");
		return IUP_IGNORE;
	}
	parse_source(IupGetDialog(self), &pinfo);
	as_parse_free(&pinfo);
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
		option = MCS_FORMAT_BIN;
	} else {
		option = MCS_FORMAT_HEX;
	}

	dlg = IupFileDlg();
	IupSetAttribute(dlg, "DIALOGTYPE", "SAVE");
	IupSetAttributeHandle(dlg, "PARENTDIALOG", main_window);
	if (option == MCS_FORMAT_BIN) {
		IupSetAttribute(dlg, "EXTFILTER", "Binary files|*.bin|");
		IupSetAttribute(dlg, "EXTDEFAULT", "bin");
	} else {
		IupSetAttribute(dlg, "EXTFILTER", "Hexadecimal files|*.hex|");
		IupSetAttribute(dlg, "EXTDEFAULT", "hex");
	}
	if (actual_export_filepath && option == actual_export_format) {
		IupSetAttribute(dlg, "FILE", actual_export_filepath);
	} else if (actual_source_filepath) {
		possible_file = change_path_extension(actual_source_filepath, (option == MCS_FORMAT_BIN) ? ".bin" : ".hex");
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
	if (actual_export_filepath) {
		export_file(IupGetDialog(window));
	} else {
		exportas_item_cb(self);
	}
	return IUP_DEFAULT;
}

static int source_multitext_valuechanged_cb(Ihandle *self) {
	if (!dirty && actual_source_filepath) {
		IupSetStrf(IupGetDialog(self), "TITLE", "%s* - %s", get_file_name(actual_source_filepath), AS_PARSER_NAME);
	}
	dirty = true;
	return IUP_DEFAULT;
}

static int dropfiles_cb(Ihandle *self, const char *filepath, int num, int x, int y) {
	Ihandle *window = IupGetDialog(self);
	if (num != 0) {
		return IUP_DEFAULT;
	}
	if (save_check(window)) {
		open_file(window, filepath);
	}
	return IUP_DEFAULT;
}

static int about_item_cb(Ihandle *self) {
	Ihandle *about_window = IupMessageDlg();
	IupSetAttribute(about_window, "BUTTONS", "OK");
	IupSetAttribute(about_window, "DIALOGTYPE", "INFORMATION");
	IupSetAttribute(about_window, "TITLE", "About");
	IupSetStrf(about_window, "VALUE",
		"AS2010-gui v" STRINGIFY(AS_PARSER_MAJOR_VERSION) "." STRINGIFY(AS_PARSER_MINOR_VERSION) "." STRINGIFY(AS_PARSER_PATCH_VERSION) " - CS2010 assembler\n"
		"Developed by GuilleX7 - guillermox7@gmail.com\n"
		"https://github.com/GuilleX7/\n");
	IupPopup(about_window, IUP_CENTER, IUP_CENTER);
	return IUP_DEFAULT;
}

/* Main function */

int main(int argc, char **argv) {
	/* Main window */
	Ihandle *main_window, *main_vbox;
	/* Menu */
	Ihandle *menu;
	/* File menu */
	Ihandle *file_menu, *file_submenu, *new_item, *open_item, *save_item, *saveas_item, *exit_item;
	/* Assembler menu */
	Ihandle *assembler_menu, *assembler_submenu, *assemble_item, *export_item, *exportas_item;
	/* About menu */
	Ihandle *about_menu, *about_submenu, *about_item;
	/* Source tab */
	Ihandle *source_multitext, *source_vbox, *status_multitext, *status_vbox, *split;
	/* Tabs */
	Ihandle *tabs;

	if (IupOpen(&argc, &argv) == IUP_ERROR) {
		puts("Error while opening GUI, aborting execution...\n");
		return EXIT_FAILURE;
	}
	IupSetGlobal("UTF8MODE", "YES");
	setlocale(LC_ALL, "");

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

	about_item = IupItem("About...", NULL);
	IupSetAttribute(about_item, "NAME", "ABOUT_ITEM");
	IupSetCallback(about_item, "ACTION", (Icallback) about_item_cb);
	about_menu = IupMenu(
		about_item,
		NULL
	);
	IupSetAttribute(about_menu, "NAME", "ABOUT_MENU");
	about_submenu = IupSubmenu("About", about_menu);
	IupSetAttribute(about_submenu, "NAME", "ABOUT_ITEM");

	menu = IupMenu(
		file_submenu,
		assembler_submenu,
		about_submenu,
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
	IupSetAttribute(main_window, "SIZE", "QUARTERxHALF");

	IupSetCallback(main_window, "K_cN", (Icallback) new_item_cb);
	IupSetCallback(main_window, "K_cO", (Icallback) open_item_cb);
	IupSetCallback(main_window, "K_cS", (Icallback) save_item_cb);
	IupSetCallback(main_window, "K_cQ", (Icallback) on_exit_cb);
	IupSetCallback(main_window, "K_cI", (Icallback) assemble_item_cb);
	IupSetCallback(main_window, "DROPFILES_CB", (Icallback) dropfiles_cb);

	IupSetCallback(main_window, "CLOSE_CB", (Icallback) on_exit_cb);

	new_item_cb(main_window);

	IupShowXY(main_window, IUP_CENTER, IUP_CENTER);
	IupSetAttribute(main_window, "SIZE", NULL);

	IupMainLoop();

	IupClose();
	return EXIT_SUCCESS;
}
/** @file m2010.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

#include <iup/iup.h>

#include "../about.h"
#include "../mcs.h"

#include "../as2010/as_parse.h"

#include "comp/m_romgrid.h"

#define PROGRAM_NAME "M2010"
#define PROGRAM_DESCRIPTION "CS2010 emulator"

m_comp_romgrid rom_gridbox;

bool open_machine_code_file(char *filepath) {
	uint16_t *code = { 0 };
	size_t code_size = 0;
	int mcs_format = 0;
	char *disassembly = { 0 };

	if (mcs_import_file(filepath, &code, &code_size, &mcs_format) == MCS_IMPORT_SUCCESS) {
		m_comp_romgrid_clear(&rom_gridbox);
		for (int i = 0; i < code_size; i++) {
			disassembly = as_disassemble_sentence(code[i]);
			if (!disassembly) {
				disassembly = "???";
				m_comp_romgrid_put_at(&rom_gridbox, i, code[i], disassembly);
			} else {
				m_comp_romgrid_put_at(&rom_gridbox, i, code[i], disassembly);
				free(disassembly);
			}
		}
		m_comp_romgrid_set_active(&rom_gridbox, 0);
		free(code);
		return true;
	}

	return false;
}

int open_machine_code_file_cb(Ihandle *self) {
	Ihandle *main_window = IupGetDialog(self);
	Ihandle *dlg = IupFileDlg();
	char *filepath = { 0 };

	IupSetAttribute(dlg, "DIALOGTYPE", "OPEN");
	IupSetAttribute(dlg, "TITLE", "Open a CS2010 machine code file");
	IupSetAttribute(dlg, "EXTFILTER", "CS2010 hexadecimal files|*.hex|CS2010 binary files|*.bin|All files|*.*|");
	IupSetAttributeHandle(dlg, "PARENTDIALOG", main_window);
	IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);
	if (IupGetInt(dlg, "STATUS") != -1) {
		filepath = IupGetAttribute(dlg, "VALUE");
		if (!open_machine_code_file(filepath)) {
			IupMessagef("Error", "Couldn't open file %s", filepath);
		}
	}
	IupDestroy(dlg);
	return IUP_IGNORE;
}

int save_dissasembled_code_file_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int exit_cb(Ihandle *self) {
	return IUP_CLOSE;
}

int set_machine_settings_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int step_instruction_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int step_microinstruction_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int start_clock_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int stop_clock_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int reset_all_simulation_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int about_cb(Ihandle *self) {
	Ihandle *about_window = IupMessageDlg();
	IupSetAttribute(about_window, "BUTTONS", "OK");
	IupSetAttribute(about_window, "DIALOGTYPE", "INFORMATION");
	IupSetAttribute(about_window, "TITLE", "About");
	IupSetStrf(about_window, "VALUE",
		MAKE_ABOUT_TEXT(
			PROGRAM_NAME,
			PROGRAM_DESCRIPTION
		)
	);
	IupPopup(about_window, IUP_CENTER, IUP_CENTER);
	return IUP_DEFAULT;
}

int main(int argc, char **argv) {
	/* Main window*/
	Ihandle *main_window, *main_vbox, *menu;
	/* File submenu */
	Ihandle *file_submenu, *open_machine_code_file_item, *save_dissasembled_code_file_item, *exit_item;
	/* Machine submenu */
	Ihandle *machine_submenu, *set_machine_settings_item;
	/* Simulation submenu */
	Ihandle *simulation_submenu, *step_instruction_item, *step_microinstruction_item,
		*start_clock_item, *stop_clock_item, *reset_all_simulation_item;
	/* About submenu */
	Ihandle *about_submenu, *about_item;
	/* ROM gridbox */
	Ihandle *rom_tabs;
	
	if (IupOpen(&argc, &argv) == IUP_ERROR) {
		puts("Error while opening GUI, aborting execution...\n");
		return EXIT_FAILURE;
	}
	IupSetGlobal("UTF8MODE", IUP_YES);
	setlocale(LC_ALL, "");

	open_machine_code_file_item = IupItem("Open machine code file\tCTRL+O", 0);
	IupSetCallback(open_machine_code_file_item, IUP_ACTION, (Icallback) open_machine_code_file_cb);
	save_dissasembled_code_file_item = IupItem("Save dissasembled code file\tCTRL+S", 0);
	IupSetCallback(save_dissasembled_code_file_item, IUP_ACTION, (Icallback) save_dissasembled_code_file_cb);
	exit_item = IupItem("Exit\tCTRL+Q", 0);
	IupSetCallback(exit_item, IUP_ACTION, (Icallback) exit_cb);
	file_submenu = IupSubmenu("File",
		IupMenu(
			open_machine_code_file_item,
			save_dissasembled_code_file_item,
			IupSeparator(),
			exit_item,
			0
		)
	);

	set_machine_settings_item = IupItem("Set machine settings", 0);
	IupSetCallback(set_machine_settings_item, IUP_ACTION, (Icallback) set_machine_settings_cb);
	machine_submenu = IupSubmenu("Machine",
		IupMenu(
			set_machine_settings_item,
			0
		)
	);

	step_instruction_item = IupItem("Step one instruction\tCTRL+I", 0);
	IupSetCallback(step_instruction_item, IUP_ACTION, (Icallback) step_instruction_cb);
	step_microinstruction_item = IupItem("Step one microinstruction\tCTRL+M", 0);
	IupSetCallback(step_microinstruction_item, IUP_ACTION, (Icallback) step_microinstruction_cb);
	start_clock_item = IupItem("Start clock", 0);
	IupSetCallback(start_clock_item, IUP_ACTION, (Icallback) start_clock_cb);
	stop_clock_item = IupItem("Stop clock", 0);
	IupSetCallback(stop_clock_item, IUP_ACTION, (Icallback) stop_clock_cb);
	reset_all_simulation_item = IupItem("Reset all simulation\tCTRL+R", 0);
	IupSetCallback(reset_all_simulation_item, IUP_ACTION, (Icallback) reset_all_simulation_cb);
	simulation_submenu = IupSubmenu("Simulation",
		IupMenu(
			step_instruction_item,
			step_microinstruction_item,
			IupSeparator(),
			start_clock_item,
			stop_clock_item,
			IupSeparator(),
			reset_all_simulation_item,
			0
		)
	);

	about_item = IupItem("About...", 0);
	IupSetCallback(about_item, IUP_ACTION, (Icallback) about_cb);
	about_submenu = IupSubmenu("About",
		IupMenu(
			about_item,
			0
		)
	);

	menu = IupMenu(
		file_submenu,
		machine_submenu,
		simulation_submenu,
		about_submenu,
		0
	);

	rom_gridbox = m_comp_romgrid_create();
	rom_tabs = IupTabs(
			m_comp_romgrid_get_handler(&rom_gridbox),
		0
	);
	IupSetAttribute(rom_tabs, "TABTITLE0", "ROM code");
	
	main_window = IupDialog(
		main_vbox = IupVbox(
			rom_tabs,
			0
		)
	);

	IupSetAttributeHandle(main_window, IUP_MENU, menu);
	IupSetAttribute(main_window, IUP_TITLE, PROGRAM_NAME);
	IupSetAttribute(main_window, IUP_SIZE, IUP_HALF "x" IUP_HALF);
	IupSetAttribute(main_window, IUP_SHRINK, IUP_YES);
	IupSetCallback(main_window, "K_cQ", (Icallback) exit_cb);
	IupSetCallback(main_window, IUP_CLOSE_CB, (Icallback) exit_cb);
	IupShowXY(main_window, IUP_CENTER, IUP_CENTER);
	IupMainLoop();
	IupClose();
	return EXIT_SUCCESS;
}
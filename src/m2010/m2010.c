/** @file m2010.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>

#include <iup/iup.h>

#include "../about.h"
#include "../mcs.h"

#include "../as2010/as_parse.h"

#include "comp/m_romgrid.h"
#include "comp/m_reggrid.h"
#include "comp/m_ramgrid.h"
#include "comp/m_siggrid.h"

#define PROGRAM_NAME "M2010"
#define PROGRAM_DESCRIPTION "CS2010 emulator"

m_comp_romgrid rom_gridbox;
m_comp_reggrid reg_gridbox;
m_comp_ramgrid ram_gridbox;
m_comp_siggrid sig_gridbox;

cs2010 *cs;
bool is_cs_ready;

int check_machine_running() {
	if (is_cs_ready) {
		switch (IupAlarm("Warning", "Current simulation will be wiped out. Do you want to continue?", "OK", "Cancel", 0)) {
		case 1:  /* Continue */
			break;
		case 2:  /* Cancel */
		default:
			return false;
			break;
		}
	}

	return true;
}

void update_machine_ui(cs2010 *cs) {
	m_comp_romgrid_set_active(&rom_gridbox, cs->reg.pc - 1);
	m_comp_ramgrid_set_all(&ram_gridbox, cs->mem.ram);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_R0, cs->reg.r0);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_R1, cs->reg.r1);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_R2, cs->reg.r2);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_R3, cs->reg.r3);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_R4, cs->reg.r4);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_R5, cs->reg.r5);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_R6, cs->reg.r6);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_R7, cs->reg.r7);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_PC, cs->reg.pc);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_SP, cs->reg.sp);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_IR, cs->reg.ir);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_AC, cs->reg.ac);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_SR_V, !!(cs->reg.sr & CS_SR_V));
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_SR_N, !!(cs->reg.sr & CS_SR_N));
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_SR_Z, !!(cs->reg.sr & CS_SR_Z));
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_SR_C, !!(cs->reg.sr & CS_SR_C));
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_MDR, cs->reg.mdr);
	m_comp_reggrid_set(&reg_gridbox, M_COMP_REGGRID_MAR, cs->reg.mar);
	m_comp_siggrid_set_signals(&sig_gridbox, cs->reg.signals);
}

bool open_machine_code_file(char *filepath, cs2010 *cs) {
	unsigned short *code = { 0 };
	size_t code_size = 0;
	int mcs_format = 0;
	char *disassembly = { 0 };

	if (!check_machine_running()) {
		return true;
	}

	if (mcs_import_file(filepath, &code, &code_size, &mcs_format) == MCS_IMPORT_SUCCESS) {
		m_comp_romgrid_clear(&rom_gridbox);
		m_comp_ramgrid_clear(&ram_gridbox);
		m_comp_reggrid_clear(&reg_gridbox);
		cs_hard_reset(cs);
		is_cs_ready = false;
		if (cs_load_and_check(cs, code, code_size) != CS_SUCCESS) {
			free(code);
			return false;
		}

		for (unsigned int i = 0; i < code_size; i++) {
			disassembly = as_disassemble_sentence(code[i]);
			if (!disassembly) {
				disassembly = "???";
				m_comp_romgrid_put_at(&rom_gridbox, i, code[i], disassembly);
			} else {
				m_comp_romgrid_put_at(&rom_gridbox, i, code[i], disassembly);
				free(disassembly);
			}
		}
		is_cs_ready = true;
		cs_fetch(cs);
		update_machine_ui(cs);
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
	IupSetAttribute(dlg, "EXTFILTER", "All files|*.*|");
	IupSetAttributeHandle(dlg, "PARENTDIALOG", main_window);
	IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);
	if (IupGetInt(dlg, "STATUS") != -1) {
		filepath = IupGetAttribute(dlg, "VALUE");
		if (!open_machine_code_file(filepath, cs)) {
			IupMessagef("Error", "Couldn't open file %s", filepath);
		}
	}
	IupDestroy(dlg);
	return IUP_IGNORE;
}

static bool save_disassembled_code_file(char const *filepath) {
	FILE *file = { 0 };
	int sentence_size;
	if (!is_cs_ready) {
		return false;
	}
	file = fopen(filepath, "w");
	if (!file) {
		return false;
	}

	sentence_size = m_comp_romgrid_get_sentence_count(&rom_gridbox);
	for (int i = 0; i < sentence_size; i++) {
		if (fputs(m_comp_romgrid_get_disassembly(&rom_gridbox, i), file) < 0 ||
			fputc('\n', file) != '\n') {
			fclose(file);
			return false;
		}
	}

	fclose(file);
	return true;
}

int save_disassembled_code_file_cb(Ihandle *self) {
	Ihandle *main_window = IupGetDialog(self);
	Ihandle *dlg;
	char *file = { 0 };

	if (!is_cs_ready) {
		return IUP_DEFAULT;
	}

	dlg = IupFileDlg();
	IupSetAttribute(dlg, "DIALOGTYPE", "SAVE");
	IupSetAttribute(dlg, "EXTFILTER", "Assembly files|*.asm|All files|*.*|");
	IupSetAttribute(dlg, "EXTDEFAULT", "asm");
	IupSetAttributeHandle(dlg, "PARENTDIALOG", main_window);

	IupPopup(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);
	if (IupGetInt(dlg, "STATUS") != -1) {
		file = IupGetAttribute(dlg, "VALUE");
		if (!save_disassembled_code_file(file)) {
			IupMessagef("Error", "Couldn't save the file %s", file);
		}
	}

	IupDestroy(dlg);
	return IUP_DEFAULT;
}

int exit_cb(Ihandle *self) {
	return IUP_CLOSE;
}

int set_machine_settings_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int step_instruction_cb(Ihandle *self) {
	if (is_cs_ready) {
		cs_fullstep(cs);
		update_machine_ui(cs);
	}
	return IUP_DEFAULT;
}

int step_microinstruction_cb(Ihandle *self) {
	if (is_cs_ready) {
		cs_microstep(cs);
		update_machine_ui(cs);
	}
	return IUP_DEFAULT;
}

int start_clock_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int stop_clock_cb(Ihandle *self) {
	return IUP_DEFAULT;
}

int reset_all_simulation_cb(Ihandle *self) {
	if (!is_cs_ready) {
		return IUP_DEFAULT;
	}
	
	cs_reset_registers(cs);
	cs_clear_memory(cs, CS_CLEAR_RAM);
	cs_soft_reset(cs);
	cs_fetch(cs);
	update_machine_ui(cs);
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
	Ihandle *main_window, *menu;
	/* File submenu */
	Ihandle *file_submenu, *open_machine_code_file_item, *save_dissasembled_code_file_item, *exit_item;
	/* Machine submenu */
	Ihandle *machine_submenu, *set_machine_settings_item;
	/* Simulation submenu */
	Ihandle *simulation_submenu, *step_instruction_item, *step_microinstruction_item,
		*start_clock_item, *stop_clock_item, *reset_all_simulation_item;
	/* About submenu */
	Ihandle *about_submenu, *about_item;
	/* Tabs and splits */
	Ihandle *rom_tabs, *reg_tabs, *ram_tabs, *sig_tabs, *romregram_split, *regram_split, *regsig_split;
	
	if (IupOpen(&argc, &argv) == IUP_ERROR) {
		puts("Error while opening GUI, aborting execution...\n");
		return EXIT_FAILURE;
	}
	IupSetGlobal("UTF8MODE", IUP_YES);
	setlocale(LC_ALL, "");

	open_machine_code_file_item = IupItem("Open machine code file\tCTRL+O", 0);
	IupSetCallback(open_machine_code_file_item, IUP_ACTION, (Icallback) open_machine_code_file_cb);
	save_dissasembled_code_file_item = IupItem("Save dissasembled code file\tCTRL+S", 0);
	IupSetCallback(save_dissasembled_code_file_item, IUP_ACTION, (Icallback) save_disassembled_code_file_cb);
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
		IupScrollBox(
			m_comp_romgrid_get_handler(&rom_gridbox)
		),
		0
	);
	IupSetAttribute(rom_tabs, "TABTITLE0", "ROM code");

	reg_gridbox = m_comp_reggrid_create();
	reg_tabs = IupTabs(
		IupScrollBox(
			m_comp_reggrid_get_handler(&reg_gridbox)
		),
		0
	);
	IupSetAttribute(reg_tabs, "TABTITLE0", "Registers");

	sig_gridbox = m_comp_siggrid_create();
	sig_tabs = IupTabs(
		IupScrollBox(
			m_comp_siggrid_get_handler(&sig_gridbox)
		),
		0
	);
	IupSetAttribute(sig_tabs, "TABTITLE0", "Signals");

	ram_gridbox = m_comp_ramgrid_create();
	ram_tabs = IupTabs(
		IupScrollBox(
			m_comp_ramgrid_get_handler(&ram_gridbox)
		),
		0
	);
	IupSetAttribute(ram_tabs, "TABTITLE0", "RAM");

	regsig_split = IupSplit(reg_tabs, sig_tabs);
	IupSetAttribute(regsig_split, "LAYOUTDRAG", IUP_NO);
	IupSetAttribute(IupGetChild(regsig_split, 0), "STYLE", "EMPTY");
	IupSetAttribute(regsig_split, IUP_ORIENTATION, IUP_VERTICAL);

	regram_split = IupSplit(regsig_split, ram_tabs);
	IupSetAttribute(regram_split, "LAYOUTDRAG", IUP_NO);
	IupSetAttribute(IupGetChild(regram_split, 0), "STYLE", "EMPTY");
	IupSetAttribute(regram_split, IUP_ORIENTATION, IUP_HORIZONTAL);

	romregram_split = IupSplit(rom_tabs, regram_split);
	IupSetAttribute(romregram_split, "LAYOUTDRAG", IUP_NO);
	IupSetAttribute(IupGetChild(romregram_split, 0), "STYLE", "EMPTY");

	main_window = IupDialog(
		romregram_split
	);

	IupSetAttributeHandle(main_window, IUP_MENU, menu);
	IupSetAttribute(main_window, IUP_TITLE, PROGRAM_NAME);
	IupSetCallback(main_window, "K_cO", (Icallback) open_machine_code_file_cb);
	IupSetCallback(main_window, "K_cS", (Icallback) save_disassembled_code_file_cb);
	IupSetCallback(main_window, "K_cQ", (Icallback) exit_cb);
	IupSetCallback(main_window, "K_cI", (Icallback) step_instruction_cb);
	IupSetCallback(main_window, "K_cM", (Icallback) step_microinstruction_cb);
	IupSetCallback(main_window, "K_cR", (Icallback) reset_all_simulation_cb);
	IupSetCallback(main_window, IUP_CLOSE_CB, (Icallback) exit_cb);

	cs = malloc(sizeof * cs);
	if (!cs) {
		IupMessage("Error", "Couldn't initialize CS2010");
		IupClose();
		return EXIT_FAILURE;
	}
	cs_init(cs);
	is_cs_ready = false;

	IupSetAttribute(main_window, IUP_SIZE, IUP_HALF "x" IUP_HALF);
	IupShowXY(main_window, IUP_CENTER, IUP_CENTER);
	IupSetAttribute(main_window, IUP_SHRINK, IUP_YES);
	IupMainLoop();
	IupClose();
	return EXIT_SUCCESS;
}
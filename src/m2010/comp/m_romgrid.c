/** @file m_romgrid.c */

#include <stddef.h>

#include "../../utils.h"

#include "m_romgrid.h"

#define NUM_COLS 3
#define COL_HEADER_1 "ADDRESS"
#define COL_HEADER_2 "CONTENT"
#define COL_HEADER_3 "DISASSEMBLY"

#define GET_ROWS_SIZE(child_count) ((child_count / NUM_COLS) - 1)
#define GET_ROW_IDX(address) ((address + 1) * NUM_COLS)

#define ADDRESS_FORMAT HEX8_X_FORMAT
#define CONTENT_FORMAT HEX16_X_FORMAT
#define DISASSEMBLY_FORMAT "%s"

#define CONTENT_UNKNOWN "???"

m_comp_romgrid m_comp_romgrid_create(void) {
    m_comp_romgrid romgrid = { 0 };

    romgrid.handler = IupGridBox(0);
    IupSetAttribute(romgrid.handler, "EXPANDCHILDREN", IUP_HORIZONTAL);
    IupSetAttribute(romgrid.handler, IUP_MARGIN, "3x0");
    IupSetInt(romgrid.handler, "NUMDIV", NUM_COLS);
    IupSetInt(romgrid.handler, "GAPLIN", 5);
    IupSetInt(romgrid.handler, "GAPCOL", 15);
    IupSetInt(romgrid.handler, "SIZECOL", -1);
    IupSetInt(romgrid.handler, "SIZELIN", -1);

    IupAppend(romgrid.handler, IupLabel(COL_HEADER_1));
    IupAppend(romgrid.handler, IupLabel(COL_HEADER_2));
    IupAppend(romgrid.handler, IupLabel(COL_HEADER_3));

    IupAppend(romgrid.handler, IupLabel(CONTENT_UNKNOWN));
    IupAppend(romgrid.handler, IupLabel(CONTENT_UNKNOWN));
    IupAppend(romgrid.handler, IupLabel(CONTENT_UNKNOWN));

    romgrid.any_row_active = false;
    romgrid.invalid_pc_warned = false;
    romgrid.active_row = 0;

    return romgrid;
}

Ihandle *m_comp_romgrid_get_handler(m_comp_romgrid *romgrid) {
    if (!romgrid) {
        return 0;
    }
    return romgrid->handler;
}

void m_comp_romgrid_clear(m_comp_romgrid *romgrid) {
    Ihandle *gridbox = { 0 };
    Ihandle *prev_child = { 0 };
    Ihandle *next_child = { 0 };
    if (!romgrid) {
        return;
    }

    m_comp_romgrid_clear_active(romgrid);
    romgrid->invalid_pc_warned = false;
    gridbox = m_comp_romgrid_get_handler(romgrid);
    prev_child = IupGetChild(gridbox, NUM_COLS - 1);
    next_child = IupGetNextChild(gridbox, prev_child);
    while (next_child) {
        prev_child = next_child;
        next_child = IupGetNextChild(gridbox, prev_child);
        IupDestroy(prev_child);
    }
}

void m_comp_romgrid_put_at(m_comp_romgrid *romgrid, int address, unsigned short content, char *disassembly) {
    Ihandle *gridbox = { 0 };
    Ihandle *label_address = { 0 };
    Ihandle *label_content = { 0 };
    Ihandle *label_disassembly = { 0 };
    int element_number = 0;
    int i = 0;

    if (!romgrid) {
        return;
    }

    gridbox = m_comp_romgrid_get_handler(romgrid);
    element_number = GET_ROWS_SIZE(IupGetChildCount(gridbox));

    if (address < 0) {
        return;
    } else if (address < element_number) {
        IupSetfAttribute(IupGetChild(gridbox, GET_ROW_IDX(address)), "TITLE", ADDRESS_FORMAT, address);
        IupSetfAttribute(IupGetChild(gridbox, GET_ROW_IDX(address) + 1), "TITLE", CONTENT_FORMAT, content);
        IupSetfAttribute(IupGetChild(gridbox, GET_ROW_IDX(address) + 2), "TITLE", "%s", disassembly);
        IupRefresh(gridbox);
    } else {
        for (i = element_number; i < address; i++) {
            IupAppend(gridbox, (label_address = IupLabel("")));
            IupMap(label_address);
            IupAppend(gridbox, (label_content = IupLabel(CONTENT_UNKNOWN)));
            IupMap(label_content);
            IupAppend(gridbox, (label_disassembly = IupLabel(CONTENT_UNKNOWN)));
            IupMap(label_disassembly);
            IupSetfAttribute(label_address, "TITLE", ADDRESS_FORMAT, i);
        }

        IupAppend(gridbox, (label_address = IupLabel("")));
        IupMap(label_address);
        IupAppend(gridbox, (label_content = IupLabel("")));
        IupMap(label_content);
        IupAppend(gridbox, (label_disassembly = IupLabel("")));
        IupMap(label_disassembly);
        IupSetfAttribute(label_address, "TITLE", ADDRESS_FORMAT, address);
        IupSetfAttribute(label_content, "TITLE", CONTENT_FORMAT, content);
        IupSetfAttribute(label_disassembly, "TITLE", "%s", disassembly);
        IupRefresh(gridbox);
    }
}

void m_comp_romgrid_set_active(m_comp_romgrid *romgrid, int index) {
    Ihandle *gridbox = { 0 };
    if (!romgrid) {
        return;
    }
    gridbox = m_comp_romgrid_get_handler(romgrid);

    if (index < 0 || index > GET_ROWS_SIZE(IupGetChildCount(gridbox)) - 1) {
        m_comp_romgrid_clear_active(romgrid);
        if (!romgrid->invalid_pc_warned) {
            IupMessagef("Caution", "PC is pointing at addresses containing garbage values, undefined behaviour could happen!");
            romgrid->invalid_pc_warned = true;
        }
        return;
    }

    if (romgrid->any_row_active) {
        m_comp_romgrid_clear_active(romgrid);
    }
    IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(index)), "FONTSTYLE", "Bold");
    IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(index) + 1), "FONTSTYLE", "Bold");
    IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(index) + 2), "FONTSTYLE", "Bold");

    romgrid->any_row_active = true;
    romgrid->active_row = index;
}

void m_comp_romgrid_clear_active(m_comp_romgrid *romgrid) {
    Ihandle *gridbox = { 0 };
    if (!romgrid) {
        return;
    }
    gridbox = m_comp_romgrid_get_handler(romgrid);

    if (romgrid->any_row_active) {
        IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(romgrid->active_row)), "FONTSTYLE", "");
        IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(romgrid->active_row) + 1), "FONTSTYLE", "");
        IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(romgrid->active_row) + 2), "FONTSTYLE", "");
        romgrid->any_row_active = false;
    }
}

int m_comp_romgrid_get_sentence_count(m_comp_romgrid *romgrid) {
    Ihandle *gridbox = { 0 };
    if (!romgrid) {
        return 0;
    }
    gridbox = m_comp_romgrid_get_handler(romgrid);

    return GET_ROWS_SIZE(IupGetChildCount(gridbox));
}

char const *m_comp_romgrid_get_disassembly(m_comp_romgrid *romgrid, int index) {
    Ihandle *gridbox = { 0 };
    if (!romgrid) {
        return 0;
    }
    gridbox = m_comp_romgrid_get_handler(romgrid);

    if (index < 0 || index > GET_ROWS_SIZE(IupGetChildCount(gridbox) - 1)) {
        return 0;
    }

    return IupGetAttribute(IupGetChild(gridbox, GET_ROW_IDX(index) + 2), IUP_TITLE);
}
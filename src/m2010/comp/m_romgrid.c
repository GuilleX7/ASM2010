/** @file m_romgrid.c */

#include <stddef.h>

#include "m_romgrid.h"

#define NUM_COLS 3
#define COL_HEADER_1 "ADDRESS"
#define COL_HEADER_2 "CONTENT"
#define COL_HEADER_3 "DISASSEMBLY"

#define GET_ROWS_SIZE(child_count) ((child_count / NUM_COLS) - 1)
#define GET_ROW_IDX(index) ((index + 1) * NUM_COLS)

#define IDX_HEX_FORMAT "%02X"
#define SENTENCE_HEX_FORMAT "%04X"
#define CONTENT_UNKNOWN "???"

Ihandle *m_comp_romgrid_get_handler(m_comp_romgrid *romgrid) {
    if (!romgrid) {
        return 0;
    }
    return romgrid->handler;
}

m_comp_romgrid m_comp_romgrid_create() {
    m_comp_romgrid romgrid = { 0 };

    romgrid.handler = IupGridBox(0);
    IupSetAttribute(romgrid.handler, "EXPANDCHILDREN", IUP_HORIZONTAL);
    IupSetAttribute(romgrid.handler, IUP_MARGIN, "10x5");
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
    romgrid.active_row = 0;

    return romgrid;
}

void m_comp_romgrid_clear(m_comp_romgrid *romgrid) {
    Ihandle *gridbox = { 0 };
    Ihandle *prev_child = { 0 };
    Ihandle *next_child = { 0 };
    if (!romgrid) {
        return;
    }

    gridbox = m_comp_romgrid_get_handler(romgrid);
    prev_child = IupGetChild(gridbox, NUM_COLS - 1);
    next_child = IupGetNextChild(gridbox, prev_child);
    while (next_child) {
        prev_child = next_child;
        next_child = IupGetNextChild(gridbox, prev_child);
        IupDestroy(prev_child);
    }
}

void m_comp_romgrid_put_at(m_comp_romgrid *romgrid, int index, uint16_t sentence, char *disassembly) {
    Ihandle *gridbox = { 0 };
    Ihandle *label_idx = { 0 };
    Ihandle *label_sentence = { 0 };
    Ihandle *label_disassembly = { 0 };
    int element_number = 0;
    if (!romgrid) {
        return;
    }

    gridbox = m_comp_romgrid_get_handler(romgrid);
    element_number = GET_ROWS_SIZE(IupGetChildCount(gridbox));
    if (index < element_number) {
        IupSetfAttribute(IupGetChild(gridbox, GET_ROW_IDX(index)), "TITLE", IDX_HEX_FORMAT, index);
        IupSetfAttribute(IupGetChild(gridbox, GET_ROW_IDX(index) + 1), "TITLE", SENTENCE_HEX_FORMAT, sentence);
        IupSetfAttribute(IupGetChild(gridbox, GET_ROW_IDX(index) + 2), "TITLE", "%s", disassembly);
        IupRefresh(gridbox);
    } else {
        for (size_t i = element_number; i < index; i++) {
            IupAppend(gridbox, (label_idx = IupLabel("")));
            IupMap(label_idx);
            IupAppend(gridbox, (label_sentence = IupLabel(CONTENT_UNKNOWN)));
            IupMap(label_sentence);
            IupAppend(gridbox, (label_disassembly = IupLabel(CONTENT_UNKNOWN)));
            IupMap(label_disassembly);
            IupSetfAttribute(label_idx, "TITLE", IDX_HEX_FORMAT, i);
        }

        IupAppend(gridbox, (label_idx = IupLabel("")));
        IupMap(label_idx);
        IupAppend(gridbox, (label_sentence = IupLabel("")));
        IupMap(label_sentence);
        IupAppend(gridbox, (label_disassembly = IupLabel("")));
        IupMap(label_disassembly);
        IupSetfAttribute(label_idx, "TITLE", IDX_HEX_FORMAT, index);
        IupSetfAttribute(label_sentence, "TITLE", SENTENCE_HEX_FORMAT, sentence);
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

    if (romgrid->any_row_active) {
        IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(romgrid->active_row)), "FONTSTYLE", "");
        IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(romgrid->active_row) + 1), "FONTSTYLE", "");
        IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(romgrid->active_row) + 2), "FONTSTYLE", "");
    } else {
        romgrid->any_row_active = true;
    }

    IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(index)), "FONTSTYLE", "Bold");
    IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(index) + 1), "FONTSTYLE", "Bold");
    IupSetAttribute(IupGetChild(gridbox, GET_ROW_IDX(index) + 2), "FONTSTYLE", "Bold");
    romgrid->active_row = index;

    IupRefresh(gridbox);
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
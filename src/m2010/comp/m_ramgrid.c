/** @file m_ramgrid.c */

#include "../../utils.h"

#include "m_ramgrid.h"

#define NUM_COLS 16
#define NUM_ROWS 16
#define RAM_SIZE (NUM_COLS * NUM_ROWS)

#define GET_ROW_IDX(address) (address / NUM_COLS * 2 + 1)

#define CONTENT_FORMAT \
        HEX8_FORMAT " " HEX8_FORMAT " " HEX8_FORMAT " " HEX8_FORMAT " "\
        HEX8_FORMAT " " HEX8_FORMAT " " HEX8_FORMAT " " HEX8_FORMAT " "\
        HEX8_FORMAT " " HEX8_FORMAT " " HEX8_FORMAT " " HEX8_FORMAT " "\
        HEX8_FORMAT " " HEX8_FORMAT " " HEX8_FORMAT " " HEX8_FORMAT

m_comp_ramgrid m_comp_ramgrid_create(void) {
    m_comp_ramgrid ramgrid = { 0 };
    Ihandle *address_label;
    Ihandle *value_label;

    ramgrid.handler = IupGridBox(0);
    IupSetAttribute(ramgrid.handler, IUP_MARGIN, "3x0");
    IupSetAttribute(ramgrid.handler, "EXPANDCHILDREN", IUP_HORIZONTAL);
    IupSetInt(ramgrid.handler, "NUMDIV", 2);
    IupSetInt(ramgrid.handler, "GAPLIN", 3);
    IupSetInt(ramgrid.handler, "GAPCOL", 10);
    IupSetInt(ramgrid.handler, "SIZECOL", -1);
    IupSetInt(ramgrid.handler, "SIZELIN", -1);
    for (size_t i = 0; i < NUM_ROWS; i++) {
        IupAppend(ramgrid.handler, address_label = IupLabel(""));
        IupSetAttribute(address_label, IUP_ALIGNMENT, IUP_ALEFT);
        IupSetStrf(address_label, IUP_TITLE, HEX8_FORMAT, i * NUM_COLS);
        IupAppend(ramgrid.handler, value_label = IupLabel(""));
        IupSetAttribute(value_label, IUP_ALIGNMENT, IUP_ALEFT);
        IupSetAttribute(value_label, IUP_FONT, "Courier 11");
    }
    m_comp_ramgrid_clear(&ramgrid);
    return ramgrid;
}

Ihandle *m_comp_ramgrid_get_handler(m_comp_ramgrid *ramgrid) {
    if (!ramgrid) {
        return 0;
    }
    return ramgrid->handler;
}

void m_comp_ramgrid_set_row(m_comp_ramgrid *ramgrid, int row_address, unsigned char *values) {
    if (!ramgrid || row_address > (NUM_ROWS * NUM_COLS) - 1) {
        return;
    }
    int idx = GET_ROW_IDX(row_address);
    IupSetStrf(IupGetChild(m_comp_ramgrid_get_handler(ramgrid), idx), IUP_TITLE, CONTENT_FORMAT,
        values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7],
        values[8], values[9], values[10], values[11], values[12], values[13], values[14], values[15]);
}

void m_comp_ramgrid_set_all(m_comp_ramgrid *ramgrid, unsigned char *values) {
    int idx;
    if (!ramgrid) {
        return;
    }

    for (int i = 0; i < RAM_SIZE; i += 16) {
        idx = GET_ROW_IDX(i);
        IupSetStrf(IupGetChild(m_comp_ramgrid_get_handler(ramgrid), idx), IUP_TITLE, CONTENT_FORMAT,
            values[i], values[i + 1], values[i + 2], values[i + 3], values[i + 4], values[i + 5], values[i + 6], values[i + 7],
            values[i + 8], values[i + 9], values[i + 10], values[i + 11], values[i + 12], values[i + 13], values[i + 14], values[i + 15]);
    }
}

void m_comp_ramgrid_clear(m_comp_ramgrid *ramgrid) {
    if (!ramgrid) {
        return;
    }
    unsigned char zero[NUM_COLS] = { 0 };
    for (int i = 0; i < NUM_ROWS; i++) {
        m_comp_ramgrid_set_row(ramgrid, i * NUM_COLS, zero);
    }
}
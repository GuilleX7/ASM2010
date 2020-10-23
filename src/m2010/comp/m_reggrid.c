/** @file m_reggrid.c */

#include "../../utils.h"

#include "m_reggrid.h"

#define NUM_ROWS 15
#define NUM_COLS 2

#define GET_ROW_IDX(index) (index * NUM_COLS + 1)

#define DEFINE_REGISTER(name, format) {name, format}

m_comp_reggrid_reg const regs[M_COMP_REGGRID_LENGTH] = {
    DEFINE_REGISTER("R0", HEX8_X_FORMAT),
    DEFINE_REGISTER("R1", HEX8_X_FORMAT),
    DEFINE_REGISTER("R2", HEX8_X_FORMAT),
    DEFINE_REGISTER("R3", HEX8_X_FORMAT),
    DEFINE_REGISTER("R4", HEX8_X_FORMAT),
    DEFINE_REGISTER("R5", HEX8_X_FORMAT),
    DEFINE_REGISTER("R6", HEX8_X_FORMAT),
    DEFINE_REGISTER("R7", HEX8_X_FORMAT),
    DEFINE_REGISTER("PC", HEX8_X_FORMAT),
    DEFINE_REGISTER("SP", HEX8_X_FORMAT),
    DEFINE_REGISTER("IR", HEX16_X_FORMAT),
    DEFINE_REGISTER("AC", HEX8_X_FORMAT),
    DEFINE_REGISTER("SR_V", BOOLEAN_FORMAT),
    DEFINE_REGISTER("SR_N", BOOLEAN_FORMAT),
    DEFINE_REGISTER("SR_Z", BOOLEAN_FORMAT),
    DEFINE_REGISTER("SR_C", BOOLEAN_FORMAT),
    DEFINE_REGISTER("MDR", HEX8_X_FORMAT),
    DEFINE_REGISTER("MAR", HEX8_X_FORMAT)
};

m_comp_reggrid m_comp_reggrid_create(void) {
    m_comp_reggrid reggrid = { 0 };
    Ihandle *name_label = { 0 };
    Ihandle *value_label = { 0 };
    size_t i = 0;

    reggrid.handler = IupGridBox(0);
    IupSetAttribute(reggrid.handler, "EXPANDCHILDREN", IUP_HORIZONTAL);
    IupSetAttribute(reggrid.handler, IUP_MARGIN, "3x0");
    IupSetInt(reggrid.handler, "NUMDIV", NUM_COLS);
    IupSetInt(reggrid.handler, "GAPLIN", 5);
    IupSetInt(reggrid.handler, "GAPCOL", 15);
    IupSetInt(reggrid.handler, "SIZECOL", -1);
    IupSetInt(reggrid.handler, "SIZELIN", -1);

    for (i = 0; i < M_COMP_REGGRID_LENGTH; i++) {
        if (regs[i].name) {
            IupAppend(reggrid.handler, name_label = IupLabel(regs[i].name));
            IupSetAttribute(name_label, IUP_ALIGNMENT, IUP_ALEFT);
            IupAppend(reggrid.handler, value_label = IupLabel(""));
            IupSetAttribute(value_label, IUP_ALIGNMENT, IUP_ARIGHT);
        } else {
            IupAppend(reggrid.handler, IupSpace());
            IupAppend(reggrid.handler, IupSpace());
        }
    }

    m_comp_reggrid_clear(&reggrid);
    return reggrid;
}

Ihandle *m_comp_reggrid_get_handler(m_comp_reggrid *reggrid) {
    if (!reggrid) {
        return 0;
    }
    return reggrid->handler;
}

void m_comp_reggrid_set(m_comp_reggrid *reggrid, m_comp_reggrid_reg_idx reg, size_t value) {
    int idx;

    if (!reggrid || reg < 0 || reg > M_COMP_REGGRID_LENGTH || !regs[reg].name) {
        return;
    }

    idx = GET_ROW_IDX(reg);
    IupSetStrf(IupGetChild(m_comp_reggrid_get_handler(reggrid), idx), IUP_TITLE, regs[reg].format, value);
}

void m_comp_reggrid_clear(m_comp_reggrid *reggrid) {
    Ihandle *gridbox = { 0 };
    int idx = 0;
    int i = 0;

    if (!reggrid) {
        return;
    }

    gridbox = m_comp_reggrid_get_handler(reggrid);
    for (i = 0; i < M_COMP_REGGRID_LENGTH; i++) {
        idx = GET_ROW_IDX(i);
        if (regs[i].name) {
            IupSetStrf(IupGetChild(gridbox, idx), IUP_TITLE, regs[i].format, 0);
        }
    }
}
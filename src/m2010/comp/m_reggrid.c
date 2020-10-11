/** @file m_reggrid.c */

#include "../../utils.h"

#include "m_reggrid.h"

#define NUM_ROWS 15
#define NUM_COLS 2

#define COL_HEADER_1 "NAME"
#define COL_HEADER_2 "VALUE"

#define GET_ROW_IDX(index) (index * NUM_COLS)

m_comp_reggrid_reg const regs[M_COMP_REGGRID_LENGTH] = {
    [M_COMP_REGGRID_R0] = {.name = "R0", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_R1] = {.name = "R1", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_R2] = {.name = "R2", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_R3] = {.name = "R3", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_R4] = {.name = "R4", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_R5] = {.name = "R5", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_R6] = {.name = "R6", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_R7] = {.name = "R7", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_PC] = {.name = "PC", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_SP] = {.name = "SP", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_IR] = {.name = "IR", .format = HEX16_FORMAT },
    [M_COMP_REGGRID_AC] = {.name = "AC", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_SR_V] = {.name = "SR_V", .format = BOOLEAN_FORMAT },
    [M_COMP_REGGRID_SR_N] = {.name = "SR_N", .format = BOOLEAN_FORMAT },
    [M_COMP_REGGRID_SR_Z] = {.name = "SR_Z", .format = BOOLEAN_FORMAT },
    [M_COMP_REGGRID_SR_C] = {.name = "SR_C", .format = BOOLEAN_FORMAT },
    [M_COMP_REGGRID_MDR] = {.name = "MDR", .format = HEX8_FORMAT },
    [M_COMP_REGGRID_MAR] = {.name = "MAR", .format = HEX8_FORMAT },
};

m_comp_reggrid m_comp_reggrid_create(void) {
    m_comp_reggrid reggrid = { 0 };
    Ihandle *name_label = { 0 };
    Ihandle *value_label = { 0 };

    reggrid.handler = IupGridBox(0);
    IupSetAttribute(reggrid.handler, "EXPANDCHILDREN", IUP_HORIZONTAL);
    IupSetAttribute(reggrid.handler, IUP_MARGIN, "3x0");
    IupSetInt(reggrid.handler, "NUMDIV", NUM_COLS);
    IupSetInt(reggrid.handler, "GAPLIN", 5);
    IupSetInt(reggrid.handler, "GAPCOL", 15);
    IupSetInt(reggrid.handler, "SIZECOL", -1);
    IupSetInt(reggrid.handler, "SIZELIN", -1);

    for (size_t i = 0; i < M_COMP_REGGRID_LENGTH; i++) {
        if (regs[i].name) {
            IupAppend(reggrid.handler, name_label = IupLabel(regs[i].name));
            IupSetAttribute(name_label, IUP_ALIGNMENT, IUP_ALEFT);
            IupAppend(reggrid.handler, value_label = IupLabel(""));
            IupSetAttribute(value_label, IUP_ALIGNMENT, IUP_ARIGHT);
            IupSetStrf(value_label, IUP_TITLE, regs[i].format, 0);
        } else {
            IupAppend(reggrid.handler, IupSpace());
            IupAppend(reggrid.handler, IupSpace());
        }
    }

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
    idx = GET_ROW_IDX(reg) + 1;
    IupSetStrf(IupGetChild(m_comp_reggrid_get_handler(reggrid), idx), IUP_TITLE, regs[reg].format, value);
}

void m_comp_reggrid_clear(m_comp_reggrid *reggrid) {
    Ihandle *gridbox = { 0 };
    int idx = 0;
    if (!reggrid) {
        return;
    }
    gridbox = m_comp_reggrid_get_handler(reggrid);
    for (int i = 0; i < M_COMP_REGGRID_LENGTH; i++) {
        idx = GET_ROW_IDX(i) + 1;
        if (regs[i].name) {
            IupSetStrf(IupGetChild(gridbox, idx), IUP_TITLE, regs[i].format, 0);
        }
    }
}
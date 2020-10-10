/** @file m_reggrid.c */

#include "../../utils.h"

#include "m_reggrid.h"

#define NUM_ROWS 15
#define NUM_COLS 2

#define COL_HEADER_1 "NAME"
#define COL_HEADER_2 "VALUE"

m_comp_reggrid_reg const regs[M_COMP_REGGRID_LENGTH] = {
    [M_COMP_REGGRID_R0] = {.name = "R0", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_R1] = {.name = "R1", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_R2] = {.name = "R2", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_R3] = {.name = "R3", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_R4] = {.name = "R4", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_R5] = {.name = "R5", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_R6] = {.name = "R6", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_R7] = {.name = "R7", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_PC] = {.name = "PC", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_SP] = {.name = "SP", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_IR] = {.name = "IR", .format = UINT16_HEX_FORMAT },
    [M_COMP_REGGRID_AC] = {.name = "AC", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_SR] = {.name = "SR", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_MDR] = {.name = "MDR", .format = UINT8_HEX_FORMAT },
    [M_COMP_REGGRID_MAR] = {.name = "MAR", .format = UINT8_HEX_FORMAT },
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
            IupSetStrf(value_label, IUP_TITLE, regs[i].format, regs[i].value);
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
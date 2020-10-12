/** @file m_siggrid.c */

#include "../../utils.h"

#include "../../cs2010/cs_registers.h"

#include "m_siggrid.h"

#define NUM_ROWS M_COMP_SIGGRID_SIGNAL_LENGTH
#define NUM_COLS 2

#define GET_ROW_IDX(index) (index * NUM_COLS + 1)

m_comp_siggrid_signal const sigs[M_COMP_SIGGRID_SIGNAL_LENGTH] = {
    [M_COMP_SIGGRID_SIGNAL_WMAR] = {.name = "WMAR", .bitmask = CS_SIGNAL_WMAR },
    [M_COMP_SIGGRID_SIGNAL_WMDR] = {.name = "WMDR", .bitmask = CS_SIGNAL_WMDR },
    [M_COMP_SIGGRID_SIGNAL_IOMDR] = {.name = "IOMDR", .bitmask = CS_SIGNAL_IOMDR },
    [M_COMP_SIGGRID_SIGNAL_WMEM] = {.name = "WMEM", .bitmask = CS_SIGNAL_WMEM },
    [M_COMP_SIGGRID_SIGNAL_RMEM] = {.name = "RMEM", .bitmask = CS_SIGNAL_RMEM },
    [M_COMP_SIGGRID_SIGNAL_WIR] = {.name = "WIR", .bitmask = CS_SIGNAL_WIR },
    [M_COMP_SIGGRID_SIGNAL_WPC] = {.name = "WPC", .bitmask = CS_SIGNAL_WPC },
    [M_COMP_SIGGRID_SIGNAL_RPC] = {.name = "RPC", .bitmask = CS_SIGNAL_RPC },
    [M_COMP_SIGGRID_SIGNAL_CPC] = {.name = "CPC", .bitmask = CS_SIGNAL_CPC },
    [M_COMP_SIGGRID_SIGNAL_IPC] = {.name = "IPC", .bitmask = CS_SIGNAL_IPC },
    [M_COMP_SIGGRID_SIGNAL_ISP] = {.name = "ISP", .bitmask = CS_SIGNAL_ISP },
    [M_COMP_SIGGRID_SIGNAL_DSP] = {.name = "DSP", .bitmask = CS_SIGNAL_DSP },
    [M_COMP_SIGGRID_SIGNAL_CSP] = {.name = "CSP", .bitmask = CS_SIGNAL_CSP },
    [M_COMP_SIGGRID_SIGNAL_RSP] = {.name = "RSP", .bitmask = CS_SIGNAL_RSP },
    [M_COMP_SIGGRID_SIGNAL_INM] = {.name = "INM", .bitmask = CS_SIGNAL_INM },
    [M_COMP_SIGGRID_SIGNAL_SRW] = {.name = "SRW", .bitmask = CS_SIGNAL_SRW },
    [M_COMP_SIGGRID_SIGNAL_WAC] = {.name = "WAC", .bitmask = CS_SIGNAL_WAC },
    [M_COMP_SIGGRID_SIGNAL_RAC] = {.name = "RAC", .bitmask = CS_SIGNAL_RAC },
    [M_COMP_SIGGRID_SIGNAL_ALUOP0] = {.name = "ALUOP0", .bitmask = CS_SIGNAL_ALUOP0 },
    [M_COMP_SIGGRID_SIGNAL_ALUOP1] = {.name = "ALUOP1", .bitmask = CS_SIGNAL_ALUOP1 },
    [M_COMP_SIGGRID_SIGNAL_ALUOP2] = {.name = "ALUOP2", .bitmask = CS_SIGNAL_ALUOP2 },
    [M_COMP_SIGGRID_SIGNAL_ALUOP3] = {.name = "ALUOP3", .bitmask = CS_SIGNAL_ALUOP3 },
    [M_COMP_SIGGRID_SIGNAL_WREG] = {.name = "WREG", .bitmask = CS_SIGNAL_WREG },
};

m_comp_siggrid m_comp_siggrid_create(void) {
    m_comp_siggrid siggrid = { 0 };
    Ihandle *name_label = { 0 };
    Ihandle *value_label = { 0 };

    siggrid.handler = IupGridBox(0);
    IupSetAttribute(siggrid.handler, "EXPANDCHILDREN", IUP_HORIZONTAL);
    IupSetAttribute(siggrid.handler, IUP_MARGIN, "3x0");
    IupSetInt(siggrid.handler, "NUMDIV", NUM_COLS);
    IupSetInt(siggrid.handler, "GAPLIN", 5);
    IupSetInt(siggrid.handler, "GAPCOL", 15);
    IupSetInt(siggrid.handler, "SIZECOL", -1);
    IupSetInt(siggrid.handler, "SIZELIN", -1);

    for (size_t i = 0; i < M_COMP_SIGGRID_SIGNAL_LENGTH; i++) {
        if (sigs[i].name) {
            IupAppend(siggrid.handler, name_label = IupLabel(sigs[i].name));
            IupSetAttribute(name_label, IUP_ALIGNMENT, IUP_ALEFT);
            IupAppend(siggrid.handler, value_label = IupLabel(""));
            IupSetAttribute(value_label, IUP_ALIGNMENT, IUP_ARIGHT);
        } else {
            IupAppend(siggrid.handler, IupSpace());
            IupAppend(siggrid.handler, IupSpace());
        }
    }

    m_comp_siggrid_clear(&siggrid);
    return siggrid;
}

Ihandle *m_comp_siggrid_get_handler(m_comp_siggrid *siggrid) {
    if (!siggrid) {
        return 0;
    }
    return siggrid->handler;
}

void m_comp_siggrid_set(m_comp_siggrid *siggrid, m_comp_siggrid_signal_idx signal, int value) {
    int idx;
    if (!siggrid || signal < 0 || signal > M_COMP_SIGGRID_SIGNAL_LENGTH || !sigs[signal].name) {
        return;
    }
    idx = GET_ROW_IDX(signal);
    IupSetStrf(IupGetChild(m_comp_siggrid_get_handler(siggrid), idx), IUP_TITLE, BOOLEAN_FORMAT, !!value);
}

void m_comp_siggrid_set_signals(m_comp_siggrid *siggrid, unsigned long signals) {
    Ihandle *gridbox = { 0 };
    int idx;
    if (!siggrid) {
        return;
    }
    gridbox = m_comp_siggrid_get_handler(siggrid);
    for (int i = 0; i < M_COMP_SIGGRID_SIGNAL_LENGTH; i++) {
        idx = GET_ROW_IDX(i);
        if (sigs[i].name) {
            IupSetStrf(IupGetChild(gridbox, idx), IUP_TITLE, BOOLEAN_FORMAT, !!(signals & sigs[i].bitmask));
        }
    }
}

void m_comp_siggrid_clear(m_comp_siggrid *siggrid) {
    Ihandle *gridbox = { 0 };
    int idx;
    if (!siggrid) {
        return;
    }
    gridbox = m_comp_siggrid_get_handler(siggrid);
    for (int i = 0; i < M_COMP_SIGGRID_SIGNAL_LENGTH; i++) {
        idx = GET_ROW_IDX(i);
        if (sigs[i].name) {
            IupSetStrf(IupGetChild(gridbox, idx), IUP_TITLE, BOOLEAN_FORMAT, 0);
        }
    }
}
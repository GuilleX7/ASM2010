/** @file m_siggrid.c */

#include "../../utils.h"

#include "../../cs2010/cs_registers.h"

#include "m_siggrid.h"

#define NUM_ROWS M_COMP_SIGGRID_SIGNAL_LENGTH
#define NUM_COLS 2

#define GET_ROW_IDX(index) (index * NUM_COLS + 1)

#define DEFINE_SIGNAL(name, bitmask) {name, bitmask}

m_comp_siggrid_signal const sigs[M_COMP_SIGGRID_SIGNAL_LENGTH] = {
    DEFINE_SIGNAL("WMAR", CS_SIGNAL_WMAR),
    DEFINE_SIGNAL("WMDR", CS_SIGNAL_WMDR),
    DEFINE_SIGNAL("IOMDR", CS_SIGNAL_IOMDR),
    DEFINE_SIGNAL("WMEM", CS_SIGNAL_WMEM),
    DEFINE_SIGNAL("RMEM", CS_SIGNAL_RMEM),
    DEFINE_SIGNAL("WIR", CS_SIGNAL_WIR),
    DEFINE_SIGNAL("WPC", CS_SIGNAL_WPC),
    DEFINE_SIGNAL("RPC", CS_SIGNAL_RPC),
    DEFINE_SIGNAL("CPC", CS_SIGNAL_CPC),
    DEFINE_SIGNAL("IPC", CS_SIGNAL_IPC),
    DEFINE_SIGNAL("ISP", CS_SIGNAL_ISP),
    DEFINE_SIGNAL("DSP", CS_SIGNAL_DSP),
    DEFINE_SIGNAL("CSP", CS_SIGNAL_CSP),
    DEFINE_SIGNAL("RSP", CS_SIGNAL_RSP),
    DEFINE_SIGNAL("INM", CS_SIGNAL_INM),
    DEFINE_SIGNAL("SRW", CS_SIGNAL_SRW),
    DEFINE_SIGNAL("WAC", CS_SIGNAL_WAC),
    DEFINE_SIGNAL("RAC", CS_SIGNAL_RAC),
    DEFINE_SIGNAL("ALUOP0", CS_SIGNAL_ALUOP0),
    DEFINE_SIGNAL("ALUOP1", CS_SIGNAL_ALUOP1),
    DEFINE_SIGNAL("ALUOP2", CS_SIGNAL_ALUOP2),
    DEFINE_SIGNAL("ALUOP3", CS_SIGNAL_ALUOP3),
    DEFINE_SIGNAL("WREG", CS_SIGNAL_WREG)
};

m_comp_siggrid m_comp_siggrid_create(void) {
    m_comp_siggrid siggrid = { 0 };
    Ihandle *name_label = { 0 };
    Ihandle *value_label = { 0 };
    size_t i = 0;

    siggrid.handler = IupGridBox(0);
    IupSetAttribute(siggrid.handler, "EXPANDCHILDREN", IUP_HORIZONTAL);
    IupSetAttribute(siggrid.handler, IUP_MARGIN, "3x0");
    IupSetInt(siggrid.handler, "NUMDIV", NUM_COLS);
    IupSetInt(siggrid.handler, "GAPLIN", 5);
    IupSetInt(siggrid.handler, "GAPCOL", 15);
    IupSetInt(siggrid.handler, "SIZECOL", -1);
    IupSetInt(siggrid.handler, "SIZELIN", -1);

    for (i = 0; i < M_COMP_SIGGRID_SIGNAL_LENGTH; i++) {
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
    int idx = 0;
    int i = 0;

    if (!siggrid) {
        return;
    }

    gridbox = m_comp_siggrid_get_handler(siggrid);
    for (i = 0; i < M_COMP_SIGGRID_SIGNAL_LENGTH; i++) {
        idx = GET_ROW_IDX(i);
        if (sigs[i].name) {
            IupSetStrf(IupGetChild(gridbox, idx), IUP_TITLE, BOOLEAN_FORMAT, !!(signals & sigs[i].bitmask));
        }
    }
}

void m_comp_siggrid_clear(m_comp_siggrid *siggrid) {
    Ihandle *gridbox = { 0 };
    int idx = 0;
    int i = 0;

    if (!siggrid) {
        return;
    }

    gridbox = m_comp_siggrid_get_handler(siggrid);
    for (i = 0; i < M_COMP_SIGGRID_SIGNAL_LENGTH; i++) {
        idx = GET_ROW_IDX(i);
        if (sigs[i].name) {
            IupSetStrf(IupGetChild(gridbox, idx), IUP_TITLE, BOOLEAN_FORMAT, 0);
        }
    }
}
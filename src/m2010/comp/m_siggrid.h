/** @file m_siggrid.h */

#ifndef M_COMP_SIGGRID_H
#define M_COMP_SIGGRID_H

#include <iup/iup.h>

enum m_comp_siggrid_signal_idx {
    M_COMP_SIGGRID_SIGNAL_WMAR = 0,
    M_COMP_SIGGRID_SIGNAL_WMDR,
    M_COMP_SIGGRID_SIGNAL_IOMDR,
    M_COMP_SIGGRID_SIGNAL_WMEM,
    M_COMP_SIGGRID_SIGNAL_RMEM,
    /* Space = 5 */
    M_COMP_SIGGRID_SIGNAL_WIR = 6,
    M_COMP_SIGGRID_SIGNAL_WPC,
    M_COMP_SIGGRID_SIGNAL_RPC,
    M_COMP_SIGGRID_SIGNAL_CPC,
    M_COMP_SIGGRID_SIGNAL_IPC,
    /* Space = 11 */
    M_COMP_SIGGRID_SIGNAL_ISP = 12,
    M_COMP_SIGGRID_SIGNAL_DSP,
    M_COMP_SIGGRID_SIGNAL_CSP,
    M_COMP_SIGGRID_SIGNAL_RSP,
    /* Space = 16 */
    M_COMP_SIGGRID_SIGNAL_INM = 17,
    M_COMP_SIGGRID_SIGNAL_SRW,
    /* Space = 19 */
    M_COMP_SIGGRID_SIGNAL_WAC = 20,
    M_COMP_SIGGRID_SIGNAL_RAC,
    M_COMP_SIGGRID_SIGNAL_ALUOP0,
    M_COMP_SIGGRID_SIGNAL_ALUOP1,
    M_COMP_SIGGRID_SIGNAL_ALUOP2,
    M_COMP_SIGGRID_SIGNAL_ALUOP3,
    M_COMP_SIGGRID_SIGNAL_WREG,
    M_COMP_SIGGRID_SIGNAL_LENGTH
};
typedef enum m_comp_siggrid_signal_idx m_comp_siggrid_signal_idx;

struct m_comp_siggrid_signal {
    char *name;
    unsigned long bitmask;
};
typedef struct m_comp_siggrid_signal m_comp_siggrid_signal;

struct m_comp_siggrid {
    Ihandle *handler;
};
typedef struct m_comp_siggrid m_comp_siggrid;

/**
 * @brief Creates a new component
 * @return A created and initiated component
*/
m_comp_siggrid m_comp_siggrid_create(void);

/**
 * @brief Gets the IUP handler of the component
 * @param siggrid Pointer to the component
 * @return Ihandler of the IUP component
*/
Ihandle *m_comp_siggrid_get_handler(m_comp_siggrid *siggrid);

/**
 * @brief Sets the given signal
 * @param siggrid Pointer to the component
 * @param signal Signal to be set
 * @param value Value to be set. Will be evaluated as boolean.
*/
void m_comp_siggrid_set(m_comp_siggrid *siggrid, m_comp_siggrid_signal_idx signal, int value);

/**
 * @brief Sets all the signals from a given 32-bit word
 * @param siggrid Pointer to the component
 * @param signals Signals to be set
*/
void m_comp_siggrid_set_signals(m_comp_siggrid *siggrid, unsigned long signals);

/**
 * @brief Sets all the signals to zero
 * @param siggrid Pointer to the component
*/
void m_comp_siggrid_clear(m_comp_siggrid *siggrid);

#endif /* M_COMP_SIGGRID_H */
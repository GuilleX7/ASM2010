/** @file m_reggrid.h */

#ifndef M_COMP_REGGRID_H
#define M_COMP_REGGRID_H

#include <iup/iup.h>

enum m_comp_reggrid_reg_idx {
    M_COMP_REGGRID_R0 = 0,
    M_COMP_REGGRID_R1,
    M_COMP_REGGRID_R2,
    M_COMP_REGGRID_R3,
    M_COMP_REGGRID_R4,
    M_COMP_REGGRID_R5,
    M_COMP_REGGRID_R6,
    M_COMP_REGGRID_R7,
    /* Bar 8 */
    M_COMP_REGGRID_PC = 9,
    M_COMP_REGGRID_SP,
    M_COMP_REGGRID_IR,
    /* Bar = 12 */
    M_COMP_REGGRID_AC = 13,
    M_COMP_REGGRID_SR_V,
    M_COMP_REGGRID_SR_N,
    M_COMP_REGGRID_SR_Z,
    M_COMP_REGGRID_SR_C,
    /* Bar = 18 */
    M_COMP_REGGRID_MDR = 19,
    M_COMP_REGGRID_MAR,
    M_COMP_REGGRID_LENGTH
};
typedef enum m_comp_reggrid_reg_idx m_comp_reggrid_reg_idx;

struct m_comp_reggrid_reg {
    char *name;
    char *format;
};
typedef struct m_comp_reggrid_reg m_comp_reggrid_reg;

struct m_comp_reggrid {
    Ihandle *handler;
};
typedef struct m_comp_reggrid m_comp_reggrid;

/**
 * @brief Creates a new component
 * @return A created and initiated component
*/
m_comp_reggrid m_comp_reggrid_create(void);

/**
 * @brief Gets the IUP handler of the component
 * @param reggrid Pointer to the component
 * @return Ihandler of the IUP component
*/
Ihandle *m_comp_reggrid_get_handler(m_comp_reggrid *reggrid);

/**
 * @brief Sets a given value for a register
 * @param reggrid Pointer to the component
 * @param idx Index of the register
 * @param value Value to be set
*/
void m_comp_reggrid_set(m_comp_reggrid *reggrid, m_comp_reggrid_reg_idx reg, size_t value);

/**
 * @brief Clears all the registers
 * @param reggrid Pointer to the component
*/
void m_comp_reggrid_clear(m_comp_reggrid *reggrid);

#endif /* M_COMP_REGGRID_H */
/** @file m_ramgrid.h */

#ifndef M_RAMGRID_H
#define M_RAMGRID_H

#include <iup/iup.h>

struct m_comp_ramgrid {
    Ihandle *handler;
};
typedef struct m_comp_ramgrid m_comp_ramgrid;

/**
 * @brief Creates a new component
 * @param size Size of the RAM in bytes
 * @return A created and initiated component
*/
m_comp_ramgrid m_comp_ramgrid_create(void);

/**
 * @brief Gets the IUP handler of the component
 * @param ramgrid Pointer to the component
 * @return Ihandler of the IUP component
*/
Ihandle *m_comp_ramgrid_get_handler(m_comp_ramgrid *ramgrid);

/**
 * @brief Sets a given row of the RAM
 * @param ramgrid Pointer to the component
 * @param address Address of the row to be set (NUM_COLS aligned)
 * @param value Values to be set (NUM_COLS bytes)
*/
void m_comp_ramgrid_set_row(m_comp_ramgrid *ramgrid, int address, uint8_t *values);

/**
 * @brief Sets the entire RAM
 * @param ramgrid Pointer to the component
 * @param values Values to be set (RAM_SIZE bytes)
*/
void m_comp_ramgrid_set_all(m_comp_ramgrid *ramgrid, uint8_t *values);

/**
 * @brief Sets the entire memory to 0
 * @param ramgrid Pointer to the component
*/
void m_comp_ramgrid_clear(m_comp_ramgrid *ramgrid);

#endif /* M_RAMGRID_H */
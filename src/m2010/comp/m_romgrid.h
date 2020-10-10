/** @file m_romgrid.h */

#ifndef M_ROMGRID_H
#define M_ROMGRID_H

#include <stdint.h>
#include <stdbool.h>

#include <iup/iup.h>

struct m_comp_romgrid {
    Ihandle *handler;
    bool any_row_active;
    int active_row;
};
typedef struct m_comp_romgrid m_comp_romgrid;

/**
 * @brief Creates a new component
 * @return A created and initiated component
*/
m_comp_romgrid m_comp_romgrid_create();

/**
 * @brief Gets the IUP handler of the component
 * @param romgrid Pointer to the component
 * @return Ihandler of the IUP component
*/
Ihandle *m_comp_romgrid_get_handler(m_comp_romgrid *romgrid);

/**
 * @brief Clears the content of the ROM grid
 * @param romgrid Pointer to the component
*/
void m_comp_romgrid_clear(m_comp_romgrid *romgrid);

/**
 * @brief Puts a row of machine code at the given position
 * @param romgrid Pointer to the component
 * @param index Where to put the machine code line
 * @param sentence Machine code sentence
 * @param dissasembly Dissasembly of the sentence (0 if none)
*/
void m_comp_romgrid_put_at(m_comp_romgrid *romgrid, int index, uint16_t sentence, char *dissasembly);

/**
 * @brief Sets the given row as active
 * @param romgrid Pointer to the component
 * @param index Index of the row to be set as active
*/
void m_comp_romgrid_set_active(m_comp_romgrid *romgrid, int index);

/**
 * @brief Clears any active row
 * @param romgrid Pointer to the component
*/
void m_comp_romgrid_clear_active(m_comp_romgrid *romgrid);

#endif /* M_ROMGRID_H */
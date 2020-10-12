/** @file cs2010.h */

#ifndef CS2010_H
#define CS2010_H

#include <stdbool.h>
#include <stddef.h>

#include "cs_memory.h"
#include "cs_registers.h"

#define CS_CLEAR_RAM (1u << 0)
#define CS_CLEAR_ROM (1u << 1)

#define CS_SUCCESS 0
#define CS_FAILED 1
#define CS_NOT_ENOUGH_ROM 2
#define CS_INVALID_INSTRUCTIONS 3

struct cs2010 {
    cs_memory mem;
    cs_registers reg;
    unsigned char microop;
    bool stopped;
    size_t last_ram_change_address;
};
typedef struct cs2010 cs2010;

/**
 * @brief Initiates a new CS2010 computer
 * @param cs Pointer to CS2010 structure
 * @return true if success, false otherwise
*/
bool cs_init(cs2010 *cs);

/**
 * @brief Hard-resets the CS2010
 *      This includes clearing all the registers and
 *      memories, bringing the machine state to the
 *      beginning.
 * @param cs Pointer to CS2010 structure
*/
void cs_hard_reset(cs2010 *cs);

/**
 * @brief Soft-resets the CS2010
 *      Doesn't clear memories, just clear PC and SP
 * @param cs Pointer to CS2010 structure
*/
void cs_soft_reset(cs2010 *cs);

/**
 * @brief Clears the given memories
 * @param cs Pointer to CS2010 structure
 * @param flags Valid bitmasks are CS_CLEAR_RAM, CS_CLEAR_ROM
*/
void cs_clear_memory(cs2010 *cs, unsigned char flags);

/**
 * @brief Resets all the registers and signals
 * @param cs Pointer to CS2010 structure
*/
void cs_reset_registers(cs2010 *cs);

/**
 * @brief Loads and checks a given machine code for valid instructions.
 * @param cs Pointer to CS2010 structure
 * @param sentences Pointer to machine code
 * @param sentences_length Size of machine code
 * @return CS_SUCCESS if success,
 *          CS_NOT_ENOUGH_ROM if machine code is too large,
 *          CS_INVALID_INSTRUCTIONS if machine code isn't valid CS2010
 *          machine code or
 *          CS_FAILED otherwise
*/
int cs_load_and_check(cs2010 *cs, unsigned short *sentences, size_t sentences_length);

/**
 * @brief Performs an increment of the microop counter
 *       and fetchs the matching flags
 * @param cs Pointer to CS2010 structure
*/
void cs_microfetch(cs2010 *cs);

/**
 * @brief Performs a full instruction fetch, resetting
 *      the microop counter
 * @param cs Pointer to CS2010 structure
*/
void cs_fetch(cs2010 *cs);

/**
 * @brief Performs a microstep, executing the current
 *      microoperation, and fetching the next instruction
 *      if needed
 * @param cs Pointer to CS2010 structure 
*/
void cs_microstep(cs2010 *cs);

/**
 * @brief Performs a step, executing the current instruction
 *      and fetching the next
 * @param cs Pointer to CS2010 structure
*/
void cs_step(cs2010 *cs);

/**
 * @brief Performs a fullstep, which means:
 *      - If the machine state is in the middle of a
 *        microoperation, it will finish the remaining
 *        ones
 *      - Otherwise, this is the same as calling cs_step
 * @param cs Pointer to CS2010 structure
*/
void cs_fullstep(cs2010 *cs);

/**
 * @brief Performs a blockstep, which means executing
 *      full instructions until a BRxx/JMP instruction
 *      is found
 * @param cs Pointer to CS2010 structure
*/
void cs_blockstep(cs2010 *cs);

/**
 * @brief Frees a given CS2010 structure
 * @param cs Pointer to CS2010 structure
*/
void cs_free(cs2010 *cs);

#endif /* CS2010_H */
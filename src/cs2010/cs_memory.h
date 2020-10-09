/** @file cs_memory.h */

#ifndef CS_MEMORY_H
#define CS_MEMORY_H

#include <stdint.h>

#define CS_ROM_SIZE 256
#define CS_RAM_SIZE 256

struct cs_memory {
    uint8_t *rom;
    uint8_t *ram;
};
typedef struct cs_memory cs_memory;

#endif /* CS_MEMORY_H */
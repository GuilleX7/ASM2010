/** @file cs_io.c */

#include <stddef.h>

#include "../../include/asm2010.h"

#include "../utils.h"

unsigned short cs_io_read_stub(unsigned char address) {
    (void)address;
    return CS_IO_READ_NOT_CONTROLLED;
}

unsigned char cs_io_write_stub(unsigned char address, unsigned char content) {
    (void)address;
    (void)content;
    return CS_IO_WRITE_NOT_CONTROLLED;
}

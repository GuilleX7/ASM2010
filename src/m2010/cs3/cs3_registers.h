/** @file cs3_registers.h */

#ifndef CS3_REGISTERS_H
#define CS3_REGISTERS_H

#define CS3_REGISTERS_INDIRECT_RMIN 6
#define CS3_REGISTERS_INDIRECT_RMAX 7

#define CS3_REGISTER_INDIRECT_R6_NAME 'Y'
#define CS3_REGISTER_INDIRECT_R7_NAME 'Z'

#define CS3_REGISTER_GET_INDIRECT_NAME(register_idx)                                                                   \
    (register_idx == 6 ? CS3_REGISTER_INDIRECT_R6_NAME : CS3_REGISTER_INDIRECT_R7_NAME)

#endif /* CS3_REGISTERS_H */

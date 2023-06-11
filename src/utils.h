/** @file utils.h */

#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#ifndef bool
#define bool  unsigned char
#define true  1
#define false 0
#endif

#define STRGIFY(a)   #a
#define STRINGIFY(a) STRGIFY(a)

#define BIT_AT(a, n) (!!(a & (1u << n)))

#ifdef _WIN32
#ifdef _WIN64
#define PRI_SIZET PRIu64
#else
#define PRI_SIZET PRIu32
#endif /* _WIN64 */
#else
#define PRI_SIZET "zu"
#endif /* _WIN32 */

#define BOOLEAN_FORMAT "%u"
#define HEX4_FORMAT    "%01X"
#define HEX8_FORMAT    "%02X"
#define HEX16_FORMAT   "%04X"
#define HEX32_FORMAT   "%08X"
#define HEX64_FORMAT   "%016X"
#define HEX_PREFIX     "0x"
#define HEX4_X_FORMAT  HEX_PREFIX HEX4_FORMAT
#define HEX8_X_FORMAT  HEX_PREFIX HEX8_FORMAT
#define HEX16_X_FORMAT HEX_PREFIX HEX16_FORMAT
#define HEX32_X_FORMAT HEX_PREFIX HEX32_FORMAT
#define HEX64_X_FORMAT HEX64_FORMAT

#endif /* UTILS_H */

/** @file cs_platforms.h */

#ifndef CS_PLATFORMS_H
#define CS_PLATFORMS_H

#define CS_PLATFORM_ALL (CS_PLATFORM_2010 | CS_PLATFORM_3)

#define CS_PLATFORM_IS_AVAILABLE(platforms, platform) (platforms & platform)
#define CS_PLATFORM_IS_VALID(platform)                (platform == CS_PLATFORM_2010 || platform == CS_PLATFORM_3)

typedef unsigned char cs_platform;

#endif /* CS_PLATFORMS_H */

#pragma once

#define SCARLETT_ARCH_UNKNOWN				"unknown"
#define SCARLETT_ARCH_PCC					"pcc"
#define SCARLETT_ARCH_PCC64					"pcc64"
#define SCARLETT_ARCH_i386					"i386"
#define SCARLETT_ARCH_x86_64				"x86_64"

#if defined(arch_ppc)
#define SCARLETT_ARCH SCARLETT_ARCH_PCC
#elif defined(arch_i386)
#define SCARLETT_ARCH SCARLETT_ARCH_i386
#elif defined(arch_x86_64)
#define SCARLETT_ARCH SCARLETT_ARCH_x86_64
#elif defined(arch_ppc64)
#define SCARLETT_ARCH SCARLETT_ARCH_PCC64
#else
#define SCARLETT_ARCH SCARLETT_ARCH_UNKNOWN
#endif
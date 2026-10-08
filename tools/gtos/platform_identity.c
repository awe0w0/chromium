// Copyright 2026 The Chromium Authors
// Use of this source code is governed by a BSD-style license in LICENSE.
#include "build/build_config.h"

_Static_assert(BUILDFLAG(IS_GTOS), "Expected GTOS identity");
_Static_assert(!BUILDFLAG(IS_LINUX), "GTOS must not inherit Linux identity");
_Static_assert(!BUILDFLAG(IS_POSIX), "GTOS has no qualified POSIX runtime yet");
_Static_assert(!BUILDFLAG(IS_ANDROID) && !BUILDFLAG(IS_CHROMEOS) &&
               !BUILDFLAG(IS_WIN) && !BUILDFLAG(IS_FUCHSIA), "Exclusive OS");
#if defined(__linux__) || defined(LIBC_GLIBC)
#error The GTOS compiler must not target Linux or inherit glibc
#endif

__attribute__((used)) const char gtos_platform_identity[] =
    "GTOS;IS_GTOS=1;IS_LINUX=0;IS_POSIX=0;role=platform-qualification";

#if defined(ARCH_CPU_X86)
typedef unsigned int u32;
static int call(u32 number, u32 first, u32 second) {
  int result;
  __asm__ volatile("int $0x80" : "=a"(result)
                   : "a"(number), "b"(first), "c"(second) : "memory", "cc");
  return result;
}
int platform_identity_main(void) {
  static const char pass[] = "GTOS BUILD PLATFORM PASS IS_GTOS=1 IS_LINUX=0 ABI1\n";
  if (call(0x4700, 0, 0) != 1) return 10;
  if (call(0x4701, (u32)pass, sizeof(pass)-1) != sizeof(pass)-1) return 11;
  return 0;
}
#endif

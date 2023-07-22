/*
 * Copyright (c) 2023 Gembera Team
 * https://github.com/gembera
 * License: MIT
 */

#ifndef __G_SYS_H__
#define __G_SYS_H__
#include "glib.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MINGW32__)
#define G_WIN 1
#endif

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include <unistd.h>
#define G_POSIX _POSIX_VERSION
#endif

typedef gpointer ghandle;

#endif
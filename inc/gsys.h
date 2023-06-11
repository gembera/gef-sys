/*
 * Copyright (c) 2023 Gembera Team
 * https://github.com/gembera
 * License: MIT
 */

#ifndef __G_SYS_H__
#define __G_SYS_H__
#include "glib.h"

#ifdef _WIN32
int gettimeofday(struct timeval *tv, void *not_used);
#else
#include <sys/time.h>
#include <unistd.h>
#endif

guint64 g_tick();
void g_sleep(guint64 time_ms);
void g_usleep(guint64 time_us);

#endif
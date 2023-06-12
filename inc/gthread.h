/*
 * Copyright (c) 2023 Gembera Team
 * https://github.com/gembera
 * License: MIT
 */

#ifndef __G_THREAD_H__
#define __G_THREAD_H__
#include "glib.h"

typedef struct {
  ghandle handle;
} GMutex;

GMutex *g_mutex_new();
void g_mutex_free(GMutex *self);
void g_mutex_lock(GMutex *self);
void g_mutex_unlock(GMutex *self);

typedef struct _GThread GThread;
typedef void (*GThreadCallback)(GThread *self);

struct _GThread {
  ghandle handle;
  gbool force_stop;
  GThreadCallback callback;
  gpointer user_data;
  GFreeCallback user_data_free_callback;
};

GThread *g_thread_new(GThreadCallback callback, gpointer user_data,
                      GFreeCallback user_data_free_callback);
void g_thread_free(GThread *self);
void g_thread_start(GThread *self);
void g_thread_stop(GThread *self);
void g_thread_join(GThread *self);

#endif
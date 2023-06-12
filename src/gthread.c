#include "gthread.h"

#include "pthread.h"

static void *_thread_callback(void *arg) {
  GThread *self = (GThread *)arg;
  g_return_val_if_fail(self && self->callback, NULL);
  self->callback(self);
}

GThread *g_thread_new(GThreadCallback callback, gpointer user_data,
                      GFreeCallback user_data_free_callback) {
  GThread *self = g_new(GThread);
  g_return_val_if_fail(self && callback, NULL);
  self->handle = NULL;
  self->callback = callback;
  self->user_data = user_data;
  self->user_data_free_callback = user_data_free_callback;
  return self;
}

void g_thread_free(GThread *self) {
  g_return_if_fail(self);
  if (self->user_data && self->user_data_free_callback) {
    self->user_data_free_callback(self->user_data);
  }
  if (self->handle)
    g_free(self->handle);
  g_free(self);
}

void g_thread_start(GThread *self) {
  g_return_if_fail(self && !self->handle);
  pthread_t *handle = g_new(pthread_t);
  g_return_if_fail(handle);
  self->handle = handle;
  pthread_create(handle, NULL, _thread_callback, self);
}

void g_thread_stop(GThread *self) {
  g_return_if_fail(self && self->handle);
  self->force_stop = TRUE;
}

void g_thread_join(GThread *self) {
  g_return_if_fail(self && self->handle);
  pthread_t *handle = (pthread_t *)self->handle;
  pthread_join(*handle, NULL);
}

GMutex *g_mutex_new() {
  GMutex *self = g_new(GMutex);
  g_return_val_if_fail(self, NULL);
  pthread_mutex_t *handle = g_new(pthread_mutex_t);
  self->handle = handle;
  pthread_mutex_init(handle, NULL);
  return self;
}

void g_mutex_free(GMutex *self) {
  g_return_if_fail(self && self->handle);
  pthread_mutex_t *handle = (pthread_mutex_t *)self->handle;
  pthread_mutex_destroy(handle);
  g_free(self->handle);
  g_free(self);
}

void g_mutex_lock(GMutex *self) {
  g_return_if_fail(self && self->handle);
  pthread_mutex_t *handle = (pthread_mutex_t *)self->handle;
  pthread_mutex_lock(handle);
}

void g_mutex_unlock(GMutex *self) {
  g_return_if_fail(self && self->handle);
  pthread_mutex_t *handle = (pthread_mutex_t *)self->handle;
  pthread_mutex_unlock(handle);
}
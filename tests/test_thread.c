#include "glib.h"
#include "gsys.h"
#include "gthread.h"
#include <assert.h>

#define WORK_SIZE 500000
#define WORKER_COUNT 10

void worker_callback(GThread *self) {
  GMap *local = (GMap *)self->user_data;
  GMutex *mutex = g_map_get(local, "mutex");
  int *total = g_map_get(local, "total");
  int i = 0;
  for (i = 0; i < WORK_SIZE; ++i) {
    g_mutex_lock(mutex);
    (*total)++;
    g_mutex_unlock(mutex);
  }
}

void master_callback(GThread *self) {
  GMap *local = (GMap *)self->user_data;
  GThread **workers = (GThread **)g_map_get(local, "workers");
  int i;
  for (i = 0; i < WORKER_COUNT; ++i) {
    g_thread_join(workers[i]);
  }
}
void free_workers(gpointer ptr) {
  GThread **workers = (GThread **)ptr;
  int i;
  for (i = 0; i < WORKER_COUNT; ++i) {
    g_thread_free(workers[i]);
  }
}
int test_thread(int argc, const char *argv[]) {

  g_mem_record(g_mem_record_default_callback);
  g_mem_record_begin();

  int total = 0;
  GThread *workers[WORKER_COUNT];
  GMutex *mutex = g_mutex_new();
  setbuf(stdout, NULL);
  guint64 st = g_tick();
  GMap *master_data = g_map_new(NULL);
  g_map_set_with(master_data, "workers", workers, free_workers);
  GThread *master =
      g_thread_new(master_callback, master_data, (GFreeCallback)g_map_free);
  int i = 0;
  for (i = 0; i < WORKER_COUNT; ++i) {
    GMap *worker_data = g_map_new(NULL);
    g_map_set(worker_data, "mutex", mutex);
    g_map_set(worker_data, "total", &total);
    workers[i] =
        g_thread_new(worker_callback, worker_data, (GFreeCallback)g_map_free);
    g_thread_start(workers[i]);
  }
  g_thread_start(master);
  g_thread_join(master);
  guint64 en = g_tick();
  g_mutex_free(mutex);
  g_thread_free(master);
  printf("\r\nTotal: %d (%ldms)", total, en - st);
  assert(total == WORKER_COUNT * WORK_SIZE);

  gulong allocated = 0;
  gulong freed = 0;
  gulong peak = 0;
  g_mem_profile(&allocated, &freed, &peak);
  g_mem_record_end();
  printf("\r\nallocated memory: %ld  \r\nfreed memory: %ld  \r\npeak memory: "
         "%ld\r\n",
         allocated, freed, peak);
  assert(allocated == freed);
  return 0;
}
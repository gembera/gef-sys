#include "gsys.h"
#include "gfilestream.h"
#include <assert.h>

int test_filestream(int argc, char *argv[]) {
  g_log_init(INFO, NULL, NULL);
  g_mem_record(g_mem_record_default_callback);
  g_mem_record_begin();

  gcstr hello = "hello ";
  gcstr gef = "gef";
  GFileStream *sfile = g_object_new(GFileStream);
  g_file_stream_open(sfile, "/tmp/hello.txt", FILE_MODE_CREATE_NEW);
  g_stream_write_string(sfile, hello);
  g_stream_write_string(sfile, gef);
  g_stream_seek(sfile, 0, SEEK_BEGIN);
  glong len;
  gstr buffer;
  g_stream_read_all_content(sfile, &buffer, &len, TRUE);
  assert(g_equal(buffer, "hello gef"));
  g_free(buffer);
  g_object_free(sfile);

  g_auto_free();

  gulong allocated = 0;
  gulong freed = 0;
  gulong peak = 0;
  g_mem_profile(&allocated, &freed, &peak);
  g_mem_record_end();
  printf("\r\nallocated memory: %ld  \r\nfreed memory: %ld  \r\npeak memory: %ld\r\n",
         allocated, freed, peak);
  assert(allocated == freed);
  return 0;
}

#include "gfilestream.h"

void g_file_stream_open(gpointer self, gcstr name, GFileMode mode) {
  GFileStream *s = GFILESTREAM(self);
  s->file_name = g_dup(name);
  s->file_handle = (ghandle)fopen(
      name, mode == FILE_MODE_READ_ONLY
                ? "rb"
                : (mode == FILE_MODE_READ_WRITE ? "r+b" : "w+b"));
}
gbool g_file_stream_set_length(gpointer self, glong len) {}
glong g_file_stream_get_length(gpointer self) {
  GFileStream *s = GFILESTREAM(self);
  glong len, pos;
  pos = g_file_stream_get_position(self);
  fseek((FILE *)s->file_handle, 0, 2);
  len = g_file_stream_get_position(self);
  g_file_stream_seek(self, pos, SEEK_BEGIN);
  return len;
}
glong g_file_stream_get_position(gpointer self) {
  GFileStream *s = GFILESTREAM(self);
  return (glong)ftell((FILE *)s->file_handle);
}
glong g_file_stream_write(gpointer self, gcstr buffer, glong length) {
  glong written = 0;
  GFileStream *s = GFILESTREAM(self);
  written = fwrite(buffer, 1, length, (FILE *)s->file_handle);
  return (glong)written;
}
glong g_file_stream_read(gpointer self, gstr buffer, glong length) {
  glong nread = 0;
  GFileStream *s = GFILESTREAM(self);
  nread = fread(buffer, 1, length, (FILE *)s->file_handle);
  return (glong)nread;
}
void g_file_stream_seek(gpointer self, glong offset, GSeekOrigin origin) {
  GFileStream *s = GFILESTREAM(self);
  int r;
  switch (origin) {
  case SEEK_BEGIN:
    r = fseek((FILE *)s->file_handle, offset, 0);
    break;
  case SEEK_CURRENT:
    r = fseek((FILE *)s->file_handle, offset, 1);
    break;
  case SEEK_END:
    r = fseek((FILE *)s->file_handle, offset, 2);
    break;
  }
}
void g_file_stream_close(gpointer self) {
  GFileStream *s = GFILESTREAM(self);
  if (s->file_handle) {
    g_free(s->file_name);
    fflush((FILE *)s->file_handle);
    fclose((FILE *)s->file_handle);
    s->file_handle = NULL;
    s->file_name = NULL;
  }
}
void g_file_stream_flush(gpointer self) {
  GFileStream *s = GFILESTREAM(self);
  fflush((FILE *)s->file_handle);
}

void g_file_stream_init(gpointer self) {
  g_stream_init(self);
  GFILESTREAM(self)->file_handle = NULL;
  GFILESTREAM(self)->file_name = NULL;
}
void g_file_stream_finalize(gpointer self) {
  g_file_stream_close(self);
  g_stream_finalize(self);
}
void g_file_stream_class_init(gpointer c) {
  g_stream_class_init(c);
  GSTREAMCLASS(c)->get_length = g_file_stream_get_length;
  GSTREAMCLASS(c)->set_length = g_file_stream_set_length;
  GSTREAMCLASS(c)->get_position = g_file_stream_get_position;
  GSTREAMCLASS(c)->read = g_file_stream_read;
  GSTREAMCLASS(c)->write = g_file_stream_write;
  GSTREAMCLASS(c)->seek = g_file_stream_seek;
  GSTREAMCLASS(c)->flush = g_file_stream_flush;
}

gpointer g_class_GFileStream() {
  gpointer clazz = g_class(CLASS_NAME(GFileStream));
  return clazz != NULL
             ? clazz
             : REGISTER_CLASS(GFileStream, GStream, g_file_stream_init,
                              g_file_stream_finalize, g_file_stream_class_init,
                              NULL);
}
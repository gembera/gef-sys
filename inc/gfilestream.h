#ifndef _G_FILE_STREAM_H_
#define _G_FILE_STREAM_H_
#include "gstream.h"

#ifdef __cplusplus
extern "C" {
#pragma }
#endif /* __cplusplus */

#define GFILESTREAM(p) ((GFileStream *)(p))
#define GFILESTREAMCLASS(p) ((GFileStreamClass *)(p))
typedef struct _GFileStream GFileStream;
typedef struct _GFileStreamClass GFileStreamClass;
typedef enum
{
	FILE_MODE_READ_ONLY,
	FILE_MODE_READ_WRITE,
	FILE_MODE_CREATE_NEW
}GFileMode;
struct _GFileStream
{
	GStream base;
	ghandle file_handle;
	gstr file_name;
};
struct _GFileStreamClass
{
	GStreamClass base;
};

void g_file_stream_open(gpointer self, gcstr name, GFileMode mode);
void g_file_stream_close(gpointer self);
gbool g_file_stream_set_length(gpointer self, glong len);
glong g_file_stream_get_length(gpointer self);
glong g_file_stream_get_position(gpointer self);
glong g_file_stream_write(gpointer self, gcstr buffer, glong length);
glong g_file_stream_read(gpointer self, gstr buffer, glong length);
void g_file_stream_seek(gpointer self, glong offset, GSeekOrigin origin);
void g_file_stream_flush(gpointer self);
gbool g_file_stream_is_open(gpointer self);

gpointer g_class_GFileStream(void);

void g_file_stream_init(gpointer self);
void g_file_stream_finalize(gpointer self);
void g_file_stream_class_init(gpointer clazz);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
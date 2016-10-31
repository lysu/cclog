#ifndef CCLOG_CCFILE_H
#define CCLOG_CCFILE_H

#include <stdio.h>

struct cc_file_appender_s {
    FILE *fp;
    char buffer[64*1024];
    size_t written_bytes;
};

typedef struct cc_file_appender_s cc_file_appender_t;

cc_file_appender_t *cc_file_create(char *filename);

void cc_file_append(cc_file_appender_t *a, char* logline, size_t len);

void cc_file_appender_flush(cc_file_appender_t *a);

static inline size_t cc_file_appender_written_bytes(cc_file_appender_t *a) {
    return a->written_bytes;
}

#endif //CCLOG_CCFILE_H

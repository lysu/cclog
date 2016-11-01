#include <stdlib.h>
#include <stdio.h>
#include "ccfile.h"

cc_file_appender_t *cc_file_create(char *filename) {
    cc_file_appender_t *a = calloc(1, sizeof(cc_file_appender_t));
    a->fp = fopen(filename, "ae");
    a->written_bytes = 0;
    return a;
}

static inline size_t cc_write(FILE *fp, char* logline, size_t len) {
    return fwrite(logline, sizeof(char), len, fp);
}

void cc_file_append(cc_file_appender_t *a, char* logline, size_t len) {
    if (len == 0) {
        return;
    }
    printf("write one \n");
    size_t n = cc_write(a->fp, logline, len);
    size_t remain = len - n;
    while (remain > 0) {
        size_t x = cc_write(a->fp, logline + n, remain);
        if (x == 0) {
            int err = ferror(a->fp);
            if (err) {
                fprintf(stderr, "AppendFile::append() failed\n");
            }
            break;
        }
        n += x;
        remain = len - n; // remain -= x
    }
    a->written_bytes += len;
}

void cc_file_appender_flush(cc_file_appender_t *a) {
    int status = fflush(a->fp);
    if (status != 0) {
        printf("fflush failure.....\n");
    }
}

#ifndef CCLOG_CC_FILE_H
#define CCLOG_CC_FILE_H

#include <stddef.h>
#include <stdlib.h>

struct cc_logfile_s {
    char *basename;
    size_t roll_size;
    int flush_interval;

};

typedef struct cc_logfile_s cc_logfile_t;

cc_logfile_t *create_logfile(char *basename, size_t roll_size, int flush_interval) {
    cc_logfile_t *l = calloc(1, sizeof(cc_logfile_t));
    l->basename = basename;
    l->flush_interval = flush_interval;
    l->roll_size = roll_size;
    return l;
}

#endif //CCLOG_CC_FILE_H

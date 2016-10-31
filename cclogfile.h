#ifndef CCLOG_CC_FILE_H
#define CCLOG_CC_FILE_H

#include <stddef.h>
#include <stdlib.h>
#include "ccfile.h"

struct cc_logfile_s {
    char *basename;
    size_t roll_size;
    int flush_interval;
    cc_file_appender_t *appender;
    int count;
    int check_every_n;
    time_t start_of_period;
    time_t last_flush;
    time_t last_roll;
};

typedef struct cc_logfile_s cc_logfile_t;

cc_logfile_t *cc_logfile_create(char *basename, size_t roll_size, int flush_interval);

void cc_logfile_append(cc_logfile_t *logfile, char *logline, size_t len);

void cc_logfile_flush(cc_logfile_t *logfile);

int cc_logfile_roll_file(cc_logfile_t *logfile);


#endif //CCLOG_CC_FILE_H

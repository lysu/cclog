#ifndef CCLOG_CCLOG_H
#define CCLOG_CCLOG_H

#include <glib.h>

#include "ccbuffer.h"
#include "pthread.h"

struct cclogger_s {
    int flush_interval;
    char *basename;
    size_t roll_size;

    pthread_t thread;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    pthread_cond_t start_cond;
    pthread_mutex_t start_mutex;
    cc_buffer_t *current_buffer;
    cc_buffer_t *next_buffer;
    GList *buffs;

    int is_running;
};

typedef struct cclogger_s cclogger_t;

cclogger_t *logger_create(char *basename, int flush_interval, size_t roll_size);

void logger_start(cclogger_t *logger);

void logger_write(cclogger_t *logger, char *content, size_t cotent_len);

#endif //CCLOG_CCLOG_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "cclog.h"

static void *writer_func(void *targ);

cclogger_t *logger_create(char *basename, int flush_interval, size_t roll_size) {
    cclogger_t *logger = calloc(1, sizeof(cclogger_t));
    logger->basename = basename;
    logger->flush_interval = flush_interval;
    logger->roll_size = roll_size;
    logger->current_buffer = create_log_buffer();
    logger->next_buffer = create_log_buffer();
    logger->buffs = NULL;
    logger->is_running = 0;
    pthread_cond_init(&logger->cond, NULL);
    pthread_mutex_init(&logger->mutex, NULL);
    pthread_cond_init(&logger->start_cond, NULL);
    pthread_mutex_init(&logger->start_mutex, NULL);
    return logger;
}

void logger_start(cclogger_t *logger) {
    pthread_create(&logger->thread, NULL, writer_func, logger);
    pthread_mutex_lock(&logger->start_mutex);
    while (!logger->is_running) {
        pthread_cond_wait(&logger->start_cond, &logger->start_mutex);
    }
    pthread_mutex_unlock(&logger->start_mutex);
}

void logger_write(cclogger_t *logger, char *content, size_t len) {
    pthread_mutex_lock(&logger->mutex);
    if (buffer_avail(logger->current_buffer) > len) {
        buffer_append(logger->current_buffer, content, len);
    } else {
        logger->buffs = g_list_append(logger->buffs, logger->current_buffer);
        logger->current_buffer = NULL;
        if (logger->next_buffer != NULL) {
            logger->current_buffer = logger->next_buffer;
            logger->next_buffer = NULL;
        } else {
            logger->current_buffer = create_log_buffer();
        }
        buffer_append(logger->current_buffer, content, len);
        pthread_cond_signal(&logger->cond);
    }
    pthread_mutex_unlock(&logger->mutex);
}

static void *writer_func(void *targ) {
    cclogger_t *logger = targ;

    pthread_mutex_lock(&logger->start_mutex);
    logger->is_running = 1;
    pthread_cond_signal(&logger->start_cond);
    pthread_mutex_unlock(&logger->start_mutex);

    cc_buffer_t *new_buff1 = create_log_buffer();
    cc_buffer_t *new_buff2 = create_log_buffer();
    GList *buffer_to_write = NULL;

    while (logger->is_running) {
        pthread_mutex_lock(&logger->mutex);
        if (g_list_length(logger->buffs) == 0) {
            struct timespec ts;
            ts.tv_sec = logger->flush_interval;
            pthread_cond_timedwait(&logger->cond, &logger->mutex, &ts);
        }
        logger->buffs = g_list_append(logger->buffs, logger->current_buffer);
        logger->current_buffer = new_buff1;

        GList *tmp = logger->buffs;
        logger->buffs = buffer_to_write;
        buffer_to_write = tmp;

        if (logger->next_buffer != NULL) {
            logger->next_buffer = new_buff2;
            new_buff2 = NULL;
        }
        pthread_mutex_unlock(&logger->mutex);

        if (g_list_length(buffer_to_write) > 25) {
            char buf[256];
            snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
                     "2011",
                     g_list_length(buffer_to_write) - 2);
            fputs(buf, stderr);

            // TODO: replace me.
            printf("%s", buf);

            for (guint j = 0; j < g_list_length(buffer_to_write); j++) {
                if (j < 2) {
                    continue;
                }
                GList *to_erase = buffer_to_write;
                buffer_to_write = g_list_remove_link(buffer_to_write, to_erase);
                if (to_erase != NULL) {
                    buffer_free(to_erase->data);
                }
            }
        }

        for (guint i = 0; i < g_list_length(buffer_to_write); i++) {
            cc_buffer_t *buf = g_list_nth_data(buffer_to_write, i);

            fputs(buf->data, stdout);
        }

        GList *last = g_list_last(buffer_to_write);
        buffer_to_write = g_list_remove_link(buffer_to_write, last);
        new_buff1 = last->data;
        buffer_reset(new_buff1);

        if (new_buff2 == NULL) {
            last = g_list_last(buffer_to_write);
            if (last != NULL) {
                buffer_to_write = g_list_remove_link(buffer_to_write, last);
                new_buff2 = last->data;
                buffer_reset(new_buff2);
            }

        }
    }
    return NULL;
}

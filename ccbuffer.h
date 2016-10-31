#ifndef CCLOG_BUFFER_H
#define CCLOG_BUFFER_H

#include "stddef.h"
#include "string.h"

#define LOG_BUFF_SIZE 4000*1000

struct cc_buffer_s {
    char *data;
    size_t size;
    char *curr;
};

typedef struct cc_buffer_s cc_buffer_t;

cc_buffer_t *create_buffer(size_t size);

void clear_buffer(cc_buffer_t *buff);

size_t buffer_avail(cc_buffer_t *buffer);

void buffer_append(cc_buffer_t *buffer, char *data, size_t len);

static inline cc_buffer_t *create_log_buffer() {
    return create_buffer(LOG_BUFF_SIZE);
}

static inline void buffer_reset(cc_buffer_t *buffer) {
    memset(buffer->data, 0, buffer->size);
}

static inline char *buffer_end(cc_buffer_t *buffer) {
    return buffer->data + buffer->size;
}

#endif //CCLOG_BUFFER_H

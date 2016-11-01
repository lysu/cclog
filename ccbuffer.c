#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ccbuffer.h"

cc_buffer_t *create_buffer(char *id, size_t size) {
    cc_buffer_t *buf = calloc(1, sizeof(cc_buffer_t));
    buf->id = id;
    buf->data = malloc(size);
    buf->curr = buf->data;
    buf->size = size;
    buffer_reset(buf);
    return buf;
}

void buffer_free(cc_buffer_t *buff) {
    free(buff->data);
    free(buff);
}

size_t buffer_avail(cc_buffer_t *buffer) {
    return buffer_end(buffer) - buffer->data;
}

void buffer_append(cc_buffer_t *buffer, char *data, size_t len) {
    if (buffer_avail(buffer) > len) {
        memcpy(buffer->curr, data, len);
        buffer->curr += len;
    }
}

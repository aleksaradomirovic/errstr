/*
 * Copyright (C) 2025  Aleksa Radomirovic
 * SPDX-License-Identifier: Apache-2.0
 */

#include "errstr.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifndef ERRSTR_DEFAULT_BUF_SIZE
#   define ERRSTR_DEFAULT_BUF_SIZE (256)
#endif

#ifndef ERRSTR_MAX_BUF_SIZE
#   define ERRSTR_MAX_BUF_SIZE (BUFSIZ)
#endif

static_assert(ERRSTR_MAX_BUF_SIZE >= ERRSTR_DEFAULT_BUF_SIZE);

struct errstr_buffer {
    size_t capacity;
    size_t length;

    char buffer[];
};

static pthread_once_t errstrbuf_init_flag = PTHREAD_ONCE_INIT;
static pthread_key_t errstrbuf_key;
static bool errstrbuf_key_exists = false;

static void errstrbuf_free(void *buffer) {
    free(buffer);
    pthread_setspecific(errstrbuf_key, NULL);
}

static void errstrbuf_key_init() {
    errstrbuf_key_exists = !pthread_key_create(&errstrbuf_key, errstrbuf_free);
}

static size_t errstrbuf_max_capacity() {
    return ERRSTR_MAX_BUF_SIZE - 1;
}

static size_t errstrbuf_space(struct errstr_buffer *buffer) {
    return buffer->capacity - buffer->length;
}

static char * errstrbuf_loc_with_capacity_and_length(struct errstr_buffer *buffer, size_t capacity, size_t length) {
    return buffer->buffer + capacity - length;
}

static char * errstrbuf_loc_with_length(struct errstr_buffer *buffer, size_t length) {
    return errstrbuf_loc_with_capacity_and_length(buffer, buffer->capacity, length);
}

static char * errstrbuf_loc(struct errstr_buffer *buffer) {
    return errstrbuf_loc_with_length(buffer, buffer->length);
}

static struct errstr_buffer * errstrbuf_allocate(size_t capacity) {
    if(capacity > errstrbuf_max_capacity()) {
        return NULL;
    }

    struct errstr_buffer *buffer = malloc(sizeof(struct errstr_buffer) + capacity + 1);
    if(!buffer) {
        return NULL;
    }

    buffer->capacity = capacity;
    buffer->length = 0;
    buffer->buffer[capacity] = '\0';

    if(pthread_setspecific(errstrbuf_key, buffer)) {
        free(buffer);
        buffer = NULL;
    }

    return buffer;
}

static struct errstr_buffer * errstrbuf_resize(struct errstr_buffer *buffer, size_t capacity) {
    if(buffer->capacity > capacity) {
        return buffer;
    }

    struct errstr_buffer *nbuf = errstrbuf_allocate(capacity);
    if(!nbuf) {
        return buffer;
    }

    memcpy(errstrbuf_loc_with_length(nbuf, buffer->length), errstrbuf_loc(buffer), buffer->length);
    free(buffer);

    return nbuf;
}

static struct errstr_buffer * errstrbuf_get() {
    if(pthread_once(&errstrbuf_init_flag, errstrbuf_key_init) || !errstrbuf_key_exists) {
        return NULL;
    }

    struct errstr_buffer *buffer = pthread_getspecific(errstrbuf_key);
    if(buffer) {
        return buffer;
    }

    return errstrbuf_allocate(ERRSTR_DEFAULT_BUF_SIZE - 1);
}

size_t errstr_length() {
    struct errstr_buffer *restrict errstrbuf = errstrbuf_get();
    if(!errstrbuf) {
        return 0;
    }

    return errstrbuf->length;
}

const char *errstr_location() {
    struct errstr_buffer *restrict errstrbuf = errstrbuf_get();
    if(!errstrbuf) {
        return NULL;
    }

    return errstrbuf_loc(errstrbuf);
}

void errstr_clear() {
    struct errstr_buffer *restrict errstrbuf = errstrbuf_get();
    if(!errstrbuf) {
        return;
    }

    errstrbuf->length = 0;
}

int errstr_present() {
    return errstr_length() > 0;
}

size_t errstrf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    size_t len = verrstrf(fmt, args);

    va_end(args);

    return len;
}

size_t verrstrf(const char *fmt, va_list args) {
    struct errstr_buffer *restrict errstrbuf = errstrbuf_get();
    if(!errstrbuf) {
        return 0;
    }

    char prepend_buf[ERRSTR_MAX_BUF_SIZE];

    int prepend_length = vsnprintf(prepend_buf, sizeof(prepend_buf), fmt, args);
    if(prepend_length < 0 || prepend_length >= sizeof(prepend_buf)) {
        strncpy(prepend_buf, "(format error) ", sizeof(prepend_buf));
        prepend_length = strnlen(prepend_buf, sizeof(prepend_buf));
    }

    if(prepend_length > errstrbuf_space(errstrbuf)) {
        size_t wanted_capacity = prepend_length + errstrbuf->length;
        if(wanted_capacity > errstrbuf_max_capacity()) {
            wanted_capacity = errstrbuf_max_capacity();
        }
        errstrbuf = errstrbuf_resize(errstrbuf, wanted_capacity);

        size_t append_length = errstrbuf->capacity - prepend_length;
        if(append_length < errstrbuf->length) {
            memmove(errstrbuf_loc_with_length(errstrbuf, append_length), errstrbuf_loc(errstrbuf), append_length);
            errstrbuf->length = append_length;
        }
    }
    memcpy(errstrbuf_loc(errstrbuf) - prepend_length, prepend_buf, prepend_length);

    errstrbuf->length += prepend_length;
    return errstrbuf->length;
}

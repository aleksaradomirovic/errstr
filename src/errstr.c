/*
 * Copyright (C) 2025  Aleksa Radomirovic
 * SPDX-License-Identifier: Apache-2.0
 */

#include "errstr.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifndef ERRSTR_DEFAULT_BUF_SIZE
#   define ERRSTR_DEFAULT_BUF_SIZE BUFSIZ
#endif

#define errstr_buf_size_plus_nul ERRSTR_DEFAULT_BUF_SIZE
#define errstr_buf_size (errstr_buf_size_plus_nul - 1)
static thread_local char errstr_buf[errstr_buf_size_plus_nul] = { [errstr_buf_size_plus_nul - 1] = '\0' };
static thread_local size_t errstr_buf_len = 0;

size_t errstr_length() {
    return errstr_buf_len;
}

const char *errstr_location() {
    return errstr_buf + errstr_buf_size - errstr_buf_len;
}

void errstr_clear() {
    errstr_buf_len = 0;
}

int errstr_present() {
    return (errstr_length() > 0);
}

size_t errstrf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    size_t len = verrstrf(fmt, args);

    va_end(args);

    return len;
}

size_t verrstrf(const char *fmt, va_list args) {
    char prepend_buf[errstr_buf_size_plus_nul];

    int prepend_length = vsnprintf(prepend_buf, errstr_buf_size_plus_nul, fmt, args);
    if(prepend_length < 0 || prepend_length >= errstr_buf_size_plus_nul) {
        strncpy(prepend_buf, "(format error) ", errstr_buf_size_plus_nul);
        prepend_length = strlen(prepend_buf);
    }

    if(prepend_length > errstr_buf_size - errstr_buf_len) {
        // PANIC!!!! (TODO truncate)
    } else {
        memcpy(errstr_buf + errstr_buf_size - errstr_buf_len - prepend_length, prepend_buf, prepend_length);
    }

    errstr_buf_len += prepend_length;
    return errstr_buf_len;
}

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

#define errstr_buf_size ERRSTR_DEFAULT_BUF_SIZE
static thread_local char errstr_buf[errstr_buf_size] = { '\0' };

size_t errstr_length() {
    return strnlen(errstr_buf, errstr_buf_size);
}

const char *errstr_location() {
    return errstr_buf;
}

void errstr_clear() {
    memset(errstr_buf, 0, errstr_buf_size);
}

size_t errstrf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    size_t len = verrstrf(fmt, args);

    va_end(args);

    return len;
}

size_t verrstrf(const char *fmt, va_list args) {
    char append_buf[errstr_buf_size];
    strncpy(append_buf, errstr_buf, errstr_buf_size);

    char *buf_ptr = errstr_buf;
    int print_length = vsnprintf(buf_ptr, errstr_buf_size - (buf_ptr - errstr_buf), fmt, args);
    if(print_length < 0) {
        buf_ptr = stpncpy(buf_ptr, "(format error) ", errstr_buf_size - (buf_ptr - errstr_buf));
    } else if(print_length >= errstr_buf_size) {
        return errstr_buf_size;
    } else {
        buf_ptr += print_length;
    }

    buf_ptr = stpncpy(buf_ptr, append_buf, errstr_buf_size - (buf_ptr - errstr_buf));
    return buf_ptr - errstr_buf;
}

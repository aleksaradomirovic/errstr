/*
 * Copyright (C) 2025  Aleksa Radomirovic
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ERRSTR_H_
#define ERRSTR_H_

#include <stdarg.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Get the current length of the contents of this thread's error string buffer.
 * @return the current length of the error string buffer
 */
size_t errstr_length();

/**
 * The current length of the contents of this thread's error string buffer.
 */
#define errstrlen (errstr_length())

/**
 * Get a pointer to this thread's error string buffer.
 * @return a pointer to the error string buffer
 */
const char *errstr_location();

/**
 * A pointer to this thread's error string buffer.
 */
#define errstr (errstr_location())

/**
 * Clear the contents of this thread's error string buffer.
 */
void errstr_clear();

/**
 * Check whether this thread's error string buffer has contents.
 * @return non-zero if contents are present, zero if not present
 */
int errstr_present();

/**
 * Prepend this thread's error string buffer with the given formatted string.
 * @param fmt a format string
 * @param ... arguments for a format string
 * @return the new length of the error string buffer
 *
 * @see <a href="https://cppreference.com/w/c/io/fprintf.html">printf</a>
 */

#if defined(__has_attribute)
#   if __has_attribute(format)
__attribute__((format(printf, 1, 2)))
#   endif
#endif
size_t errstrf(const char *fmt, ...);

/**
 * Prepend this thread's error string buffer with the given formatted string.
 * @param fmt a format string
 * @param args va_list of arguments for a format string
 * @return the new length of the error string buffer
 *
 * @see <a href="https://cppreference.com/w/c/io/vfprintf.html">vprintf</a>
 */

#if defined(__has_attribute)
#   if __has_attribute(format)
__attribute__((format(printf, 1, 0)))
#   endif
#endif
size_t verrstrf(const char *fmt, va_list args);


/**
 * Prepend as if using {@link errstrf}, then return **value** from the current function.
 * @param value return value
 * @param fmt same as in {@link errstrf}
 * @param ... same as in {@link errstrf}
 *
 * @note This is a macro, **not a function**, for convenient error reporting. It will cause the calling function to return.
 */
#define errstrf_return(value, fmt, ...) { errstrf(fmt __VA_OPT__(,) __VA_ARGS__); return value; }

/**
 * Prepend as if using {@link errstrf}, then call <a href="https://en.cppreference.com/w/c/program/exit.html">exit</a> with **status**.
 * @param status exit status
 * @param fmt same as in {@link errstrf}
 * @param ... same as in {@link errstrf}
 */
#define errstrf_exit(status, fmt, ...) { errstrf(fmt __VA_OPT__(,) __VA_ARGS__); exit(status); }


#ifdef __cplusplus
}
#endif

#endif

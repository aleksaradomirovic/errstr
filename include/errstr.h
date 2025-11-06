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
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


size_t errstr_length();
#define errstrlen (errstr_length())

const char *errstr_location();
#define errstr (errstr_location())

void errstr_clear();

__attribute__((format(printf, 1, 2)))
size_t errstrf(const char *fmt, ...);

__attribute__((format(printf, 1, 0)))
size_t verrstrf(const char *fmt, va_list args);


#ifdef __cplusplus
}
#endif

#endif

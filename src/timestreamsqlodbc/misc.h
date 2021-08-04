/*
 * Copyright <2021> Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 */

#ifndef __MISC_H__
#define __MISC_H__

#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

size_t strncpy_null(char *dst, const char *src, ssize_t len);
#ifndef HAVE_STRLCAT
size_t strlcat(char *, const char *, size_t);
#endif /* HAVE_STRLCAT */
int snprintfcat(char *buf, size_t size, const char *format, ...)
    __attribute__((format(TS_PRINTF_ATTRIBUTE, 3, 4)));

char *make_string(const SQLCHAR *s, SQLINTEGER len, char *buf, size_t bufsize);
/* #define	GET_SCHEMA_NAME(nspname) 	(stricmp(nspname, "public") ? nspname :
 * "") */

#define GET_SCHEMA_NAME(nspname) (nspname)

/* defines for return value of my_strcpy */
#define STRCPY_SUCCESS 1
#define STRCPY_FAIL 0
#define STRCPY_TRUNCATED (-1)
#define STRCPY_NULL (-2)

ssize_t my_strcpy(char *dst, ssize_t dst_len, const char *src, ssize_t src_len);

/*
 *	Macros to safely strcpy, strcat or sprintf to fixed arrays.
 *
 */

/* macro to safely strcpy() to fixed arrays. */
#define STRCPY_FIXED(to, from) strncpy_null((to), (from), sizeof(to))

/* macro to safely strcat() to fixed arrays. */
#define STRCAT_FIXED(to, from) strlcat((to), (from), sizeof(to))

/* macro to safely sprintf() to fixed arrays. */
#define SPRINTF_FIXED(to, ...) snprintf((to), sizeof(to), __VA_ARGS__)

/* macro to safely sprintf() & cat to fixed arrays. */
#define SPRINTFCAT_FIXED(to, ...) snprintfcat((to), sizeof(to), __VA_ARGS__)

#define ITOA_FIXED(to, from) snprintf((to), sizeof(to), "%d", from)

#ifdef __cplusplus
}
#endif

#endif /* __MISC_H__ */
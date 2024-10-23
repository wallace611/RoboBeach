#pragma once
#ifndef _THREADS_H
#define _THREADS_H

#include <time.h>

typedef unsigned int _Thrd_id_t;

struct thrd_t { // thread identifier for Win32
    void* _Hnd; // Win32 HANDLE
    _Thrd_id_t _Id;
};
typedef struct thrd_t thrd_t;

enum thrd_result { 
    thrd_success = 0,
    thrd_nomem = 1,
    thrd_timedout = 2,
    thrd_busy = 3,
    thrd_error = 4
};

/** THIS SECTION IS COPIED FROM GNU LIB C **/
/** Copyright (C) 2018-2023 Free Software Foundation, Inc. **/
typedef int (*thrd_start_t) (void*);
/* Create a new thread executing the function __FUNC.  Arguments for __FUNC
are passed through __ARG.  If successful, __THR is set to new thread
identifier.  */
extern int __cdecl thrd_create (thrd_t *__thr, thrd_start_t __func, void *__arg);

/** END GNU LIBC SECTION **/

extern int __cdecl thrd_join(thrd_t, int*);

#endif
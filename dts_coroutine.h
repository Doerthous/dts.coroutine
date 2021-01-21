/*
   The MIT License (MIT)

   Copyright (c) 2021 Doerthous

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

   Authour: Doerthous <doerthous@gmail.com>
*/

#ifndef DTS_COROUTINE_H_
#define DTS_COROUTINE_H_

#include <assert.h>
#include <setjmp.h>
#include <stdint.h>

typedef struct
{
    jmp_buf *out;
    int call_level;
    uint32_t use_jmp:1;
    uint32_t yield:1;
    uint32_t max_level:31;
    uint16_t *breakpoint;
} dts_co_t;

/*
    1. The return value of function is occupied by coroutine.

    2. If you want to use co_yield_val, the first argument of coroutine function
        must be `type_t *co_yval`. e.g: int cf1(int *co_yval, ...).s
*/

#define dts_co_start(co) \
    assert ((co)->call_level < ((co)->max_level-1)); \
    (co)->call_level += 1; \
    switch((co)->breakpoint[(co)->call_level]) \
        default:{

#define dts_co_exit(co) \
			(co)->breakpoint[(co)->call_level] = __LINE__; \
            (co)->call_level -= 1; \
			case __LINE__:; \
		} \
	return 0;

#define dts_co_yield(co) do \
{ \
    (co)->breakpoint[(co)->call_level] = __LINE__; \
    (co)->yield = 1; \
    if ((co)->use_jmp) { \
        (co)->call_level = -1; \
        longjmp(*((co)->out), 1); \
    } \
    else { \
        (co)->call_level -= 1; \
        return 1; \
    } \
    case __LINE__:; \
}while(0)

#define dts_co_call(co, co_func, ...) do \
{ \
    (co)->breakpoint[(co)->call_level] = __LINE__; \
    case __LINE__: if (co_func(co, ##__VA_ARGS__)) { \
        (co)->call_level -= 1; \
        return 1; \
    } \
} while (0)

#define dts_co_wait_until(co, expr) do \
{ \
    dts_co_yield(co); \
} while (!(expr))

dts_co_t *dts_co_cast(void *mem, size_t size);

#define dts_co_resume(co, co_func, jmp, ...) do \
{ \
    co->yield = 0; \
    if (jmp) { \
        jmp_buf outside; \
        co->use_jmp = 1; \
        co->out = &outside; \
        if (setjmp(outside)==0) { \
            co_func(co, ##__VA_ARGS__); \
        } \
    } \
    else { \
        co->use_jmp = 0; \
        co->out = NULL; \
        co_func(co, ##__VA_ARGS__); \
    } \
} while (0)
#define dts_co_dead(co) (!co1->yield)

#ifdef USING_DTS_COROUTINE
# define co_t dts_co_t
# define co_start dts_co_start
# define co_exit dts_co_exit
# define co_yield dts_co_yield
# define co_call dts_co_call
# define co_yval dts_co_yval
# define co_wait_until dts_co_wait_until
# define co_cast dts_co_cast
# define co_resume dts_co_resume
# define co_dead dts_co_dead
#endif // USING_DTS_COROUTINE

#endif // DTS_COROUTINE_H_

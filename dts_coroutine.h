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

#include <setjmp.h>
#include <stdint.h>

typedef struct
{
    char sign[4];
    jmp_buf *main;
    jmp_buf ctx;
    uint8_t *stack;
} dts_co_t;

// MinGW32
#ifdef __MINGW32__
# define jmp_buf_sp(jb) ((jb)[2])
# ifdef setjmp
#  undef setjmp
# endif
# define setjmp __builtin_setjmp
# ifdef longjmp
#  undef longjmp
# endif
# define longjmp __builtin_longjmp
#elif defined(_WIN32) || defined(_WIN64)
// Windows
// https://stackoverflow.com/questions/26605063/an-invalid-or-unaligned-stack-was-encountered-during-an-unwind-operation
# define jmp_buf_sp(jb) (((_JUMP_BUFFER *)jb)->Rsp)
# define longjmp(jb,v) ((_JUMP_BUFFER *)jb)->Frame = 0; longjmp(jb, v);
#endif

void dts_co_yield(dts_co_t *co);

void dts_co_exit(dts_co_t *co);

int dts_co_dead(dts_co_t *co);

#define dts_co_resume(co, func, ...) do \
{ \
    jmp_buf mjb; \
    (co)->main = &mjb; \
    if (!setjmp(mjb)) { \
        if ((co)->sign[0] != 'C') { \
            (co)->sign[0] = 'C'; \
            func(co, ##__VA_ARGS__); \
        } \
        else if (!dts_co_dead(co)) { \
            /* here we save the co ptr to the stack space which is no used. */ \
            ((dts_co_t **)jmp_buf_sp((co)->ctx))[-2] = co; \
            longjmp((co)->ctx, 1); \
        } \
    } \
} while (0)

dts_co_t *dts_co_init(dts_co_t *co, uint8_t *stack);

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
# define co_init dts_co_init
#endif // USING_DTS_COROUTINE

#endif // DTS_COROUTINE_H_

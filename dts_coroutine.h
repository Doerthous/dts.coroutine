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

/*
    1. The return value of function is occupied by coroutine.

    2. If you want to use co_yield_val, the first argument of coroutine function
        must be `type_t *co_yval`. e.g: int cf1(int *co_yval, ...).s
*/

#define dts_co_start() \
    static int __dts_co_break_point; \
    switch(__dts_co_break_point) \
        default:{

#define dts_co_exit() \
			__dts_co_break_point = __LINE__; \
			case __LINE__:; \
		} \
	return 0;

#define dts_co_yield() do \
{ \
    __dts_co_break_point = __LINE__; \
    return 1; \
    case __LINE__:; \
}while(0)

#define dts_co_yield_val(val) do \
{ \
    *dts_co_yval = (val); \
    dts_co_yield(); \
} while (0)

#define dts_co_call(co_func, ...) do \
{ \
    case __LINE__: if (co_func(__VA_ARGS__)) { \
        __dts_co_break_point = __LINE__; \
        return 1; \
    } \
} while (0)

#define dts_co_wait_until(expr) do \
{ \
    dts_co_yield(); \
} while (!(expr))

#ifdef USING_DTS_COROUTINE
# define co_start dts_co_start
# define co_exit dts_co_exit
# define co_yield dts_co_yield
# define co_yield_val dts_co_yield_val
# define co_call dts_co_call
# define co_yval dts_co_yval
# define co_wait_until dts_co_wait_until
#endif // USING_DTS_COROUTINE

#endif // DTS_COROUTINE_H_

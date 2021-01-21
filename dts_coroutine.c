#define USING_DTS_COROUTINE
#include <dts_coroutine.h>
#include <string.h>

void dts_co_yield(dts_co_t *co)
{
    jmp_buf ctx;
    if (!setjmp(co->ctx)) {
        // save the stack of co.
        memcpy(co->stack, (void *)jmp_buf_sp(co->ctx),
            jmp_buf_sp(*(co->main))-jmp_buf_sp(co->ctx)+32);
        longjmp(*(co->main), 1);
    }
    setjmp(ctx); // restore stack pointer.
    co = ((dts_co_t **)jmp_buf_sp(ctx))[-2]; // get the real co ptr.
    memcpy((void *)jmp_buf_sp(co->ctx), co->stack, // restore the stack of co.
        jmp_buf_sp(*(co->main))-jmp_buf_sp(co->ctx)+32);
}

void dts_co_exit(dts_co_t *co)
{
    co->sign[3] = 0;
}

int dts_co_dead(dts_co_t *co)
{
    return co->sign[3] == 0;
}

dts_co_t *dts_co_init(dts_co_t *co, uint8_t *stack)
{
    memcpy(co->sign, "AORT", 4);
    co->stack = stack;
    return co;
}

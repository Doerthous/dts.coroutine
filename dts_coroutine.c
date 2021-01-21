#define USING_DTS_COROUTINE
#include <dts_coroutine.h>
#include <string.h>

//
static co_t *co_init(co_t *co)
{
    co->call_level = -1;
    co->out = NULL;
    co->use_jmp = 0;
}

co_t *co_cast(void *mem, size_t size) 
{
    if (size >= sizeof(co_t)+4) {
        co_t *co = (co_t *)mem;
        memset(mem, 0, size);
        co->breakpoint = (uint16_t *)(((size_t)mem)+sizeof(co_t));
        co->max_level = (size-sizeof(co_t))/2;
        return co_init(co);
    }
    return NULL;
}

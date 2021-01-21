#define USING_DTS_COROUTINE
#include <dts_coroutine.h>

#include <stdio.h>

int d(co_t *co)
{
    dts_co_start(co);
    printf("d");
    dts_co_yield(co);
    printf("d");
    dts_co_exit(co);
}

int c(co_t *co)
{
    dts_co_start(co);
    printf("c");
    dts_co_yield(co);
    printf("x");
    dts_co_call(co, d);
    printf("c");
    dts_co_exit(co);
}


int main(int argc, char const *argv[])
{
    co_t *co1;
    uint8_t mem[32];

    co1 = co_cast(mem, 32);
    while (1) {
        printf("j");
        co_resume(co1, c, 1);
        if (co_dead(co1)) {
            break;
        }
    }

    printf("\n");

    co1 = co_cast(mem, 32);
    while (1) {
        printf("r");
        co_resume(co1, c, 0);
        if (co_dead(co1)) {
            break;
        }
    }

    printf("\n");

    co_t *co2;
    uint8_t mem2[32];
    co1 = co_cast(mem, 32);
    co2 = co_cast(mem2, 32);
    while (1) {
        printf("1");
        co_resume(co1, c, 0);
        printf("2");
        co_resume(co2, c, 1);
        if (co_dead(co1) && co_dead(co2)) {
            break;
        }
    }

    return 0;
}

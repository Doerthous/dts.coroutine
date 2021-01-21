#define USING_DTS_COROUTINE
#include <dts_coroutine.h>

#include <stdio.h>

void d(co_t *co)
{
    int id = 1;
    printf("co[%d]: din\n", id);
    co_yield(co);
    printf("co[%d]: dout\n", id);
}

void c(co_t *co)
{
    int id = 1;
    printf("co[%d]: cin\n", id);
    co_yield(co);
    d(co);
    co_yield(co);
    printf("co[%d]: cout\n", id);
    co_exit(co);
}

void e(co_t *co, co_t *join)
{
    while (!co_dead(join)) {
        printf("co[2]\n");
        co_yield(co);
    }
    co_exit(co);
}

int main(int argc, char const *argv[])
{
    uint8_t stack1[1024];
    uint8_t stack2[1024];
    co_t co1;
    co_t co2;

    co_init(&co1, stack1);
    co_init(&co2, stack2);

    while (!co_dead(&co2)) {
        co_resume(&co1, c);
        co_resume(&co2, e, &co1);
    }

    return 0;
}


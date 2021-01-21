#define USING_DTS_COROUTINE
#include "dts_coroutine.h"

#include <stdio.h>
int co_level_3() {
    co_start();
    printf("l3: co_yield()\n"); co_yield();
    co_exit();
}
int co_level_2(int *co_yval) {
    co_start();
    printf("l2: co_yield_val(201)\n"); co_yield_val(201);
    co_call(co_level_3);
    printf("l2: co_yield_val(202)\n"); co_yield_val(202);
    co_exit();
}
int co_level_1(int *co_yval) {
    co_start();
    printf("l1: co_yield_val(101)\n"); co_yield_val(101);
    printf("l1: co_yield()\n"); co_yield();
    static int i = 3;
    while(--i) {
        printf("l1: co_yield_val(i*4+100), i=%d\n", i); co_yield_val(i*4+100);
    }
    co_call(co_level_2, co_yval);
    co_exit();
}
int main(int argc, char const *argv[]) {
    while (co_level_1(&argc))
        printf("yield: %d\n", argc);
    return 0;
}

#include "test.h"

void develop_test(void)
{
    printf("TEST\n");
    // Add function to test here
    uint16_t m1 = 0;
    rotate_single_pv(m1, 1, false);
    sleep_ms(100);
    uint16_t m2 = 1;
    rotate_single_pv(m2, 1, false);
    sleep_ms(100);
    uint16_t m3 = 2;
    rotate_single_pv(m3, 1, false);
    sleep_ms(100);
    uint16_t m4 = 3;
    rotate_single_pv(m4, 1, false);
}

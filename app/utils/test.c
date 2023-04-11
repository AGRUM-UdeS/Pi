#include "test.h"

void develop_test(void)
{
    printf("TEST\n");
    // Add function to test here
    static uint16_t i = 0;
    if (i++ == 12) {
        while (true) {
            tight_loop_contents();
        }
    }
}

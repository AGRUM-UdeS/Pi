#include "wrap_WATCHDOG.h"

#define WATCHDOG_TIMEOUT_MS 200

void init_watchdog(void)
{
    if (watchdog_caused_reboot()) {
        printf("Watchdog reboot!\n");
    }

    watchdog_enable(WATCHDOG_TIMEOUT_MS, 1);
    printf("Watchdog enabled!\n");
}

void feed_watchdog(void)
{
    watchdog_update();
}

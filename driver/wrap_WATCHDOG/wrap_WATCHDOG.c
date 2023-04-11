#include "wrap_WATCHDOG.h"

#define WATCHDOG_TIMEOUT_MS 200

static bool reboot_by_watchdog = false;

void init_watchdog(void)
{
    if (watchdog_caused_reboot()) {
        reboot_by_watchdog = true;
        printf("Reboot by watchdog!\n");
    }

    watchdog_enable(WATCHDOG_TIMEOUT_MS, 1);
    printf("Watchdog enabled!\n");
}

void feed_watchdog(void)
{
    watchdog_update();
}

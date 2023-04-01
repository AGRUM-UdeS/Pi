#include "irrigation.h"

void init_irrigation(void) {
    init_water_level_sensors();
    init_pump();
    sleep_ms(10);
    initp_valve();
}

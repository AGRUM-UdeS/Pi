#include "test.h"

void develop_test(void)
{
    printf("TEST\n");
    // Add function to test here
    SHT_measure_t meas = read_temp_humidity();
    printf("\nAir Temperature under panel (Celsius): %f",meas.temp);
    printf("\nAir Humidity under panel (%): %f", meas.humidity);
}

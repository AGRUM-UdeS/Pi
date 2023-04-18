#include "test.h"

void develop_test(void)
{
    printf("TEST\n");
    // Add function to test here
    uint16_t rv;
    for (size_t i = 0; i < 5; i++)
    {
        if (ADC_read_pin(ADC_address_1, ADC_pin[i], &rv) != ADC_ok) {
            printf("Erreur lecture ADC (%d)\n", i);
        } else {
            printf("Lecture pin %d : %.2f V\n", i, ADC_bits2voltage(rv));
        }
    }

    for (size_t i = 0; i < 5; i++)
    {
        if (ADC_read_pin(ADC_address_0, ADC_pin[i], &rv) != ADC_ok) {
            printf("Erreur lecture ADC (%d)\n", i);
        } else {
            printf("Lecture pin adc 0 %d : %.2f V\n", i, ADC_bits2voltage(rv));
        }
    }
}

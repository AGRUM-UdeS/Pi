#include "test.h"

void develop_test(void)
{
    printf("TEST\n");
    // Add function to test here
    //get_instant_power_PV();

    // tous ces tests permettent de vérifier le fonctionnement des IO expender et des GPIO extra du PI
    //test_GPIO(IO_address_1,5);
    // printf("26,27");
    // test_GPIO_ras(26,27,5);
    // printf("20,19");
    // test_GPIO_ras(20,19,5);
    // printf("0,1");
    // test_GPIO_ras(0,1,5);
    // printf("4,5");
    // test_GPIO_ras(4,5,5);
    // sleep_ms(1000);

    //permet de tester le moteur 1
    //test_motor_PCB(1,20);

    //permet de tester le capteur de température 0x44 du canal I2C0
    test_Capt_temp(0x44);
}

void test_GPIO(int adresse, int nb_pulse)
{
    static int it = 0;
    printf("Itération (%d)\n", it++);
    int i;
    int j;
    for (i = 0; i < 8; i++)
    {
        IO_clear_pin(adresse,i);
        IO_set_as_output(adresse, i);
        IO_clear_pin(adresse,i);
    }
    printf("fin du init\n");
    for (i = 0; i < 8; i++)
    {
        printf("gere le port %d\n",i);
        for (j = 1; j<=nb_pulse; j++)
        {
            IO_set_pin(adresse,i);
            sleep_ms(1000);
            IO_clear_pin(adresse,i);
            sleep_ms(1000);
        }
        sleep_ms(5000);
    }


    
    
    
}

void test_leds(int nb)
{
    gpio_init(10);
    gpio_init(11);
    gpio_set_dir(10, GPIO_OUT);
    gpio_set_dir(11, GPIO_OUT);

    for (int i = 0; i <=nb; i++)
    {
        gpio_put(10,1);
        gpio_put(11,1);
        sleep_ms(500);
        gpio_put(10,0);
        gpio_put(11,0);
        sleep_ms(500);
    }
    
}

void test_GPIO_ras(int GP1, int GP2, int nb)
{
    gpio_init(GP1);
    gpio_init(GP2);
    gpio_set_dir(GP1, GPIO_OUT);
    gpio_set_dir(GP2, GPIO_OUT);

    for (int i = 0; i < nb; i++)
    {
        gpio_put(GP1,1);
        gpio_put(GP2,1);
        sleep_ms(500);
        gpio_put(GP1,0);
        gpio_put(GP2,0);
        sleep_ms(500);
    }
    
}

void test_motor_PCB(int ind_mot, int angle)
{
    init_motor();
    rotate_single_pv(ind_mot,angle,0);

}

void test_Capt_temp(int adresse)
{
    read_temp(adresse);
}
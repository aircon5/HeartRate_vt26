#include "esp_task_wdt.h"
#include "driver/dac.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "soc/dac_channel.h"
#include "esp32/rom/ets_sys.h"

//The sampling freq to be used
int freq = 1000;
#define M 4
#define N 4

#define SDA_PIN 14
#define SCL_PIN 12

//sofia är inne
//annika med!!
//Callback for how often you sample.
static void periodic_timer_callback(void *arg)
{
    gpio_set_level(GPIO_NUM_27, 1);
    float adc_val = 0;

    for(int i=0;i<8;i++){
        adc_val += adc1_get_raw(ADC1_CHANNEL_4);
    }
    adc_val /= 8;

    int dac_val = (int) adc_val >> 4;

    dac_output_voltage(DAC_CHANNEL_1, dac_val);
    printf("%d \n", (int)adc_val);
    gpio_set_level(GPIO_NUM_27, 0);
}

void app_main()
{
    printf("hejjjjjhej \n");
    //init adc and dac
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
    gpio_pullup_en(GPIO_NUM_32);
    dac_output_enable(DAC_CHANNEL_1);

    gpio_config_t config;
    config.pin_bit_mask = 1 << GPIO_NUM_27;
    config.mode = GPIO_MODE_DEF_OUTPUT;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_ENABLE;
    ESP_ERROR_CHECK(gpio_config(&config));

    printf("hejjjjjhej2 \n");

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &periodic_timer_callback,
        //name is optional, but may help identify the timer when debugging
        .name = "periodic"};

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000000/freq));

}
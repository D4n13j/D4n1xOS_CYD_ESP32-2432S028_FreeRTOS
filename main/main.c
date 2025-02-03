#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"

#define LED_PIN_1 4   // GPIO Pin 4 for the first LED
#define LED_PIN_2 16  // GPIO Pin 16 for the second LED
#define LED_PIN_3 17  // GPIO Pin 16 for the second LED

// Task 1 - Blink LED on pin 4 every 500ms
void blink_led_1(void *pvParameter) {
    gpio_set_direction(LED_PIN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN_2, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_PIN_1, 0);  // Turn LED on
        gpio_set_level(LED_PIN_2, 0);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 500ms
        gpio_set_level(LED_PIN_1, 1);  // Turn LED off
        gpio_set_level(LED_PIN_2, 1);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 500ms
    }
}

// Task 2 - Blink LED on pin 16 every 200ms
void blink_led_2(void *pvParameter) {
    gpio_set_direction(LED_PIN_3, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_PIN_3, 1);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 200ms
        gpio_set_level(LED_PIN_3, 0);  // Turn LED off
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 200ms
    }
}

void app_main() {
    // Create two tasks to blink LEDs with different timings
    xTaskCreatePinnedToCore(blink_led_1, "blink_led_1", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(blink_led_2, "blink_led_2", 2048, NULL, 1, NULL, 1);
    
}

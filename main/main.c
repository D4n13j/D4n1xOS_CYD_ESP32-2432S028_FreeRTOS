#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio.h"
#include "lvgl.h"
#include "lvgl_helpers.h"
#include "XPT2046.h" // Include the touch driver header

#define LED_PIN_1 4   // GPIO Pin 4 for the red LED
#define LED_PIN_2 16  // GPIO Pin 16 for the green LED
#define LED_PIN_3 17  // GPIO Pin 17 for the blue LED

// Task 1 - Blink LEDs on pins 4 and 16 every 500ms
void blink_led_1(void *pvParameter) {
    gpio_set_direction(LED_PIN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_PIN_2, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_PIN_1, 0);  // Turn LED on
        gpio_set_level(LED_PIN_2, 0);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 500ms
        gpio_set_level(LED_PIN_1, 1);  // Turn LED off
        gpio_set_level(LED_PIN_2, 1);  // Turn LED off
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 500ms
    }
}

// Task 2 - Blink LED on pin 16 every 500ms
void blink_led_2(void *pvParameter) {
    gpio_set_direction(LED_PIN_3, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_PIN_3, 1);  // Turn LED off
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 500ms
        gpio_set_level(LED_PIN_3, 0);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 500ms
    }
}

// Task to handle LVGL
void lvgl_task(void *pvParameter) {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_task_handler();
    }
}

void app_main() {
    // Initialize LVGL
    lv_init();

    // Initialize ILI9341 driver
    lvgl_driver_init();

    static lv_color_t buf1[DISP_BUF_SIZE];
    static lv_color_t buf2[DISP_BUF_SIZE];
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 320;
    lv_disp_drv_register(&disp_drv);

    // Initialize XPT2046 touch driver
    lvgl_touch_init();

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_driver_read;
    lv_indev_drv_register(&indev_drv);

    // Create two tasks to blink LEDs with different timings on different cores
    xTaskCreatePinnedToCore(blink_led_1, "blink_led_1", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(blink_led_2, "blink_led_2", 2048, NULL, 1, NULL, 1);

    // Create a task to handle LVGL
    xTaskCreatePinnedToCore(lvgl_task, "lvgl_task", 4096, NULL, 5, NULL, 1);
}
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BUTTON_GPIO 15
#define RELAY1 47

typedef enum {
    PRESSED,
    RELEASE,
} btn_event_e;
typedef void *buttonEventCallback(btn_event_e);


buttonEventCallback btn_event = NULL;
void button_init(buttonEventCallback cb){//callback function
    gpio_reset_pin(RELAY1);
    gpio_set_direction(RELAY1, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY); // Nút kéo lên, nhấn xuống nối GND

    ifg
    xTaskCreate(button_task, "Button Task", 2048, NULL, 5, NULL);
}

void button_task(void *pvParam) {
    bool last_state = 1;

    while (1) {
        bool current_state = gpio_get_level(BUTTON_GPIO);
        if (last_state == 1 && current_state == 0) { // Phát hiện nhấn (falling edge)
            led3_state = !led3_state; // Đổi trạng thái LED
            buttonEventCallback(PRESSED);
            vTaskDelay(pdMS_TO_TICKS(300)); // Chống dội nút
        }
        last_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(10)); // Đọc nút mỗi 10ms
    }
}
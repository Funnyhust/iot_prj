#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// ==== Định nghĩa GPIO ====
#define LED1_GPIO 14
#define LED2_GPIO 13
#define LED3_GPIO 12
#define BUTTON_GPIO 15
#define RELAY1 47


// ==== Biến trạng thái LED3 (dùng chung giữa các task) ====
volatile bool led3_state = false;

// ==== Task 1: Nháy LED1 mỗi 500ms ====
void blink_led1_task(void *pvParam) {
    gpio_reset_pin(LED1_GPIO);
    gpio_set_direction(LED1_GPIO, GPIO_MODE_OUTPUT);


    while (1) {
        gpio_set_level(LED1_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED1_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ==== Task 2: Nháy LED2 mỗi 1000ms ====
void blink_led2_task(void *pvParam) {
    gpio_reset_pin(LED2_GPIO);
    gpio_set_direction(LED2_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED2_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(LED2_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ==== Task 3: Đọc nút nhấn để bật/tắt LED3 ====
void button_task(void *pvParam) {
    gpio_reset_pin(RELAY1);
    gpio_set_direction(RELAY1, GPIO_MODE_OUTPUT);

    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY); // Nút kéo lên, nhấn xuống nối GND
    
    bool last_state = 1;

    while (1) {
        bool current_state = gpio_get_level(BUTTON_GPIO);
        if (last_state == 1 && current_state == 0) { // Phát hiện nhấn (falling edge)
            led3_state = !led3_state; // Đổi trạng thái LED
            gpio_set_level(RELAY1, led3_state);
            vTaskDelay(pdMS_TO_TICKS(300)); // Chống dội nút
        }
        last_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(10)); // Đọc nút mỗi 10ms
    }
}



// ==== Hàm main ====
void app_main(void) {
    // Tạo các task







    ESP_LOGI("CHECK", "ESP32-S3 is running your code!");
    xTaskCreate(blink_led1_task, "LED1 Task", 2048, NULL, 5, NULL);
    xTaskCreate(blink_led2_task, "LED2 Task", 2048, NULL, 5, NULL);
    xTaskCreate(button_task, "Button Task", 2048, NULL, 5, NULL);
}

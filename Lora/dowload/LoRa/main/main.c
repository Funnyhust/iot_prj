#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lora/lora.h"
#include "esp_log.h"

void task_tx(void *p)
{
   for(;;) {
      vTaskDelay(pdMS_TO_TICKS(5000));
      lora_send_packet((uint8_t*)"Hello", 5);
      printf("packet sent...\n");
      ESP_LOGI("LORA", "packet sent: Hello");
   }
}

void app_main()
{
   lora_init();
   lora_set_frequency(433e6);
   lora_enable_crc();
   xTaskCreate(&task_tx, "task_tx", 4096, NULL, 5, NULL);
}
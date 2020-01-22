/***
** Created by Aleksey Volkov on 16.01.2020.
***/

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include <esp_log.h>
#include <esp_system.h>
#include <driver/gpio.h>
#include <light.h>

#include "include/buttons.h"

static const char *TAG = "BUTTONS";
static xQueueHandle xQueueButton = NULL;

static void gpio_isr_handler(void *arg)
{
  uint32_t gpio_num = (uint32_t) arg;
  xQueueSendFromISR(xQueueButton, &gpio_num, NULL);
}

static void task_buttons(void *arg)
{
  uint32_t io_num;
  uint8_t active_preset = 0;
  uint8_t brightness_preset[4] = {0, 25, 50, 75};

  for (;;) {
    if (xQueueReceive(xQueueButton, &io_num, portMAX_DELAY)) {
      ESP_LOGI(TAG, "GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));

      /* button was released after press event */
      if (io_num == GPIO_NUM_0)
      {
        active_preset++;
        if (active_preset == 4) {
          active_preset = 0;
        }

        set_brightness(brightness_preset[active_preset]);
        ESP_LOGI(TAG, "set brightness preset: %d", brightness_preset[active_preset]);
      }

      vTaskDelay(200/portTICK_RATE_MS);
    }
  }
}

void init_buttons()
{
  /* GPIO_0 */
  gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
  gpio_set_pull_mode(GPIO_NUM_0, GPIO_PULLUP_ONLY);

  /* change gpio intrrupt type for one pin */
  gpio_set_intr_type(GPIO_NUM_0, GPIO_INTR_POSEDGE);

  ESP_LOGI(TAG, "[APP] free memory before start buttons event task %d bytes", esp_get_free_heap_size());

  /* create a queue to handle gpio event from isr */
  xQueueButton = xQueueCreate(10, sizeof(uint32_t));

  /* start gpio task */
  xTaskCreate(task_buttons, "task_buttons", 2048, NULL, 10, NULL);

  /* install gpio isr service */
  gpio_install_isr_service(0);

  /* hook isr handler for specific gpio pin */
  gpio_isr_handler_add(GPIO_NUM_0, gpio_isr_handler, (void *) GPIO_NUM_0);
}

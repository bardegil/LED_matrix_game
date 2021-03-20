/*
 * Example of a Arduino interruption and RTOS Binary Semaphore
 * https://www.freertos.org/Embedded-RTOS-Binary-Semaphores.html
 */


/*
 * Declaring a global variable of type SemaphoreHandle_t 
 * 
 */
#include "button_response.h"
#include <cstdio>




button_response::button_response(int button_pin,interrupt_response_function_t callback_function)
{
  char buf[20]
  sprintf(buf,"led_%d_task",button_pin);
  pinMode(button_pin, INPUT_PULLUP);
   // Create task for Arduino led 
  btn_sem = xSemaphoreCreateBinary();
  if (interruptSemaphore != NULL) {
    // Attach interrupt for Arduino digital pin
    attachInterrupt(digitalPinToInterrupt(button_pin), interruptHandler, LOW);
  }
  callback = callback_function;
  configASSERT(callback != NULL);
  task = xTaskCreate(TaskLed, // Task function
              buf, // Task name
              128, // Stack size 
              NULL, 
              4, // Priority
              NULL );
  configASSERT(task != NULL);  
}

button_response::~button_response()
{
  vTaskDelete(task);
}



void button_response::interruptHandler() {
  /**
   * Give semaphore in the interrupt handler
   * https://www.freertos.org/a00124.html
   */
  BaseType_t wakup = pdFALSE; 
  configASSERT(xSemaphoreGiveFromISR(btn_sem, &wakup) == pdPASS);
  if(wakup){
    portYIELD_FROM_ISR();
  }

}


/* 
 * Led task. 
 */
void button_response::TaskLed(void *pvParameters)
{
  (void) pvParameters;

  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) {
    
    /**
     * Take the semaphore.
     * https://www.freertos.org/a00122.html
     */
    if (xSemaphoreTake(interruptSemaphore, portMAX_DELAY) == pdPASS) {
      callback();
    }
    vTaskDelay(10);
  }
}
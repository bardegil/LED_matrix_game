/*
 * Example of a Arduino interruption and RTOS Binary Semaphore
 * https://www.freertos.org/Embedded-RTOS-Binary-Semaphores.html
 */


/*
 * Declaring a global variable of type SemaphoreHandle_t 
 * 
 */
#include "button_response.h"


static void taskfun(void* parm);
static void my_interruptfun();

button_response::button_response(int button_pin,interrupt_response_function_t callback_function)
{
  char buf[20];
  sprintf(buf,"led_%d_task",button_pin);
  pinMode(button_pin, INPUT_PULLUP);
   // Create task for Arduino led 
  btn_sem = xSemaphoreCreateBinary();
  if (btn_sem != NULL) {
    // Attach interrupt for Arduino digital pin
    attachInterrupt(digitalPinToInterrupt(button_pin), my_interruptfun, LOW);
  }
  callback = callback_function;
  configASSERT(callback != NULL);
  xTaskCreate(taskfun, // Task function
              buf, // Task name
              128, // Stack size 
              NULL, 
              4, // Priority
              &task );
  configASSERT(task != NULL);  
}

button_response::~button_response()
{
  vTaskDelete(task);
}

static void taskfun(void* parm) {
    static_cast<button_response *>(parm)->TaskLed();
}

static void my_interruptfun() {
    void* parm;
    static_cast<button_response *>(parm)->interruptHandler();
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
void button_response::TaskLed()
{
  //(void) pvParameters;

  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) {
    
    /**
     * Take the semaphore.
     * https://www.freertos.org/a00122.html
     */
    if (xSemaphoreTake(btn_sem, portMAX_DELAY) == pdPASS) {
      callback();
    }
    vTaskDelay(10);
  }
}
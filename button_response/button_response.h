// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>

// Include semaphore supoport
#include <semphr.h>
typedef void (* interrupt_response_function_t)();
class button_response
{
private:
  TaskHandle_t task;
  SemaphoreHandle_t btn_sem;
  interrupt_response_function_t callback;
  /* data */
public:
  button_response(/* args */);
  ~button_response();
  TaskLed(void * pvArgs);
  interruptHandler();
};

/* 
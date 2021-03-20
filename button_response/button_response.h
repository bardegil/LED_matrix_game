// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
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
    button_response(int button_pin,interrupt_response_function_t callback_function);
    ~button_response();
    void TaskLed();
     void interruptHandler();
};


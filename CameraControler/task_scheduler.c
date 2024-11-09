#include "task_scheduler.h"

typedef struct {
    void (*task)(void);
    uint32_t delay;
    uint32_t lastRun;
} TaskTypeDef;

#define MAX_TASKS 10

static TaskTypeDef tasks[MAX_TASKS];
static uint8_t taskCount = 0;

void AddTask(void (*task)(void), uint32_t delay) {
    if (taskCount < MAX_TASKS) {
        tasks[taskCount].task = task;
        tasks[taskCount].delay = delay;
        tasks[taskCount].lastRun = 0;
        taskCount++;
    }
}

void TaskScheduler_Run(void) {
    uint32_t currentTime = HAL_GetTick();
    
    for (uint8_t i = 0; i < taskCount; i++) {
        if (currentTime - tasks[i].lastRun >= tasks[i].delay) {
            tasks[i].task();
            tasks[i].lastRun = currentTime;
        }
    }
}

void TaskScheduler_Init(void) {
    taskCount = 0;
}

#ifndef __TASK_SCHEDULER_H
#define __TASK_SCHEDULER_H

#include <stdint.h>

void AddTask(void (*task)(void), uint32_t delay);

void TaskScheduler_Run(void);

void TaskScheduler_Init(void);

#endif /* __TASK_SCHEDULER_H */

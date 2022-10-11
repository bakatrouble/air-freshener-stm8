//
// Created by bakatrouble on 07/10/22.
//

#ifndef STM8TEST_MOTOR_H
#define STM8TEST_MOTOR_H

#include <stm8s_gpio.h>
#include "time.h"

void Motor_Init();
void Motor_SetSleep(bool status);
void Motor_Run(bool cw);
void Motor_Cycle();

#endif //STM8TEST_MOTOR_H

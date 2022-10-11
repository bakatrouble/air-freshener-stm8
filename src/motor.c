//
// Created by bakatrouble on 07/10/22.
//

#include "motor.h"
#include "config.h"


void Motor_Init() {
    GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5), GPIO_MODE_OUT_PP_HIGH_FAST);  // motor control
}

void Motor_SetSleep(bool status) {
    if (status) {
        GPIO_WriteLow(GPIOC, GPIO_PIN_3);
        GPIO_WriteLow(GPIOC, GPIO_PIN_4);
        GPIO_WriteLow(GPIOC, GPIO_PIN_5);
    } else {
        GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
    }
}

void Motor_Run(bool cw) {
    if (cw) {
        GPIO_WriteHigh(GPIOC, GPIO_PIN_4);
        GPIO_WriteLow(GPIOC, GPIO_PIN_5);
    } else {
        GPIO_WriteLow(GPIOC, GPIO_PIN_4);
        GPIO_WriteHigh(GPIOC, GPIO_PIN_5);
    }
    Motor_SetSleep(FALSE);
}

void Motor_Cycle() {
    Motor_Run(TRUE);
    delay_ms(config.downDuration);
    Motor_Run(FALSE);
    delay_ms(config.upDuration);
    Motor_SetSleep(TRUE);
}

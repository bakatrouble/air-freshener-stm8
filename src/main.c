#include <stm8s_gpio.h>
#include <string.h>
#include <stm8s_tim2.h>
#include "stm8s_clk.h"
#include "stm8s_uart1.h"
#include "stm8s_exti.h"
#include "stm8s_adc1.h"
#include "stm8s_flash.h"
#include "time.h"
#include "motor.h"
#include "config.h"
#include "console_io.h"

#define TIM2_PERIOD 124

typedef enum {
    INIT,
    IDLE,
    LIGHT_ON,
    FORCE_SPRAY,
} State;

uint16_t light_brightness;
State state = INIT;
uint32_t lightStart;
// interactive config

void CLK_Config();
void GPIO_Config();
void TIM2_Config();
void UART1_Config();
void ADC_Config();
void FLASH_Config();

void loop() {
    bool aboveThreshold = light_brightness > config.threshold;
    switch (state) {
        case IDLE:
            if (aboveThreshold) {
                lightStart = millis;
                state = LIGHT_ON;
                printf_tiny("light\r\n");
            }
            break;
        case LIGHT_ON:
            if (!aboveThreshold) {
                if (millis - lightStart > config.lightDuration) {
                    Motor_Cycle();
                } else {
                    printf_tiny("!dur\r\n");
                }
                state = IDLE;
            }
            break;
        case FORCE_SPRAY:
            printf_tiny("force\r\n");
            Motor_Cycle();
            state = IDLE;
            break;

    }
}

int main() {
    CLK_Config();
    GPIO_Config();
    UART1_Config();
    ADC_Config();
    TIM2_Config();
    FLASH_Config();
    Motor_Init();

    Config_Read();

    enableInterrupts();

    delay_ms(100);

    if (!GPIO_ReadInputPin(GPIOC, GPIO_PIN_6)) {
        Config_Interactive();
    }

    state = IDLE;

    uint8_t i = 0;

    while (1) {
        if (i % 16 == 0) {
            printf_tiny("brig %u\r\n", light_brightness);
        }
        i++;
        loop();
        delay_ms(100);
    }
}

void CLK_Config() {
    CLK_DeInit();
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void GPIO_Config() {
    EXTI_DeInit();
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_FALL);
    GPIO_Init(GPIOC, (GPIO_PIN_6 | GPIO_PIN_7), GPIO_MODE_IN_PU_IT);  // buttons
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);  // LED
}

void TIM2_Config() {
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_128, TIM2_PERIOD);
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    TIM2_Cmd(ENABLE);
}

void UART1_Config() {
    UART1_DeInit();
    UART1_Init((uint32_t)921600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
               UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
}

void ADC_Config() {
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);

    ADC1_DeInit();
    ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_3, ADC1_PRESSEL_FCPU_D18,
              ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
    ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);

    ADC1_StartConversion();
}

void FLASH_Config() {
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
}

void usr_btns_handler(void) __interrupt(5) {
    uint8_t val = GPIO_ReadInputData(GPIOC);
    uint8_t usr = (val & GPIO_PIN_6) == 0;
    uint8_t ext = (val & GPIO_PIN_7) == 0;
    if (usr && (state == IDLE || state == LIGHT_ON)) {
        state = FORCE_SPRAY;
    }
}

void tim2_handler(void) __interrupt(13) {
    Time_Increment();
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}

void adc1_handler(void) __interrupt(22) {
    light_brightness = ADC1_GetConversionValue();
    ADC1_ClearITPendingBit(ADC1_IT_EOC);
    if (light_brightness > config.threshold) {
        GPIO_WriteHigh(GPIOD, GPIO_PIN_3);
    } else {
        GPIO_WriteLow(GPIOD, GPIO_PIN_3);
    }
}
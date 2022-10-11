#include "time.h"

__IO uint16_t delayCounter = 0;
__IO uint32_t millis;

void delay_ms(uint16_t n_ms) {
    delayCounter = n_ms;
    while (delayCounter > 0);
}

void Time_Increment() {
    millis++;
    if (delayCounter > 0) {
        delayCounter--;
    }
}
//
// Created by bakatrouble on 12/10/22.
//

#ifndef STM8TEST_CONFIG_H
#define STM8TEST_CONFIG_H

#include <stm8s.h>

typedef struct {
    uint8_t version;
    uint16_t threshold;
    uint16_t downDuration;
    uint16_t upDuration;
    uint16_t lightDuration;
} config_t;

void Config_Interactive();
void Config_Read();

extern config_t config;

#endif //STM8TEST_CONFIG_H

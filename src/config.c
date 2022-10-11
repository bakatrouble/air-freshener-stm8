#include <stm8s_flash.h>
#include <string.h>
#include "config.h"
#include "console_io.h"
#include "motor.h"

uint8_t buf[16];
uint8_t *ptr = buf;
uint8_t c;
uint16_t val;

config_t config;

void Config_Write() {
    for (uint8_t i=0; i < sizeof(config_t); i++) {
        FLASH_ProgramByte(0x4000 + i, *((uint8_t*)(&config) + i));
    }
}

void Config_Read() {
    for (uint8_t i=0; i < sizeof(config_t); i++) {
        *((uint8_t*)(&config) + i) = FLASH_ReadByte(0x4000 + i);
    }

    if (config.version == 0) {
        memset(&config, 0, sizeof(config_t));
        config.version = 1;
        config.threshold = 50;
        config.downDuration = 500;
        config.upDuration = 500;
        config.lightDuration = 5000;
        Config_Write();
        printf_tiny("config initialized\r\n");
    }
}

uint16_t parse_uint() {
    ptr = buf + 1;
    uint16_t num = 0;
    while (*ptr != '\0') {
        num *= 10;
        if (*ptr < '0' || *ptr > '9') {
            printf_tiny("\r\ninv\r\n");
            return 0xFFFF;
        }
        num += *ptr - '0';
        ptr++;
    }
    return num;
}

void Config_Interactive() {
    printf_tiny("conf (qtudml):\r\n");
    while (1) {
        c = getc();
        switch (c) {
            case '\b':
                printf_tiny("\b \b");
                ptr--;
                break;
            case '\r':
                *ptr = '\0';
                if (ptr == buf) {
                    printf_tiny("emp\r\n");
                    break;
                }
                switch (buf[0]) {
                    case 'q':
                        printf_tiny("\r\nt=%u, d=%u, u=%u, l=%u\r\n", config.threshold, config.downDuration, config.upDuration, config.lightDuration);
                        break;
                    case 'm':
                        printf_tiny("\r\ncycling motor\r\n");
                        Motor_Cycle();
                        printf_tiny("done\r\n");
                        break;
                    case 't':
                        val = parse_uint();
                        if (val != 0xFFFF) {
                            config.threshold = val;
                            Config_Write();
                            printf_tiny("\r\ndone\r\n");
                        }
                        break;
                    case 'u':
                        val = parse_uint();
                        if (val != 0xFFFF) {
                            config.upDuration = val;
                            Config_Write();
                            printf_tiny("\r\ndone\r\n");
                        }
                        break;
                    case 'd':
                        val = parse_uint();
                        if (val != 0xFFFF) {
                            config.downDuration = val;
                            Config_Write();
                            printf_tiny("\r\ndone\r\n");
                        }
                        break;
                    case 'l':
                        val = parse_uint();
                        if (val != 0xFFFF) {
                            config.lightDuration = val;
                            Config_Write();
                            printf_tiny("\r\ndone\r\n");
                        }
                        break;
                    default:
                        printf_tiny("\r\nunk\r\n");
                        break;
                }
                ptr = buf;
                break;
            case '\x1b':
                break;
            default:
                if (ptr != buf + 15) {
                    *ptr++ = c;
                    putc(c);
                }
                break;
        }
    }
}

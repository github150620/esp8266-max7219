#include "driver/gpio.h"


#define ADDR_NOOP           0
#define ADDR_DIGIT0         1
#define ADDR_DIGIT1         2
#define ADDR_DIGIT2         3
#define ADDR_DIGIT3         4
#define ADDR_DIGIT4         5
#define ADDR_DIGIT5         6
#define ADDR_DIGIT6         7
#define ADDR_DIGIT7         8
#define ADDR_DECODEMODE     9
#define ADDR_INTENSITY      10
#define ADDR_SCANLIMIT      11
#define ADDR_SHUTDOWN       12
#define ADDR_DISPLAYTEST    15

int max7219_cs;
int max7219_clk;
int max7219_din;

int max7219_Write(uint8_t address, uint8_t data)
{
    int i;

    gpio_set_level(max7219_clk, 0);
    gpio_set_level(max7219_cs, 0);

    for (i=0;i<8;i++) {
        gpio_set_level(max7219_clk, 0);
        if (address & 0x80) {
            gpio_set_level(max7219_din, 1);
        } else {
            gpio_set_level(max7219_din, 0);
        }
        gpio_set_level(max7219_clk, 1);
        address <<= 1;
    }

    for (i=0;i<8;i++) {
        gpio_set_level(max7219_clk, 0);
        if (data & 0x80) {
            gpio_set_level(max7219_din, 1);
        } else {
            gpio_set_level(max7219_din, 0);
        }
        gpio_set_level(max7219_clk, 1);
        data <<= 1;
    }

    gpio_set_level(max7219_cs, 1);

    return 0;
}

int max7219_Init(int cs, int clk, int din)
{
    max7219_cs = cs;
    max7219_clk = clk;
    max7219_din = din;

    gpio_config_t config;
    config.intr_type = GPIO_INTR_DISABLE;
    config.mode = GPIO_MODE_OUTPUT;
    config.pin_bit_mask = (1ULL<<din) | (1ULL<<cs) | (1ULL<<clk);
    config.pull_down_en = 0;
    config.pull_up_en = 0;
    gpio_config(&config);

    gpio_set_level(max7219_cs, 1);

    max7219_Write(ADDR_DECODEMODE, 0xff);
    max7219_Write(ADDR_INTENSITY, 0);
    max7219_Write(ADDR_SCANLIMIT, 7);
    max7219_Write(ADDR_SHUTDOWN, 1);
    max7219_Write(ADDR_DISPLAYTEST, 0);

    return 0;
}

void app_main(void)
{
    max7219_Init(GPIO_NUM_0, GPIO_NUM_2, GPIO_NUM_3);
    max7219_Write(ADDR_DIGIT7, 1);
    max7219_Write(ADDR_DIGIT6, 2);
    max7219_Write(ADDR_DIGIT5, 3);
    max7219_Write(ADDR_DIGIT4, 4);
    max7219_Write(ADDR_DIGIT3, 5);
    max7219_Write(ADDR_DIGIT2, 6);
    max7219_Write(ADDR_DIGIT1, 7);
    max7219_Write(ADDR_DIGIT0, 8);
}


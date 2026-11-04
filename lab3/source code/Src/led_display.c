#include "led_display.h"

static const uint8_t segmentNumber[10] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,
    0x92, 0x82, 0xF8, 0x80, 0x90
};

static int led_buffer_1[2] = {0, 0};
static int led_buffer_2[2] = {0, 0};
static int current_led_index = 0;

void updateBuffer(int val1, int val2) {
    if (val1 > 99) val1 = 99;
    if (val2 > 99) val2 = 99;

    led_buffer_1[0] = val1 / 10;
    led_buffer_1[1] = val1 % 10;

    led_buffer_2[0] = val2 / 10;
    led_buffer_2[1] = val2 % 10;
}

static void display7SEG_1(int num) {
    uint8_t val = segmentNumber[num];
    HAL_GPIO_WritePin(GPIOB, a1_Pin, (val >> 0) & 0x01);
    HAL_GPIO_WritePin(GPIOB, b1_Pin, (val >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOB, c1_Pin, (val >> 2) & 0x01);
    HAL_GPIO_WritePin(GPIOB, d1_Pin, (val >> 3) & 0x01);
    HAL_GPIO_WritePin(GPIOB, e1_Pin, (val >> 4) & 0x01);
    HAL_GPIO_WritePin(GPIOB, f1_Pin, (val >> 5) & 0x01);
    HAL_GPIO_WritePin(GPIOB, g1_Pin, (val >> 6) & 0x01);
}

static void display7SEG_2(int num) {
    uint8_t val = segmentNumber[num];
    HAL_GPIO_WritePin(GPIOB, a2_Pin, (val >> 0) & 0x01);
    HAL_GPIO_WritePin(GPIOB, b2_Pin, (val >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOB, c2_Pin, (val >> 2) & 0x01);
    HAL_GPIO_WritePin(GPIOB, d2_Pin, (val >> 3) & 0x01);
    HAL_GPIO_WritePin(GPIOB, e2_Pin, (val >> 4) & 0x01);
    HAL_GPIO_WritePin(GPIOB, f2_Pin, (val >> 5) & 0x01);
    HAL_GPIO_WritePin(GPIOB, g2_Pin, (val >> 6) & 0x01);
}

void scan7SEG(void) {
    HAL_GPIO_WritePin(GPIOA, EN1_Pin | EN2_Pin | EN3_Pin | EN4_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB, a1_Pin|b1_Pin|c1_Pin|d1_Pin|e1_Pin|f1_Pin|g1_Pin |
                             a2_Pin|b2_Pin|c2_Pin|d2_Pin|e2_Pin|f2_Pin|g2_Pin, GPIO_PIN_SET);

    switch(current_led_index) {
        case 0:
            display7SEG_1(led_buffer_1[0]);
            HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            display7SEG_1(led_buffer_1[1]);
            HAL_GPIO_WritePin(GPIOA, EN2_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            display7SEG_2(led_buffer_2[0]);
            HAL_GPIO_WritePin(GPIOA, EN3_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            display7SEG_2(led_buffer_2[1]);
            HAL_GPIO_WritePin(GPIOA, EN4_Pin, GPIO_PIN_RESET);
            break;
    }

    current_led_index++;
    if (current_led_index >= 4) {
        current_led_index = 0;
    }
}

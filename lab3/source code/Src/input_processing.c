#include "input_processing.h"
#include "input_reading.h"
#include "led_display.h"

#define MODE_NORMAL       1
#define MODE_MODIFY_RED   2
#define MODE_MODIFY_AMBER 3
#define MODE_MODIFY_GREEN 4

#define ROAD1_GREEN  0
#define ROAD1_AMBER  1
#define ROAD2_GREEN  2
#define ROAD2_AMBER  3

#define MIN_DURATION 1
#define MAX_DURATION 99

static int red_duration    = 5;
static int yellow_duration = 2;
static int green_duration  = 3;

static int new_red_duration    = 5;
static int new_yellow_duration = 2;
static int new_green_duration  = 3;

static uint8_t red_set = 0, yellow_set = 0, green_set = 0;

static int mode = MODE_NORMAL;
static int traffic_state = ROAD1_GREEN;
static uint32_t phase_start_tick = 0;

static void set_traffic_lights(int state) {
    HAL_GPIO_WritePin(GPIOA, RED1_Pin | RED2_Pin | YELLOW1_Pin | YELLOW2_Pin | GREEN1_Pin | GREEN2_Pin, GPIO_PIN_RESET);
    switch (state) {
        case ROAD1_GREEN:
            HAL_GPIO_WritePin(GPIOA, GREEN1_Pin | RED2_Pin, GPIO_PIN_SET);
            break;
        case ROAD1_AMBER:
            HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin | RED2_Pin, GPIO_PIN_SET);
            break;
        case ROAD2_GREEN:
            HAL_GPIO_WritePin(GPIOA, RED1_Pin | GREEN2_Pin, GPIO_PIN_SET);
            break;
        case ROAD2_AMBER:
            HAL_GPIO_WritePin(GPIOA, RED1_Pin | YELLOW2_Pin, GPIO_PIN_SET);
            break;
    }
}

static uint32_t get_phase_duration(int state) {
    switch (state) {
        case ROAD1_GREEN: case ROAD2_GREEN: return (uint32_t)green_duration * 1000U;
        case ROAD1_AMBER: case ROAD2_AMBER: return (uint32_t)yellow_duration * 1000U;
        default: return 1000U;
    }
}

static void apply_new_durations(void) {
    int R = red_duration, Y = yellow_duration, G = green_duration;

    if (red_set && yellow_set && green_set) {
        R = new_red_duration;
        if (R < 2) R = 2;
        int sum = new_yellow_duration + new_green_duration;
        Y = (R * new_yellow_duration + sum / 2) / sum;
        if (Y < 1) Y = 1;
        if (Y > R - 1) Y = R - 1;
        G = R - Y;
    }
    else if (red_set && yellow_set) {
        R = new_red_duration;
        Y = new_yellow_duration;
        if (R < 2) R = 2;
        if (Y >= R) Y = R - 1;
        G = R - Y;
    }
    else if (red_set && green_set) {
        R = new_red_duration;
        G = new_green_duration;
        if (R < 2) R = 2;
        if (G >= R) G = R - 1;
        Y = R - G;
    }
    else if (red_set) {
        R = new_red_duration;
        if (R < 2) R = 2;
        if (Y >= R) Y = R - 1;
        G = R - Y;
    }
    else if (yellow_set && green_set) {
        Y = new_yellow_duration;
        G = new_green_duration;
        if (Y > MAX_DURATION - 1) Y = MAX_DURATION - 1;
        if (Y + G > MAX_DURATION) G = MAX_DURATION - Y;
        R = Y + G;
    }
    else if (yellow_set) {
        Y = new_yellow_duration;
        if (Y + G > MAX_DURATION) Y = MAX_DURATION - G;
        R = Y + G;
    }
    else if (green_set) {
        G = new_green_duration;
        if (Y + G > MAX_DURATION) G = MAX_DURATION - Y;
        R = Y + G;
    }

    red_duration = R;
    yellow_duration = Y;
    green_duration = G;

    red_set = yellow_set = green_set = 0;
}

static void traffic_fsm(void) {
    uint32_t now = HAL_GetTick();
    uint32_t elapsed = now - phase_start_tick;
    uint32_t duration = get_phase_duration(traffic_state);

    if (elapsed >= duration) {
        phase_start_tick = now;
        switch (traffic_state) {
            case ROAD1_GREEN: traffic_state = ROAD1_AMBER; break;
            case ROAD1_AMBER: traffic_state = ROAD2_GREEN; break;
            case ROAD2_GREEN: traffic_state = ROAD2_AMBER; break;
            case ROAD2_AMBER: traffic_state = ROAD1_GREEN; break;
        }
        set_traffic_lights(traffic_state);
        elapsed = 0;
        duration = get_phase_duration(traffic_state);
    }

    int time_left = (duration - elapsed) / 1000 + 1;
    int road1_time, road2_time;

    if (traffic_state == ROAD1_GREEN) {
        road1_time = time_left;
        road2_time = time_left + yellow_duration;
    } else if (traffic_state == ROAD2_GREEN) {
        road1_time = time_left + yellow_duration;
        road2_time = time_left;
    } else {
        road1_time = time_left;
        road2_time = time_left;
    }
    updateBuffer(road1_time, road2_time);
}

void fsm_for_input_processing(void) {
    static uint8_t is_initialized = 0;
    if (is_initialized == 0) {
        set_traffic_lights(traffic_state);
        phase_start_tick = HAL_GetTick();
        is_initialized = 1;
    }

    if (is_button_pressed(0)) {
        mode++;
        if (mode > MODE_MODIFY_GREEN) {
            mode = MODE_NORMAL;
            apply_new_durations();
            traffic_state = ROAD1_GREEN;
            phase_start_tick = HAL_GetTick();
            set_traffic_lights(traffic_state);
        }
        else if (mode == MODE_MODIFY_RED)   { if (!red_set)    new_red_duration    = red_duration; }
        else if (mode == MODE_MODIFY_AMBER) { if (!yellow_set) new_yellow_duration = yellow_duration; }
        else if (mode == MODE_MODIFY_GREEN){ if (!green_set)  new_green_duration  = green_duration; }

        if (mode != MODE_NORMAL) {
            HAL_GPIO_WritePin(GPIOA, RED1_Pin | RED2_Pin | YELLOW1_Pin | YELLOW2_Pin | GREEN1_Pin | GREEN2_Pin, GPIO_PIN_RESET);

        }
    }

    if (is_button_pressed(1)) {
        if (mode == MODE_MODIFY_RED) {
            if (++new_red_duration > MAX_DURATION) new_red_duration = MIN_DURATION;
        } else if (mode == MODE_MODIFY_AMBER) {
            if (++new_yellow_duration > MAX_DURATION) new_yellow_duration = MIN_DURATION;
        } else if (mode == MODE_MODIFY_GREEN) {
            if (++new_green_duration > MAX_DURATION) new_green_duration = MIN_DURATION;
        }
    }

    if (is_button_pressed(2)) {
        if (mode == MODE_MODIFY_RED)        red_set = 1;
        else if (mode == MODE_MODIFY_AMBER) yellow_set = 1;
        else if (mode == MODE_MODIFY_GREEN) green_set = 1;
    }

    if (mode == MODE_NORMAL) {
        traffic_fsm();
    } else {
        uint8_t toggle = (HAL_GetTick() / 250) % 2;   // 2Hz (chớp tắt 250ms ON / 250ms OFF)

        if (mode == MODE_MODIFY_RED) {
            updateBuffer(mode, new_red_duration);
            HAL_GPIO_WritePin(GPIOA, RED1_Pin | RED2_Pin, toggle ? GPIO_PIN_SET : GPIO_PIN_RESET);
        } else if (mode == MODE_MODIFY_AMBER) {
            updateBuffer(mode, new_yellow_duration);
            HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin | YELLOW2_Pin, toggle ? GPIO_PIN_SET : GPIO_PIN_RESET);
        } else if (mode == MODE_MODIFY_GREEN) {
            updateBuffer(mode, new_green_duration);
            HAL_GPIO_WritePin(GPIOA, GREEN1_Pin | GREEN2_Pin, toggle ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }
}

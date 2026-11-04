#include "input_reading.h"

#define NO_OF_BUTTONS       3
#define DEBOUNCE_TIME       2
#define LONG_PRESS_TIME    100

static GPIO_PinState button_pins[NO_OF_BUTTONS];
static unsigned char button_state[NO_OF_BUTTONS];
static GPIO_PinState previous_reading[NO_OF_BUTTONS];
static unsigned int debounce_counter[NO_OF_BUTTONS];
static unsigned char button_pressed_flag[NO_OF_BUTTONS];
static unsigned int hold_counter[NO_OF_BUTTONS];
static unsigned char button_pressed_1s_flag[NO_OF_BUTTONS];
static unsigned char initialized = 0;

void button_reading(void)
{
    GPIO_PinState current_reading[NO_OF_BUTTONS];

    current_reading[0] = HAL_GPIO_ReadPin(GPIOC, bt1_Pin);
    current_reading[1] = HAL_GPIO_ReadPin(GPIOC, bt2_Pin);
    current_reading[2] = HAL_GPIO_ReadPin(GPIOC, bt3_Pin);

    if (!initialized)
    {
        for (int i = 0; i < NO_OF_BUTTONS; i++)
        {
            previous_reading[i] = current_reading[i];
            button_pins[i] = current_reading[i];
            button_state[i] = (current_reading[i] == GPIO_PIN_RESET) ? 1 : 0;
            debounce_counter[i] = 0;
            hold_counter[i] = 0;
            button_pressed_flag[i] = 0;
            button_pressed_1s_flag[i] = 0;
        }
        initialized = 1;
        return;
    }

    for (int i = 0; i < NO_OF_BUTTONS; i++)
    {
        if (current_reading[i] != previous_reading[i])
        {
            previous_reading[i] = current_reading[i];
            debounce_counter[i] = 0;
        }
        else if (debounce_counter[i] < DEBOUNCE_TIME)
        {
            debounce_counter[i]++;
        }

        if (debounce_counter[i] >= DEBOUNCE_TIME)
        {
            unsigned char new_state = (current_reading[i] == GPIO_PIN_RESET) ? 1 : 0;

            if (new_state == 1 && button_state[i] == 0)
            {
                button_pressed_flag[i] = 1;
                hold_counter[i] = 0;
                button_pressed_1s_flag[i] = 0;
            }
            if (new_state == 0)
            {
                hold_counter[i] = 0;
                button_pressed_1s_flag[i] = 0;
            }
            button_state[i] = new_state;
        }

        if (button_state[i] == 1)
        {
            if (hold_counter[i] < LONG_PRESS_TIME)
            {
                hold_counter[i]++;
            }
            if (hold_counter[i] >= LONG_PRESS_TIME)
            {
                button_pressed_1s_flag[i] = 1;
            }
        }
    }
}

unsigned char is_button_pressed(unsigned char index)
{
    if (index >= NO_OF_BUTTONS) return 0;
    if (button_pressed_flag[index])
    {
        button_pressed_flag[index] = 0;
        return 1;
    }
    return 0;
}

unsigned char is_button_pressed_1s(unsigned char index)
{
    if (index >= NO_OF_BUTTONS) return 0;
    if (button_pressed_1s_flag[index])
    {
        button_pressed_1s_flag[index] = 0;
        return 1;
    }
    return 0;
}

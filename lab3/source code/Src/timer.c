#include "timer.h"
#include "input_reading.h"
#include "led_display.h"

static uint16_t logic_counter = 0;
static volatile uint8_t logic_flag = 0;

void timer_run(void)
{
    // 1. Bộ đếm làm chậm tốc độ quét LED 7 đoạn
    static uint8_t scan_delay = 0;
    scan_delay++;

    // Thay đổi số 2 thành 3, 4, hoặc 5... để LED quét chậm lại theo ý bạn
    if (scan_delay >= 50)
    {
        scan_delay = 0;
        scan7SEG(); // Nếu bằng 2, cứ 20ms mới đổi 1 LED thay vì 10ms
    }

    // 2. Tính toán chu kỳ đọc nút và chạy FSM (vẫn giữ đúng nhịp 10ms)
    logic_counter += TIMER_TICK_MS;
    if (logic_counter >= LOGIC_PERIOD_MS)
    {
        logic_counter = 0;
        button_reading();
        logic_flag = 1;
    }
}

uint8_t timer_logic_ready(void)
{
    if (logic_flag)
    {
        logic_flag = 0;
        return 1;
    }
    return 0;
}

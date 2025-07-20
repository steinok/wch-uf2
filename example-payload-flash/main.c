#include <stdint.h>
#include "led_effects.h"

#pragma GCC diagnostic ignored "-Wprio-ctor-dtor"
__attribute__((constructor(0))) static void setup_qingke() {
    asm volatile(
        "li t0, 0x1f\n"
        "csrw 0xbc0, t0\n"
    );
}

int main(void) {
    led_init();
    while (1) {
        for (int effect = 0; effect < LED_EFFECT_COUNT; effect++) {
            run_led_effect((led_effect_t)effect);
        }
    }
}

#ifndef LED_EFFECTS_H
#define LED_EFFECTS_H

#include <stdint.h>

typedef enum {
    LED_EFFECT_CONSTANT,
    LED_EFFECT_PULSATE,
    LED_EFFECT_BLINK_FAST,
    LED_EFFECT_BLINK_SLOW,
    LED_EFFECT_DOUBLE_BLINK,
    LED_EFFECT_TRIPLE_BLINK,
    LED_EFFECT_HEARTBEAT,
    LED_EFFECT_STROBE,
    LED_EFFECT_RAMP_UP,
    LED_EFFECT_RAMP_DOWN,
    LED_EFFECT_RANDOM,
    LED_EFFECT_COUNT
} led_effect_t;

void led_init(void);
void run_led_effect(led_effect_t effect);

#endif // LED_EFFECTS_H

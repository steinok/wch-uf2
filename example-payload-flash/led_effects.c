#include "led_effects.h"

#define R32_RCC_APB2PCENR   (*(volatile uint32_t*)0x40021018)
#define R32_GPIOA_CFGLR     (*(volatile uint32_t*)0x40010800)
#define R32_GPIOA_BSHR      (*(volatile uint32_t*)0x40010810)

static inline void led_on(void) {
    R32_GPIOA_BSHR = 1 << 0;
}

static inline void led_off(void) {
    R32_GPIOA_BSHR = 1 << 16;
}

static void delay_cycles(volatile uint32_t cycles) {
    while (cycles--)
        asm volatile("");
}

static uint32_t rand_state = 1;
static uint32_t rand_next(void) {
    rand_state = rand_state * 1664525 + 1013904223;
    return rand_state;
}

void led_init(void) {
    R32_RCC_APB2PCENR |= 1 << 2;
    R32_GPIOA_CFGLR = (R32_GPIOA_CFGLR & ~0xf) | (0b0010 << 0);
}

void run_led_effect(led_effect_t effect) {
    switch (effect) {
    case LED_EFFECT_CONSTANT:
        led_on();
        delay_cycles(800000);
        led_off();
        delay_cycles(200000);
        break;
    case LED_EFFECT_PULSATE:
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 1000; i++) {
                led_on();
                delay_cycles(i * 200);
                led_off();
                delay_cycles((1000 - i) * 200);
            }
        }
        break;
    case LED_EFFECT_BLINK_FAST:
        for (int i = 0; i < 20; i++) {
            led_on();
            delay_cycles(100000);
            led_off();
            delay_cycles(100000);
        }
        break;
    case LED_EFFECT_BLINK_SLOW:
        for (int i = 0; i < 10; i++) {
            led_on();
            delay_cycles(500000);
            led_off();
            delay_cycles(500000);
        }
        break;
    case LED_EFFECT_DOUBLE_BLINK:
        for (int i = 0; i < 8; i++) {
            led_on();
            delay_cycles(150000);
            led_off();
            delay_cycles(150000);
            led_on();
            delay_cycles(150000);
            led_off();
            delay_cycles(600000);
        }
        break;
    case LED_EFFECT_TRIPLE_BLINK:
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 3; j++) {
                led_on();
                delay_cycles(120000);
                led_off();
                delay_cycles(120000);
            }
            delay_cycles(600000);
        }
        break;
    case LED_EFFECT_HEARTBEAT:
        for (int i = 0; i < 8; i++) {
            led_on();
            delay_cycles(100000);
            led_off();
            delay_cycles(100000);
            led_on();
            delay_cycles(300000);
            led_off();
            delay_cycles(800000);
        }
        break;
    case LED_EFFECT_STROBE:
        for (int i = 0; i < 50; i++) {
            led_on();
            delay_cycles(50000);
            led_off();
            delay_cycles(50000);
        }
        break;
    case LED_EFFECT_RAMP_UP:
        for (int r = 0; r < 3; r++) {
            for (int i = 1; i <= 10; i++) {
                led_on();
                delay_cycles(i * 50000);
                led_off();
                delay_cycles(100000);
            }
        }
        break;
    case LED_EFFECT_RAMP_DOWN:
        for (int r = 0; r < 3; r++) {
            for (int i = 10; i >= 1; i--) {
                led_on();
                delay_cycles(i * 50000);
                led_off();
                delay_cycles(100000);
            }
        }
        break;
    case LED_EFFECT_RANDOM:
        for (int i = 0; i < 20; i++) {
            uint32_t on_d = (rand_next() % 10 + 1) * 100000;
            uint32_t off_d = (rand_next() % 10 + 1) * 100000;
            led_on();
            delay_cycles(on_d);
            led_off();
            delay_cycles(off_d);
        }
        break;
    default:
        break;
    }
}


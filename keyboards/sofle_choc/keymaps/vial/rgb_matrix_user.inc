RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

extern uint8_t led_boost[RGB_MATRIX_LED_COUNT];

bool pastel_pulse(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t global_v = rgb_matrix_get_val();
    uint8_t global_s = rgb_matrix_get_sat();
    uint8_t global_h = rgb_matrix_get_hue();
    uint8_t speed = rgb_matrix_get_speed();
    uint32_t time = (timer_read32() * speed) / 1024;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        uint8_t boost = led_boost[i];

        // Base brightness 127 + boost (clamped to 255)
        uint16_t v_total = 127 + boost;
        if (v_total > 255) v_total = 255;
        v_total = (v_total * global_v) / 255;

        // Saturation baseline 127 (half) + boost to full
        uint16_t s_total = 127 + boost;
        if (s_total > 255) s_total = 255;
        s_total = (s_total * global_s) / 255;

        // Use global hue drifting with time, plus slight variation per LED
        uint8_t hue = (uint8_t)(global_h + time + i);
        uint8_t sat = (uint8_t)s_total;
        uint8_t val = (uint8_t)v_total;

        HSV hsv = {hue, sat, val};
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < RGB_MATRIX_LED_COUNT;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

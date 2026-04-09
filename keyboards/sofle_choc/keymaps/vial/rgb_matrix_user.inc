RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

#define MAX_CONCURRENT_PULSES 4

typedef struct {
    uint16_t boost;
    uint8_t x;
    uint8_t y;
} pulse_t;

typedef struct {
    pulse_t pulses[MAX_CONCURRENT_PULSES];
} pulse_state_t;

extern pulse_state_t pulse_state;

bool pastel_pulse(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t global_v = rgb_matrix_get_val();
    uint8_t global_s = rgb_matrix_get_sat();
    uint32_t time = timer_read32() / 8; // Faster hue animation

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        uint16_t total_local_boost = 0;
        uint16_t min_dist = 0xFFFF;

        for (uint8_t j = 0; j < MAX_CONCURRENT_PULSES; j++) {
            if (pulse_state.pulses[j].boost > 0) {
                int16_t dx = (int16_t)g_led_config.point[i].x - (int16_t)pulse_state.pulses[j].x;
                int16_t dy = (int16_t)g_led_config.point[i].y - (int16_t)pulse_state.pulses[j].y;
                uint16_t dist = (uint16_t)((abs(dx) + abs(dy)) * 2 / 3);

                if (dist < 20) {
                    total_local_boost += pulse_state.pulses[j].boost * (20 - dist) / 20;
                }
                if (dist < min_dist) {
                    min_dist = dist;
                }
            }
        }

        if (total_local_boost > 255) total_local_boost = 255;

        // Base brightness 127 + total_local_boost
        uint16_t v_total = 127 + total_local_boost;
        if (v_total > 255) v_total = 255;
        v_total = (v_total * global_v) / 255;

        // Saturation baseline 127 (half) + boost to full
        uint16_t s_total = 127 + total_local_boost;
        if (s_total > 255) s_total = 255;
        s_total = (s_total * global_s) / 255;

        // Hue drifts with time and slightly with distance from the closest pulse
        uint8_t hue = (uint8_t)(time + (min_dist / 4));
        uint8_t sat = (uint8_t)s_total;
        uint8_t val = (uint8_t)v_total;

        HSV hsv = {hue, sat, val};
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < RGB_MATRIX_LED_COUNT;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

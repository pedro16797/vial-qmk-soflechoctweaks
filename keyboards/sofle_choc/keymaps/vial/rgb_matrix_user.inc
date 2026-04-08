RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

typedef struct {
    uint16_t boost;
    uint8_t x;
    uint8_t y;
} pulse_state_t;

extern pulse_state_t pulse_state;

bool pastel_pulse(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t global_v = rgb_matrix_get_val();
    uint32_t time = timer_read32() / 64;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        int16_t dx = (int16_t)g_led_config.point[i].x - (int16_t)pulse_state.x;
        int16_t dy = (int16_t)g_led_config.point[i].y - (int16_t)pulse_state.y;

        // Quick approx distance
        uint16_t dist = (uint16_t)((abs(dx) + abs(dy)) * 2 / 3);

        uint16_t local_boost = 0;
        if (pulse_state.boost > 0 && dist < 128) {
            local_boost = pulse_state.boost * (128 - dist) / 128;
        }

        // Base brightness 127 + local_boost
        uint16_t v_total = 127 + local_boost;
        if (v_total > 255) v_total = 255;

        // Scale by global brightness
        v_total = (v_total * global_v) / 255;

        // Use a consistent global hue that drifts with time,
        // and only slightly vary it by distance from the PRESS (not global coordinates)
        // to avoid whole-keyboard color jumping when origin changes.
        uint8_t hue = (uint8_t)(time + (dist / 8));
        uint8_t sat = 100;
        uint8_t val = (uint8_t)v_total;

        HSV hsv = {hue, sat, val};
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < RGB_MATRIX_LED_COUNT;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

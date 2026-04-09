RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

extern uint8_t led_boost[RGB_MATRIX_LED_COUNT];
extern layer_state_t layer_state;
extern uint32_t g_rgb_timer;

bool pastel_pulse(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t global_v = rgb_matrix_get_val();
    uint8_t global_s = rgb_matrix_get_sat();
    uint8_t global_h = rgb_matrix_get_hue();
    uint8_t speed = rgb_matrix_get_speed();

    uint8_t layer = get_highest_layer(layer_state);

    // Reverse time if in Lower (1) or Raise (2) layers
    int32_t time_raw = (int32_t)g_rgb_timer;
    if (layer == 1 || layer == 2) {
        time_raw = -time_raw;
    }
    uint32_t time = (uint32_t)(time_raw * speed) / 1024;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        uint8_t boost = led_boost[i];

        // Adjust layer logic: turn off RGB except for specific functional keys
        if (layer == 3) {
            bool is_functional = false;

            // Check if key at this LED index has a function in Adjust layer
            uint8_t row = g_led_config.matrix_co[i][0];
            uint8_t col = g_led_config.matrix_co[i][1];

            // Adjust layer keys:
            // F1-F4: Row 1, Col 1-4 (Left) and Row 6, Col 1-4 (Right)
            // F5-F8: Row 2, Col 1-4 (Left) and Row 7, Col 1-4 (Right)
            // F9-F12: Row 3, Col 1-4 (Left) and Row 8, Col 1-4 (Right)
            // BTN1/BTN2: Row 4, Col 5 (Left) and Row 9, Col 5 (Right) - based on keyboard.json center buttons

            if ((row == 1 || row == 6) && col >= 1 && col <= 4) is_functional = true;
            if ((row == 2 || row == 7) && col >= 1 && col <= 4) is_functional = true;
            if ((row == 3 || row == 8) && col >= 1 && col <= 4) is_functional = true;
            if ((row == 4 || row == 9) && col == 5) is_functional = true;

            if (!is_functional) {
                rgb_matrix_set_color(i, 0, 0, 0);
                continue;
            }
        }

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

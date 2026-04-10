RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

extern volatile uint8_t led_boost[RGB_MATRIX_LED_COUNT];
extern uint8_t led_to_row[RGB_MATRIX_LED_COUNT];
extern uint8_t led_to_col[RGB_MATRIX_LED_COUNT];
extern layer_state_t layer_state;
extern uint32_t g_rgb_timer;
extern bool is_keyboard_master(void);
extern bool is_keyboard_left(void);

bool pastel_pulse(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t global_v = rgb_matrix_get_val();
    uint8_t global_s = rgb_matrix_get_sat();
    uint8_t global_h = rgb_matrix_get_hue();
    uint8_t speed = rgb_matrix_get_speed();

    uint8_t layer = get_highest_layer(layer_state);

    // Reverse time if in Lower (1) or Raise (2) layers
    uint32_t time_base = (layer == 1 || layer == 2) ? (0 - g_rgb_timer) : g_rgb_timer;
    uint32_t time      = (uint32_t)((uint64_t)time_base * speed) / 1024;

    uint8_t limit = is_keyboard_master() ? RGB_MATRIX_LED_COUNT : 29;

    for (uint8_t i = led_min; i < led_max; i++) {
        if (i >= limit) break;
        RGB_MATRIX_TEST_LED_FLAGS();

        uint8_t g_i = is_keyboard_master() ? i : (is_keyboard_left() ? i : i + 29);
        if (g_i >= RGB_MATRIX_LED_COUNT) break;

        uint8_t level = led_boost[g_i];

        // Adjust layer logic: turn off RGB except for specific functional keys
        if (layer == 3) {
            bool is_functional = false;

            uint8_t row = led_to_row[g_i];
            uint8_t col = led_to_col[g_i];

            if (row != 255 && col != 255) {
                if (row == 1 && col >= 1 && col <= 4) is_functional = true;
                if (row == 2 && col >= 1 && col <= 4) is_functional = true;
                if (row == 3 && col >= 1 && col <= 4) is_functional = true;
                if (row == 4 && col == 5) is_functional = true;

                if (row == 6 && col >= 1 && col <= 4) is_functional = true;
                if (row == 7 && col >= 1 && col <= 4) is_functional = true;
                if (row == 8 && col >= 1 && col <= 4) is_functional = true;
                if (row == 9 && col == 5) is_functional = true;
            }

            if (!is_functional) {
                rgb_matrix_set_color(i, 0, 0, 0);
                continue;
            }
        }

        // level already contains baseline 127 + per-key boost
        uint16_t v_total = (level * global_v) / 255;
        uint16_t s_total = (level * global_s) / 255;

        uint8_t hue = (uint8_t)(global_h + time + g_i);
        uint8_t sat = (uint8_t)s_total;
        uint8_t val = (uint8_t)v_total;

        HSV hsv_final = {hue, sat, val};
        RGB rgb = hsv_to_rgb(hsv_final);
        rgb_matrix_set_color(g_i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < limit;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

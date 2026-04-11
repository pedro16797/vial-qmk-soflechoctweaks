RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

extern volatile uint8_t led_boost[RGB_MATRIX_LED_COUNT];
extern uint8_t led_to_row[RGB_MATRIX_LED_COUNT];
extern uint8_t led_to_col[RGB_MATRIX_LED_COUNT];
extern layer_state_t layer_state;
extern uint32_t g_rgb_timer;
extern bool is_keyboard_left(void);

static uint8_t lerp8(uint8_t a, uint8_t b, uint8_t t, uint8_t max_t) {
    if (t == 0) return a;
    if (t >= max_t) return b;
    return (uint8_t)((int16_t)a + ((int16_t)b - (int16_t)a) * (int16_t)t / (int16_t)max_t);
}

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

    uint8_t local_min = is_keyboard_left() ? 0 : 29;
    uint8_t local_max = is_keyboard_left() ? 29 : RGB_MATRIX_LED_COUNT;

    for (uint8_t i = led_min; i < led_max; i++) {
        if (i < local_min || i >= local_max) continue;
        RGB_MATRIX_TEST_LED_FLAGS();

        uint8_t boost = led_boost[i];

        // Adjust layer logic: turn off RGB except for specific functional keys
        if (layer == 3 && is_keyboard_left()) {
            uint8_t row = led_to_row[i];
            uint8_t col = led_to_col[i];

            if ((col >= 1 && col <= 4 && row >= 1 && row <= 8 && row != 4) ||
                (col == 5 && (row == 4 || row == 9))) {
                rgb_matrix_set_color(i - local_min, 255, 255, 255);
                continue;
            }
        }

        RGB final_rgb;
        if (boost < 128) {
            uint8_t level = 127 + boost;
            uint16_t v_total = (level * global_v) / 255;
            uint16_t s_total = (level * global_s) / 255;

            uint8_t hue = (uint8_t)(global_h + time + i);
            HSV hsv_final = {hue, (uint8_t)s_total, (uint8_t)v_total};
            final_rgb = hsv_to_rgb(hsv_final);
        } else {
            RGB rgb_start;
            RGB target;
            uint8_t t;

            if (boost < 160) {
                // Base color at boost 127 (level 254)
                uint16_t v_base = (254 * global_v) / 255;
                uint16_t s_base = (254 * global_s) / 255;
                uint8_t hue = (uint8_t)(global_h + time + i);
                HSV hsv_base = {hue, (uint8_t)s_base, (uint8_t)v_base};
                rgb_start = hsv_to_rgb(hsv_base);
                target = (RGB){255, 0, 0};
                t = boost - 128;
            } else if (boost < 192) {
                rgb_start = (RGB){255, 0, 0};
                target = (RGB){255, 255, 0};
                t = boost - 160;
            } else if (boost < 224) {
                rgb_start = (RGB){255, 255, 0};
                target = (RGB){255, 255, 255};
                t = boost - 192;
            } else {
                rgb_start = (RGB){255, 255, 255};
                target = (RGB){223, 223, 255};
                t = boost - 224;
            }
            final_rgb.r = lerp8(rgb_start.r, target.r, t, 31);
            final_rgb.g = lerp8(rgb_start.g, target.g, t, 31);
            final_rgb.b = lerp8(rgb_start.b, target.b, t, 31);
        }
        rgb_matrix_set_color(i - local_min, final_rgb.r, final_rgb.g, final_rgb.b);
    }
    return led_max < local_max;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

extern volatile uint8_t led_boost[RGB_MATRIX_LED_COUNT];
extern uint8_t led_to_row[RGB_MATRIX_LED_COUNT];
extern uint8_t led_to_col[RGB_MATRIX_LED_COUNT];
extern layer_state_t layer_state;
extern uint32_t g_rgb_timer;
extern bool is_keyboard_left(void);

bool pastel_pulse(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t global_v = rgb_matrix_get_val();
    uint8_t global_s = rgb_matrix_get_sat();
    uint8_t global_h = rgb_matrix_get_hue();
    uint8_t speed    = rgb_matrix_get_speed();

    uint8_t layer = get_highest_layer(layer_state);
    
    // Hoist keyboard side check out of the loop
    bool is_left = is_keyboard_left(); 
    bool is_layer3_left = (layer == 3 && is_left);

    uint32_t time_base = (layer == 1 || layer == 2) ? (0 - g_rgb_timer) : g_rgb_timer;
    uint32_t time = (uint32_t)(((uint64_t)time_base * speed) >> 10);

    uint8_t local_min = is_left ? 0 : 29;
    uint8_t local_max = is_left ? 29 : RGB_MATRIX_LED_COUNT;

    for (uint8_t i = led_min; i < led_max; i++) {
        if (i < local_min || i >= local_max) continue;
        RGB_MATRIX_TEST_LED_FLAGS();

        if (is_layer3_left) {
            uint8_t row = led_to_row[i];
            uint8_t col = led_to_col[i];

            if ((col >= 1 && col <= 4 && row >= 1 && row <= 8 && row != 4) ||
                (col == 5 && (row == 4 || row == 9))) {
                rgb_matrix_set_color(i - local_min, 255, 255, 255);
                continue;
            }
        }

        uint8_t boost = led_boost[i];
        uint8_t hue = (uint8_t)(global_h + time + i);
        RGB final_rgb;

        if (boost < 128) {
            uint8_t level = 127 + boost;
            uint8_t v_total = (level * global_v) >> 8;
            uint8_t s_total = (level * global_s) >> 8;
            final_rgb = hsv_to_rgb((HSV){hue, s_total, v_total});  
        } else if (boost < 160) {
            // Base to Red (255, 0, 0)
            RGB rgb_base = hsv_to_rgb((HSV){hue, global_s, global_v});
            uint8_t t = boost - 127;
            final_rgb.r = rgb_base.r + (((255 - rgb_base.r) * t) >> 5);
            final_rgb.g = rgb_base.g - ((rgb_base.g * t) >> 5);
            final_rgb.b = rgb_base.b - ((rgb_base.b * t) >> 5);
        } else if (boost < 192) {
            // Red to Yellow (255, 255, 0)
            uint8_t t = boost - 159;
            final_rgb.r = 255;
            final_rgb.g = (t << 3) + (t >> 2); 
            final_rgb.b = 0;
        } else if (boost < 224) {
            // Yellow to White (255, 255, 255)
            uint8_t t = boost - 191;
            final_rgb.r = 255;
            final_rgb.g = 255;
            final_rgb.b = (t << 3) + (t >> 2);
        } else {
            // White to Off-White (224, 224, 255)
            uint8_t t = boost - 223;
            final_rgb.r = 255 - t; 
            final_rgb.g = 255 - t;
            final_rgb.b = 255;
        }

        rgb_matrix_set_color(i - local_min, final_rgb.r, final_rgb.g, final_rgb.b);
    }
    return led_max < local_max;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

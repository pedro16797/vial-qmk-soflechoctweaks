RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

extern uint8_t led_boost[RGB_MATRIX_LED_COUNT];
extern uint32_t fkey_led_mask;
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
    // Only the low 8 bits are used, so a wrapping 32-bit multiply is exact
    uint8_t hue_base = (uint8_t)(global_h + ((time_base * speed) >> 10));

    // led_min/led_max are already limited to this half
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        if (is_layer3_left && (fkey_led_mask & (1UL << i))) {
            rgb_matrix_set_color(i, 255, 255, 255);
            continue;
        }

        uint8_t boost = led_boost[i];
        uint8_t hue = (uint8_t)(hue_base + i);
        RGB final_rgb;

        if (boost < 128) {
            uint8_t level = 127 + boost;
            uint8_t v_total = (level * global_v) >> 8;
            uint8_t s_total = (level * global_s) >> 8;
            final_rgb = hsv_to_rgb((HSV){hue, s_total, v_total});  
        } else {
            uint8_t t = boost & 31;
            uint8_t t_scale = (t << 3) | (t >> 2);

            if (boost < 160) {
                // Base to Red (255, 0, 0)
                RGB rgb_base = hsv_to_rgb((HSV){hue, global_s, global_v});
                final_rgb.r = rgb_base.r + (((255 - rgb_base.r) * (uint16_t)t_scale) >> 8);
                final_rgb.g = rgb_base.g - (((uint16_t)rgb_base.g * t_scale) >> 8);
                final_rgb.b = rgb_base.b - (((uint16_t)rgb_base.b * t_scale) >> 8);
            } else if (boost < 192) {
                // Red to Yellow (255, 255, 0)
                final_rgb.r = 255;
                final_rgb.g = t_scale;
                final_rgb.b = 0;
            } else if (boost < 224) {
                // Yellow to White (255, 255, 255)
                final_rgb.r = 255;
                final_rgb.g = 255;
                final_rgb.b = t_scale;
            } else {
                // White to Off-White (224, 224, 255)
                final_rgb.r = 255 - t;
                final_rgb.g = 255 - t;
                final_rgb.b = 255;
            }
        }

        rgb_matrix_set_color(i, final_rgb.r, final_rgb.g, final_rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

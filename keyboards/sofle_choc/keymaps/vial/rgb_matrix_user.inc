// Every mode is the same shader with features switched on or off, see pastel_mode_flags()
RGB_MATRIX_EFFECT(pastel_pulse)
RGB_MATRIX_EFFECT(pp_rainbow)
RGB_MATRIX_EFFECT(pp_expand)
RGB_MATRIX_EFFECT(pp_sideways)
RGB_MATRIX_EFFECT(pp_breath)
RGB_MATRIX_EFFECT(pp_heat)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include <stdlib.h>

extern uint8_t led_boost[RGB_MATRIX_LED_COUNT];
extern uint32_t fkey_led_mask;
extern layer_state_t layer_state;
extern uint32_t g_rgb_timer;
extern bool is_keyboard_left(void);

enum {
    PP_DRIFT   = 1 << 0, // Hue drifts with time
    PP_LAYERS  = 1 << 1, // Drift reverses on Lower/Raise, Adjust lights the F-keys
    PP_SPREAD  = 1 << 2, // Hue offset per LED index
    PP_X       = 1 << 3, // Hue offset by horizontal position
    PP_BOOST   = 1 << 4, // Keypress glow and heat colours, half-level resting brightness
    PP_BREATHE = 1 << 5, // Brightness follows a slow sine
};

// Feature set per mode; Solid Color (and anything unknown) is 0
uint8_t pastel_mode_flags(uint8_t mode) {
    switch (mode) {
        case RGB_MATRIX_CUSTOM_pastel_pulse: return PP_DRIFT | PP_LAYERS | PP_SPREAD | PP_BOOST;
        case RGB_MATRIX_CUSTOM_pp_rainbow:   return PP_DRIFT;
        case RGB_MATRIX_CUSTOM_pp_expand:    return PP_DRIFT | PP_SPREAD;
        case RGB_MATRIX_CUSTOM_pp_sideways:  return PP_DRIFT | PP_X;
        case RGB_MATRIX_CUSTOM_pp_breath:    return PP_BREATHE;
        case RGB_MATRIX_CUSTOM_pp_heat:      return PP_BOOST;
        default:                             return 0;
    }
}

// Colour of one LED, also used by the Adjust layer previews in keymap.c
rgb_t pastel_led_color(uint8_t flags, uint8_t i, hsv_t hsv, uint8_t time) {
    if (flags & PP_DRIFT) hsv.h += time;
    if (flags & PP_SPREAD) hsv.h += i;
    if (flags & PP_X) hsv.h += g_led_config.point[i].x;
    if (flags & PP_BREATHE) hsv.v = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
    if (!(flags & PP_BOOST)) return hsv_to_rgb(hsv);

    uint8_t boost = led_boost[i];
    if (boost < 128) {
        uint8_t level = 127 + boost;
        return hsv_to_rgb((hsv_t){.h = hsv.h, .s = (level * hsv.s) >> 8, .v = (level * hsv.v) >> 8});
    }

    uint8_t t       = boost & 31;
    uint8_t t_scale = (t << 3) | (t >> 2);
    rgb_t   rgb;
    if (boost < 160) {
        // Base to Red (255, 0, 0)
        rgb   = hsv_to_rgb(hsv);
        rgb.r = rgb.r + (((255 - rgb.r) * (uint16_t)t_scale) >> 8);
        rgb.g = rgb.g - (((uint16_t)rgb.g * t_scale) >> 8);
        rgb.b = rgb.b - (((uint16_t)rgb.b * t_scale) >> 8);
    } else if (boost < 192) {
        // Red to Yellow (255, 255, 0)
        rgb = (rgb_t){.r = 255, .g = t_scale, .b = 0};
    } else if (boost < 224) {
        // Yellow to White (255, 255, 255)
        rgb = (rgb_t){.r = 255, .g = 255, .b = t_scale};
    } else {
        // White to Off-White (224, 224, 255)
        rgb = (rgb_t){.r = 255 - t, .g = 255 - t, .b = 255};
    }
    return rgb;
}

static bool pastel_core(effect_params_t* params, uint8_t flags) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    hsv_t   hsv            = rgb_matrix_get_hsv();
    uint8_t layer          = (flags & PP_LAYERS) ? get_highest_layer(layer_state) : 0;
    bool    is_layer3_left = (layer == 3 && is_keyboard_left());

    uint32_t time_base = (layer == 1 || layer == 2) ? (0 - g_rgb_timer) : g_rgb_timer;
    // Only the low 8 bits are used, so a wrapping 32-bit multiply is exact
    uint8_t time = (time_base * rgb_matrix_get_speed()) >> 10;

    // led_min/led_max are already limited to this half
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        if (is_layer3_left && (fkey_led_mask & (1UL << i))) {
            rgb_matrix_set_color(i, 255, 255, 255);
            continue;
        }

        rgb_t rgb = pastel_led_color(flags, i, hsv, time);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

bool pastel_pulse(effect_params_t* params) { return pastel_core(params, pastel_mode_flags(RGB_MATRIX_CUSTOM_pastel_pulse)); }
bool pp_rainbow(effect_params_t* params) { return pastel_core(params, pastel_mode_flags(RGB_MATRIX_CUSTOM_pp_rainbow)); }
bool pp_expand(effect_params_t* params) { return pastel_core(params, pastel_mode_flags(RGB_MATRIX_CUSTOM_pp_expand)); }
bool pp_sideways(effect_params_t* params) { return pastel_core(params, pastel_mode_flags(RGB_MATRIX_CUSTOM_pp_sideways)); }
bool pp_breath(effect_params_t* params) { return pastel_core(params, pastel_mode_flags(RGB_MATRIX_CUSTOM_pp_breath)); }
bool pp_heat(effect_params_t* params) { return pastel_core(params, pastel_mode_flags(RGB_MATRIX_CUSTOM_pp_heat)); }

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

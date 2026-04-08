RGB_MATRIX_EFFECT(pastel_pulse)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

extern uint16_t boost_brightness;

bool pastel_pulse(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // Origin: Top-Center (approximate middle of the split)
    // Based on keyboard.json, X goes from 0 to 224, so 112 is center.
    uint8_t origin_x = 112;
    uint8_t origin_y = 0;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        int16_t dx = (int16_t)g_led_config.point[i].x - (int16_t)origin_x;
        int16_t dy = (int16_t)g_led_config.point[i].y - (int16_t)origin_y;

        // Calculate distance (squared is enough for a simple pulse effect, but let's use linear for better scaling)
        uint32_t dist_sq = (uint32_t)dx * dx + (uint32_t)dy * dy;
        uint16_t dist = 0;
        if (dist_sq > 0) {
            // Simple integer square root or just use the distance for hue variation
            // Since this is RP2040, we can afford some math, but let's keep it efficient.
            dist = (uint16_t)(dist_sq / 128); // Rough scaling
        }

        // Base brightness 127 + boost_brightness (clamped to 255)
        uint16_t v_total = 127 + boost_brightness;
        if (v_total > 255) v_total = 255;

        // Pastel characteristics: High Value, Low Saturation
        uint8_t hue = (uint8_t)(dist + (timer_read32() / 64));
        uint8_t sat = 100;
        uint8_t val = (uint8_t)v_total;

        HSV hsv = {hue, sat, val};
        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

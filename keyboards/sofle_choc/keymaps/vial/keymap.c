/* Copyright 2023 Brian Low
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include <string.h>
#include <stdlib.h>

#ifdef SPLIT_KEYBOARD
#    include "quantum/split_common/transactions.h"
#endif

#ifdef RGB_MATRIX_ENABLE
#    include "rgb_matrix.h"
#endif

enum layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST
};

const uint16_t PROGMEM keymaps[4][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(
    // Row 0
    KC_ESC,   RALT(KC_1), RALT(KC_2), RALT(KC_3), LSFT(KC_6), RALT(KC_E),                     KC_NUBS,    LSFT(KC_NUBS), KC_PSLS, LSFT(KC_1), LSFT(KC_MINUS), KC_MPLY,
    // Row 1
    KC_TAB,   KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,                           KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_BSPC,
    // Row 2
    KC_CAPS,  KC_A,       KC_S,       KC_D,       KC_F,       KC_G,                           KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_ENT,
    // Row 3 (Left Row 3, Center L Btn, Center R Btn, Right Row 3)
    KC_LSFT,  KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,    KC_MUTE,   RGB_TOG,    KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_QUOT,
    // Thumb Row (Left 1-5, Right 1-5)
                    KC_LGUI, KC_LALT, KC_LCTL, KC_SPACE, MO(_LOWER),                         MO(_RAISE), KC_SPACE, KC_DEL, KC_PGUP, TD(1)
),
[_LOWER] = LAYOUT(
    // Row 0
    KC_TRNS,  LSFT(KC_3), KC_GRV,     RALT(KC_4), RALT(KC_6), LSFT(KC_4),                       RALT(KC_LBRC), RALT(KC_RBRC), RALT(KC_GRV), KC_EQL, LSFT(KC_EQL), KC_TRNS,
    // Row 1
    LALT(KC_TAB), KC_KP_1, KC_KP_2,   KC_KP_3,    KC_PAST,    KC_PSLS,                          LSFT(KC_8), LSFT(KC_9), KC_UP, LSFT(KC_2), KC_MINUS, KC_TRNS,
    // Row 2
    KC_TRNS,  KC_KP_4,    KC_KP_5,    KC_KP_6,    KC_PPLS,    KC_PMNS,                          KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_TRNS,
    // Row 3
    KC_RSFT,  KC_KP_7,    KC_KP_8,    KC_KP_9,    KC_PDOT,    LSFT(KC_0), KC_TRNS,   KC_TRNS,   RALT(KC_QUOT), RALT(KC_BSLS), LSFT(KC_COMM), LSFT(KC_DOT), LSFT(KC_SLSH), KC_LBRC,
    // Thumb Row
                      KC_KP_0, LSFT(KC_5), LSFT(KC_LBRC), KC_ENT, KC_TRNS,   KC_TRNS, KC_TRNS, KC_INS, KC_PGDN, KC_TRNS
),
[_RAISE] = LAYOUT(
    // Row 0
    KC_TRNS,  LSFT(KC_3), KC_GRV,     RALT(KC_4), RALT(KC_6), LSFT(KC_4),                       RALT(KC_LBRC), RALT(KC_RBRC), RALT(KC_GRV), KC_EQL, LSFT(KC_EQL), KC_TRNS,
    // Row 1
    LALT(KC_TAB), KC_KP_1, KC_KP_2,   KC_KP_3,    KC_PAST,    KC_PSLS,                          LSFT(KC_8), LSFT(KC_9), KC_UP, LSFT(KC_2), KC_MINUS, KC_TRNS,
    // Row 2
    KC_TRNS,  KC_KP_4,    KC_KP_5,    KC_KP_6,    KC_PPLS,    KC_PMNS,                          KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_TRNS,
    // Row 3
    KC_RSFT,  KC_KP_7,    KC_KP_8,    KC_KP_9,    KC_PDOT,    LSFT(KC_0), KC_TRNS,   KC_TRNS,   RALT(KC_QUOT), RALT(KC_BSLS), LSFT(KC_COMM), LSFT(KC_DOT), LSFT(KC_SLSH), KC_LBRC,
    // Thumb Row
                      KC_KP_0, LSFT(KC_5), LSFT(KC_LBRC), KC_ENT, KC_TRNS,   KC_TRNS, KC_TRNS, KC_INS, KC_PGDN, KC_TRNS
),
[_ADJUST] = LAYOUT(
    // Row 0
    KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    // Row 1
    KC_TRNS,  KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_TRNS,                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    // Row 2
    KC_TRNS,  KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_TRNS,                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    // Row 3
    KC_TRNS,  KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_TRNS, KC_BTN2,   KC_BTN1,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    // Thumb Row
                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
)
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

volatile uint8_t led_boost[RGB_MATRIX_LED_COUNT];
uint8_t led_to_row[RGB_MATRIX_LED_COUNT];
uint8_t led_to_col[RGB_MATRIX_LED_COUNT];
uint32_t decay_timer = 0;

#ifdef OLED_ENABLE
static const char PROGMEM matrix_to_ascii[5][6] = {
    {0,   '1', '2', '3', '4', '5'},
    {0,   'Q', 'W', 'E', 'R', 'T'},
    {0,   'A', 'S', 'D', 'F', 'G'},
    {0,   'Z', 'X', 'C', 'V', 'B'},
    {0,   0,   0,   '_', 0,   0}
};

static const char PROGMEM matrix_to_ascii_right[5][6] = {
    {0,   '0', '9', '8', '7', '6'},
    {0,   'P', 'O', 'I', 'U', 'Y'},
    {0x1B, ';', 'L', 'K', 'J', 'H'},
    {0,   '/', '.', ',', 'M', 'N'},
    {0,   0,   0,   '_', 0,   0}
};

typedef struct {
    char     c;
    uint8_t  x;
    uint32_t timestamp;
} typing_char_t;

typing_char_t typing_buffer[10];
uint8_t       typing_buffer_index = 0;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

#endif

typedef struct {
    uint8_t row;
    uint8_t col;
} key_hit_t;

void apply_key_boost(uint8_t row, uint8_t col) {
    uint8_t led_index = g_led_config.matrix_co[row][col];
    if (led_index == NO_LED || led_index >= RGB_MATRIX_LED_COUNT) return;

    if (led_boost[led_index] + 4 > 255) {
        led_boost[led_index] = 255;
    } else {
        led_boost[led_index] += 4;
    }
}

#ifdef OLED_ENABLE
void add_to_buffer_at(char c, uint8_t x) {
    typing_buffer[typing_buffer_index].c         = c;
    typing_buffer[typing_buffer_index].x         = x;
    typing_buffer[typing_buffer_index].timestamp = timer_read32();
    typing_buffer_index                          = (typing_buffer_index + 1) % 10;
}
#endif

void keyboard_post_init_user(void) {
    decay_timer = timer_read32();

#ifdef OLED_ENABLE
    srand(timer_read32());
#endif

    // Initialize state
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        led_boost[i] = 0;
        led_to_row[i] = 255;
        led_to_col[i] = 255;
    }

    // Pre-populate LED to Matrix mapping for fast/safe shader lookup
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            uint8_t led = g_led_config.matrix_co[r][c];
            if (led != NO_LED && led < RGB_MATRIX_LED_COUNT) {
                led_to_row[led] = r;
                led_to_col[led] = c;
            }
        }
    }
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // Optimization: avoid oled_clear() or loops which are heavy on I2C/Split.
    // Overwrite the entire waterfall area (rows 0-11) and separator (12) in one go.
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("     \n     \n     \n     \n     \n     \n     \n     \n     \n     \n     \n     \n-----"), false);

    for (uint8_t i = 0; i < 10; i++) {
        typing_char_t tc = typing_buffer[i];
        if (tc.c != '\0') {
            uint32_t elapsed = timer_elapsed32(tc.timestamp);
            // Sliding up effect: Start at row 11 and slide towards row 0.
            // Speed: 1 row per 150ms
            int8_t row = 11 - (elapsed / 150);

            if (row >= 0 && row <= 11) {
                oled_set_cursor(tc.x, (uint8_t)row);
                if (tc.c == 0x1B) {
                    oled_write_P(PSTR("\x1B"), false);
                } else {
                    char buf[2] = {tc.c, '\0'};
                    oled_write(buf, false);
                }
            } else if (row < 0) {
                typing_buffer[i].c = '\0';
            }
        }
    }
    return false;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return true;
}

void housekeeping_task_user(void) {
    static uint32_t visual_scan_timer = 0;
    if (timer_elapsed32(visual_scan_timer) >= 10) {
        visual_scan_timer = timer_read32();

        static matrix_row_t last_matrix[MATRIX_ROWS];
        bool is_left = is_keyboard_left();
        uint8_t r_min = is_left ? 0 : (MATRIX_ROWS / 2);
        uint8_t r_max = is_left ? (MATRIX_ROWS / 2) : MATRIX_ROWS;

        for (uint8_t r = r_min; r < r_max; r++) {
            matrix_row_t current_row = matrix_get_row(r);
            matrix_row_t diff        = current_row & ~last_matrix[r];
            if (diff) {
                for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                    if (diff & (1 << c)) {
                        apply_key_boost(r, c);
#ifdef OLED_ENABLE
                        char c_ascii = 0;
                        if (is_left) {
                            c_ascii = pgm_read_byte(&matrix_to_ascii[r][c]);
                        } else {
                            c_ascii = pgm_read_byte(&matrix_to_ascii_right[r % 5][c]);
                        }
                        if (c_ascii) {
                            add_to_buffer_at(c_ascii, rand() % 5);
                        }
#endif
                    }
                }
            }
            last_matrix[r] = current_row;
        }
    }

    uint32_t tickLength = 200;
    uint32_t elapsed = timer_elapsed32(decay_timer);
    if (elapsed >= tickLength) {
        uint32_t ticks = elapsed / tickLength;
        decay_timer += ticks * tickLength;
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
            if (led_boost[i] > 0) {
                uint8_t delta = led_boost[i];
                if (ticks >= delta) {
                    led_boost[i] = 0;
                } else {
                    led_boost[i] -= (uint8_t)ticks;
                }
            }
        }
    }
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[4][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_WH_D, KC_WH_U) },
    [_LOWER]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_WH_L, KC_WH_R) },
    [_RAISE]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_WH_L, KC_WH_R) },
    [_ADJUST] = { ENCODER_CCW_CW(KC_MS_L, KC_MS_R), ENCODER_CCW_CW(KC_MS_D, KC_MS_U) },
};
#endif

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
#include "vial.h"
#include "dynamic_keymap.h"
#include <string.h>
#include <stdlib.h>

#define VIAL_INIT_MAGIC 0x564B

// Vial resets its EEPROM on the first boot of every new build (random BUILD_ID),
// so defaults are re-applied whenever that happens
static void init_vial_defaults(void) {
    vial_tap_dance_entry_t td0 = {
        .on_tap = KC_MPLY,
        .on_hold = KC_NO,
        .on_double_tap = KC_MNXT,
        .on_tap_hold = KC_MNXT,
        .custom_tapping_term = 200
    };
    dynamic_keymap_set_tap_dance(0, &td0);

    vial_tap_dance_entry_t td1 = {
        .on_tap = KC_NO,
        .on_hold = KC_SLEP,
        .on_double_tap = KC_PWR,
        .on_tap_hold = KC_PWR,
        .custom_tapping_term = 200
    };
    dynamic_keymap_set_tap_dance(1, &td1);

    vial_combo_entry_t combo0 = {
        .input = { KC_C, KC_S, KC_NO, KC_NO },
        .output = KC_BSLS
    };
    dynamic_keymap_set_combo(0, &combo0);

    eeconfig_update_user(VIAL_INIT_MAGIC);
    vial_init();
}

static bool via_was_reset = false;

void via_init_kb(void) {
    // Runs just before via_init() checks the magic and resets on mismatch
    via_was_reset = !via_eeprom_is_valid();
}

#include "assets/lyr_qwerty.h"
#include "assets/lyr_lower.h"
#include "assets/lyr_raise.h"
#include "assets/lyr_adjust.h"
#include "assets/mod_shift.h"
#include "assets/mod_ctrl.h"
#include "assets/mod_alt.h"
#include "assets/mod_gui.h"
#include "assets/lock_caps_on.h"
#include "assets/lock_caps_off.h"
#include "assets/img_empty_32.h"
#include "assets/img_empty_12.h"

#ifdef SPLIT_KEYBOARD
#    include "quantum/split_common/transactions.h"
#    include "quantum/split_common/split_util.h"
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
    KC_ESC,   RALT(KC_1), RALT(KC_2), RALT(KC_3), LSFT(KC_6), RALT(KC_E),                     KC_NUBS,    LSFT(KC_NUBS), KC_PSLS, LSFT(KC_1), LSFT(KC_MINUS), TD(0),
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

uint8_t  led_boost[RGB_MATRIX_LED_COUNT];
uint32_t fkey_led_mask = 0; // Left-half LEDs lit white on _ADJUST
uint32_t decay_timer   = 0;

static bool    boost_active = false;
static uint8_t local_row_min, local_led_min, local_led_max;

#ifdef OLED_ENABLE
static const char PROGMEM matrix_to_ascii[5][6] = {
    {0, '|', '@', '#', '&', '$'},
    {0, 'Q', 'W', 'E', 'R', 'T'},
    {0, 'A', 'S', 'D', 'F', 'G'},
    {0, 'Z', 'X', 'C', 'V', 'B'},
    {0,  0 ,  0 , '0',  0 ,  0 }
};

static const char PROGMEM matrix_to_ascii_right[5][6] = {
    {0, '?', '!', '/', '>', '<'},
    {0, 'P', 'O', 'I', 'U', 'Y'},
    {0, 'N', 'L', 'K', 'J', 'H'},
    {0, '-', '.', ',', 'M', 'N'},
    {0,  0,   0,   0 ,  0 ,  0 }
};

typedef struct {
    char     c;
    uint8_t  x;
    uint32_t timestamp;
} typing_char_t;

// Rows advance at the OLED refresh rate so every step is drawn
#define WATERFALL_ROW_MS OLED_UPDATE_INTERVAL

typing_char_t typing_buffer[32];
uint8_t       typing_buffer_index = 0;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_clear();
    oled_off();
    return OLED_ROTATION_270;
}

static bool render_status_slave(void) {
    static layer_state_t last_layer_state = 0;
    static uint8_t       last_mods        = 0;
    static bool          last_caps        = false;
    static bool          first_run        = true;

    layer_state_t current_layer_state = layer_state;
    uint8_t       current_mods        = get_mods();
#ifdef ONESHOT_ENABLE
    current_mods |= get_oneshot_mods();
#endif
    bool current_caps = host_keyboard_led_state().caps_lock;

    if (!first_run && current_layer_state == last_layer_state && current_mods == last_mods && current_caps == last_caps) {
        return false;
    }

    last_layer_state = current_layer_state;
    last_mods        = current_mods;
    last_caps        = current_caps;
    first_run        = false;

    oled_set_cursor(0, 0);
    switch (get_highest_layer(current_layer_state)) {
        case _QWERTY: oled_write_raw_P(img_lyr_qwerty, 128); break;
        case _LOWER:  oled_write_raw_P(img_lyr_lower, 128); break;
        case _RAISE:  oled_write_raw_P(img_lyr_raise, 128); break;
        case _ADJUST: oled_write_raw_P(img_lyr_adjust, 128); break;
        default:      oled_write_raw_P(img_empty_32, 128); break;
    }

    // Layer 0-3, mods 4-11 (2 lines each), caps 12-15
    oled_set_cursor(0, 4);
    oled_write_raw_P((current_mods & MOD_MASK_SHIFT) ? img_mod_shift : img_empty_12, 64);
    oled_set_cursor(0, 6);
    oled_write_raw_P((current_mods & MOD_MASK_CTRL) ? img_mod_ctrl : img_empty_12, 64);
    oled_set_cursor(0, 8);
    oled_write_raw_P((current_mods & MOD_MASK_ALT) ? img_mod_alt : img_empty_12, 64);
    oled_set_cursor(0, 10);
    oled_write_raw_P((current_mods & MOD_MASK_GUI) ? img_mod_gui : img_empty_12, 64);
    oled_set_cursor(0, 12);
    oled_write_raw_P(current_caps ? img_lock_caps_on : img_lock_caps_off, 128);

    return true;
}

#endif

void apply_key_boost(uint8_t row, uint8_t col) {
    uint8_t led_index = g_led_config.matrix_co[row][col];
    if (led_index == NO_LED || led_index >= RGB_MATRIX_LED_COUNT) return;

    uint8_t current_boost = led_boost[led_index];
    uint8_t add           = 16;
    if (current_boost & 128) add >>= 2;
    if (current_boost & 64) add >>= 1;

    if ((uint16_t)current_boost + add > 255) {
        led_boost[led_index] = 255;
    } else {
        led_boost[led_index] += add;
    }
    boost_active = true;
}

#ifdef OLED_ENABLE
static bool waterfall_idle = true;

void add_to_buffer_at(char c, uint8_t x) {
    typing_buffer[typing_buffer_index].c         = c;
    typing_buffer[typing_buffer_index].x         = x;
    typing_buffer[typing_buffer_index].timestamp = timer_read32();
    typing_buffer_index                          = (typing_buffer_index + 1) & 0x1F;
    waterfall_idle                               = false;
}
#endif

void keyboard_post_init_user(void) {
    decay_timer = timer_read32();

    // Each half only scans and decays its own rows/LEDs
    const uint8_t split[] = RGB_MATRIX_SPLIT;
    bool          is_left = is_keyboard_left();
    local_row_min         = is_left ? 0 : MATRIX_ROWS / 2;
    local_led_min         = is_left ? 0 : split[0];
    local_led_max         = is_left ? split[0] : RGB_MATRIX_LED_COUNT;

    // F1-F12 block: rows 1-3, cols 1-4 of the left half
    for (uint8_t r = 1; r <= 3; r++) {
        for (uint8_t c = 1; c <= 4; c++) {
            uint8_t led = g_led_config.matrix_co[r][c];
            if (led < 32) fkey_led_mask |= 1UL << led;
        }
    }

    // Effect indices saved before built-in effects were trimmed may now be out of range
    if (rgb_matrix_get_mode() >= RGB_MATRIX_EFFECT_MAX) {
        rgb_matrix_mode(RGB_MATRIX_DEFAULT_MODE);
    }

    // One-time initialization for Vial defaults
    if (is_keyboard_master()) {
        if (via_was_reset || (uint16_t)eeconfig_read_user() != VIAL_INIT_MAGIC) {
            init_vial_defaults();
        }
    }
}

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    if (!is_keyboard_master()) {
        // No master yet (PC off), stay dark while the split watchdog retries
        if (!split_watchdog_check()) {
            oled_off();
            return false;
        }
        if (render_status_slave()) {
            oled_on();
        }
        return false;
    }

    // Screen is already blank, nothing to do until the next keypress
    if (waterfall_idle) {
        if (is_oled_on()) oled_off();
        return false;
    }

    // Only cells that changed since the last frame are written, keeping I2C traffic minimal
    static char shown[16][5];
    char        grid[16][5] = {{0}};
    bool        any_active  = false;

    for (uint8_t i = 0; i < 32; i++) {
        if (typing_buffer[i].c == '\0') continue;
        uint32_t elapsed = timer_elapsed32(typing_buffer[i].timestamp);
        if (elapsed >= WATERFALL_ROW_MS * 16) {
            typing_buffer[i].c = '\0';
            continue;
        }
        // Sliding up effect: Start at row 15 and slide towards row 0
        uint8_t row = 15 - (uint8_t)((elapsed * (65536 / WATERFALL_ROW_MS)) >> 16);
        grid[row][typing_buffer[i].x] = typing_buffer[i].c;
        any_active = true;
    }

    for (uint8_t r = 0; r < 16; r++) {
        for (uint8_t x = 0; x < 5; x++) {
            if (grid[r][x] == shown[r][x]) continue;
            shown[r][x] = grid[r][x];
            oled_set_cursor(x, r);
            oled_write_char(grid[r][x] ? grid[r][x] : ' ', false);
        }
    }

    if (any_active) {
        oled_on();
    } else {
        waterfall_idle = true;
        oled_off();
    }
    return false;
}
#endif

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // No master yet (PC off), keep the LEDs dark while the split watchdog retries
    if (!is_keyboard_master() && !split_watchdog_check()) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
        return false;
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef OLED_ENABLE
    // Skip combo/encoder events, their row/col are not matrix positions
    if (is_keyboard_master() && record->event.pressed && IS_KEYEVENT(record->event)) {
        uint8_t r = record->event.key.row;
        uint8_t c = record->event.key.col;
        char    c_ascii = 0;
        if (r < 5) {
            c_ascii = pgm_read_byte(&matrix_to_ascii[r][c]);
        } else {
            c_ascii = pgm_read_byte(&matrix_to_ascii_right[r - 5][c]);
        }
        if (c_ascii) {
            add_to_buffer_at(c_ascii, (uint8_t)(timer_read32() % 5));
        }
    }
#endif
    return true;
}

void housekeeping_task_user(void) {
    // timer_read32() does a 64-bit division on ChibiOS, read it once per loop
    uint32_t now = timer_read32();

    static uint32_t visual_scan_timer = 0;
    if (TIMER_DIFF_32(now, visual_scan_timer) >= 10) {
        visual_scan_timer = now;

        static matrix_row_t last_matrix[MATRIX_ROWS];
        for (uint8_t r = local_row_min; r < local_row_min + MATRIX_ROWS / 2; r++) {
            matrix_row_t current_row = matrix_get_row(r);
            matrix_row_t diff        = current_row & ~last_matrix[r];
            if (diff) {
                for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                    if (diff & (1 << c)) {
                        apply_key_boost(r, c);
                    }
                }
            }
            last_matrix[r] = current_row;
        }
    }

    // Nothing is glowing, keep the decay clock parked
    if (!boost_active) {
        decay_timer = now;
        return;
    }

    uint32_t elapsed = TIMER_DIFF_32(now, decay_timer);
    if (elapsed >= 256) {
        uint32_t ticks = elapsed >> 8;
        decay_timer += ticks << 8;
        boost_active = false;
        for (uint8_t i = local_led_min; i < local_led_max; i++) {
            if (led_boost[i] > ticks) {
                led_boost[i] -= (uint8_t)ticks;
                boost_active = true;
            } else {
                led_boost[i] = 0;
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

void eeconfig_init_user(void) {
    init_vial_defaults();
}

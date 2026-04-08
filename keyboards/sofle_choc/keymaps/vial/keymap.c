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
    /* Sprint 1.3: Tri-layer logic for MO(_LOWER) + MO(_RAISE) = MO(_ADJUST) */
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

/* Sprint 2.2: Brightness Buffer */
uint16_t boost_brightness = 0;

void boost_brightness_sync_handler(uint8_t initiator2target_length, const void* initiator2target_buffer, uint8_t target2initiator_length, void* target2initiator_buffer) {
    if (initiator2target_length == sizeof(boost_brightness)) {
        memcpy(&boost_brightness, initiator2target_buffer, sizeof(boost_brightness));
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(BOOST_BRIGHTNESS_SYNC, boost_brightness_sync_handler);
}

/* Sprint 2.3: Additive "Boost" Logic */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        boost_brightness += 51;
        if (boost_brightness > 255) {
            boost_brightness = 255;
        }
    }
    return true;
}

/* Sprint 2.4: Linear Decay Math */
void matrix_scan_user(void) {
    static uint32_t decay_timer = 0;
    static uint16_t last_boost_brightness = 0;

    if (is_keyboard_master()) {
        if (timer_elapsed32(decay_timer) > 10) {
            decay_timer = timer_read32();
            if (boost_brightness >= 5) {
                boost_brightness -= 5;
            } else {
                boost_brightness = 0;
            }
        }

        if (boost_brightness != last_boost_brightness) {
            if (transaction_rpc_send(BOOST_BRIGHTNESS_SYNC, sizeof(boost_brightness), &boost_brightness)) {
                last_boost_brightness = boost_brightness;
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

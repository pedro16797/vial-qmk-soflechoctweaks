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

/*
 * Note on Vial Dynamic Features:
 * VIAL_TAP_DANCE_ENTRIES and VIAL_COMBO_ENTRIES are defined in config.h.
 * The Vial core automatically manages the tap_dance_actions[] and key_combos[] arrays.
 * Hardcoding these arrays in this file would cause "multiple definition" linker errors.
 * TD(0) and TD(1) in the keymap are placeholders for these dynamic entries.
 */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(
    KC_ESC,   RALT(KC_1), RALT(KC_2), RALT(KC_3), LSFT(KC_6), RALT(KC_E),                       KC_NUBS, LSFT(KC_NUBS), KC_PSLS, LSFT(KC_1), LSFT(KC_MINUS), KC_MPLY,
    KC_TAB,   KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,                             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_CAPS,  KC_A,       KC_S,       KC_D,       KC_F,       KC_G,                             KC_H,    KC_J,    KC_K,    KC_L,    KC_SCOLON, KC_ENT,
    KC_LSFT,  KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,    KC_MUTE,   RGB_TOG,      KC_N,    KC_M,    KC_COMMA, KC_DOT, KC_SLSH, KC_QUOT,
                      KC_LGUI, KC_LALT, KC_LCTL, KC_SPACE, MO(_LOWER),   KC_SPACE, MO(_RAISE), KC_PGUP, KC_DEL, TD(1)
),
[_LOWER] = LAYOUT(
    KC_TRNS,  LSFT(KC_3), KC_GRV,     RALT(KC_4), RALT(KC_6), LSFT(KC_4),                       LSFT(KC_4), RALT(KC_6), RALT(KC_4), KC_GRV, LSFT(KC_3), KC_TRNS,
    LALT(KC_TAB), KC_1,   KC_2,       KC_3,       KC_PAST,    KC_PSLS,                          KC_HOME, KC_UP,   KC_END,  KC_PGUP, KC_NO,   KC_NO,
    KC_TRNS,  KC_4,       KC_5,       KC_6,       KC_PPLS,    KC_PMNS,                          KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_NO,   KC_NO,
    KC_RSFT,  KC_7,       KC_8,       KC_9,       KC_PDOT,    LSFT(KC_0), KC_TRNS,   KC_TRNS,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_RSFT,
                      KC_0, LSFT(KC_5), LSFT(KC_LBRC), KC_PENT, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),
[_RAISE] = LAYOUT(
    KC_TRNS,  LSFT(KC_3), KC_GRV,     RALT(KC_4), RALT(KC_6), LSFT(KC_4),                       LSFT(KC_4), RALT(KC_6), RALT(KC_4), KC_GRV, LSFT(KC_3), KC_TRNS,
    LALT(KC_TAB), KC_F1,  KC_F2,      KC_F3,      KC_F4,      KC_F5,                            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    KC_TRNS,  KC_F12,     KC_NO,      KC_NO,      KC_NO,      KC_NO,                            KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    KC_TRNS,  KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,    KC_TRNS,   KC_TRNS,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_RSFT,
                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),
[_ADJUST] = LAYOUT(
    KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS,  KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_TRNS,                          KC_TRNS, KC_F4,   KC_F3,   KC_F2,   KC_F1,   KC_TRNS,
    KC_TRNS,  KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_TRNS,                          KC_TRNS, KC_F8,   KC_F7,   KC_F6,   KC_F5,   KC_TRNS,
    KC_TRNS,  KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_TRNS, KC_BTN2,   KC_BTN1,      KC_TRNS, KC_F12,  KC_F11,  KC_F10,  KC_F9,   KC_TRNS,
                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
)
};

layer_state_t layer_state_set_user(layer_state_t state) {
    /* Sprint 1.3: Tri-layer logic for MO(_LOWER) + MO(_RAISE) = MO(_ADJUST) */
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_WH_D, KC_WH_U) },
    [_LOWER]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_WH_L, KC_WH_R) },
    [_RAISE]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_WH_L, KC_WH_R) },
    [_ADJUST] = { ENCODER_CCW_CW(KC_MS_L, KC_MS_R), ENCODER_CCW_CW(KC_MS_D, KC_MS_U) },
};
#endif

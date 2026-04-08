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

enum tap_dance_codes {
    TD0 = 0,
    TD1,
};

// TD(0): ["KC_MPLY", "KC_NO", "KC_MNXT", "KC_MNXT", 200]
void td0_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        if (!state->pressed) tap_code16(KC_MPLY);
    } else if (state->count >= 2) {
        if (state->pressed) register_code16(KC_MNXT);
        else tap_code16(KC_MNXT);
    }
}
void td0_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count >= 2 && state->pressed) unregister_code16(KC_MNXT);
}

// TD(1): ["KC_NO", "KC_SLEP", "KC_PWR", "KC_PWR", 200]
void td1_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        if (state->pressed) register_code16(KC_SLEP);
    } else if (state->count >= 2) {
        if (state->pressed) register_code16(KC_PWR);
        else tap_code16(KC_PWR);
    }
}
void td1_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        if (state->pressed) unregister_code16(KC_SLEP);
    } else if (state->count >= 2) {
        if (state->pressed) unregister_code16(KC_PWR);
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD0] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td0_finished, td0_reset),
    [TD1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td1_finished, td1_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT(
    KC_ESC,   RALT(KC_1), RALT(KC_2), RALT(KC_3), LSFT(KC_6), RALT(KC_E),                       KC_NONUS_BACKSLASH, LSFT(KC_NONUS_BACKSLASH), KC_PSLS, LSFT(KC_1), LSFT(KC_MINUS), KC_MPLY,
    KC_TAB,   KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,                             KC_Y,               KC_U,                 KC_I,    KC_O,       KC_P,           KC_BSPC,
    KC_CAPS,  KC_A,       KC_S,       KC_D,       KC_F,       KC_G,                             KC_H,               KC_J,                 KC_K,    KC_L,       KC_SCLN,        KC_ENT,
    KC_LSFT,  KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,    KC_MUTE,   RGB_TOG,      KC_N,               KC_M,                 KC_COMMA, KC_DOT,    KC_SLSH,        KC_QUOT,
                      KC_LGUI, KC_LALT, KC_LCTL, KC_SPACE, MO(_LOWER),   TD(1), KC_PGUP, KC_DEL, KC_SPACE, MO(_RAISE)
),
[_LOWER] = LAYOUT(
    KC_TRNS,  LSFT(KC_3), KC_GRV,     RALT(KC_4), RALT(KC_6), LSFT(KC_4),                       RALT(KC_LBRC),      RALT(KC_RBRC),        RALT(KC_GRAVE), KC_EQL,    LSFT(KC_EQL),   KC_TRNS,
    LALT(KC_TAB), KC_KP_1, KC_KP_2,   KC_KP_3,    KC_PAST,    KC_PSLS,                          LSFT(KC_8),         LSFT(KC_9),           KC_UP,       LSFT(KC_2), KC_MINS,        KC_TRNS,
    KC_TRNS,  KC_KP_4,    KC_KP_5,    KC_KP_6,    KC_PPLS,    KC_PMNS,                          KC_HOME,            KC_LEFT,              KC_DOWN,     KC_RGHT,    KC_END,         KC_TRNS,
    KC_RSFT,  KC_KP_7,    KC_KP_8,    KC_KP_9,    KC_PDOT,    LSFT(KC_0), KC_TRNS,   KC_TRNS,   RALT(KC_QUOT),      RALT(KC_BSLS),        LSFT(KC_COMMA), LSFT(KC_DOT), LSFT(KC_SLSH), KC_LBRC,
                      KC_KP_0, LSFT(KC_5), LSFT(KC_LBRC), KC_ENT, KC_TRNS,   KC_TRNS, KC_PGDN, KC_INS, KC_TRNS, KC_TRNS
),
[_RAISE] = LAYOUT(
    KC_TRNS,  LSFT(KC_3), KC_GRV,     RALT(KC_4), RALT(KC_6), LSFT(KC_4),                       RALT(KC_LBRC),      RALT(KC_RBRC),        RALT(KC_GRAVE), KC_EQL,    LSFT(KC_EQL),   KC_TRNS,
    LALT(KC_TAB), KC_KP_1, KC_KP_2,   KC_KP_3,    KC_PAST,    KC_PSLS,                          LSFT(KC_8),         LSFT(KC_9),           KC_UP,       LSFT(KC_2), KC_MINS,        KC_TRNS,
    KC_TRNS,  KC_KP_4,    KC_KP_5,    KC_KP_6,    KC_PPLS,    KC_PMNS,                          KC_HOME,            KC_LEFT,              KC_DOWN,     KC_RGHT,    KC_END,         KC_TRNS,
    KC_RSFT,  KC_KP_7,    KC_KP_8,    KC_KP_9,    KC_PDOT,    LSFT(KC_0), KC_TRNS,   KC_TRNS,   RALT(KC_QUOT),      RALT(KC_BSLS),        LSFT(KC_COMMA), LSFT(KC_DOT), LSFT(KC_SLSH), KC_LBRC,
                      KC_KP_0, LSFT(KC_5), LSFT(KC_LBRC), KC_ENT, KC_TRNS,   KC_TRNS, KC_PGDN, KC_INS, KC_TRNS, KC_TRNS
),
[_ADJUST] = LAYOUT(
    KC_TRNS,  KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                          KC_TRNS,            KC_TRNS,              KC_TRNS,     KC_TRNS,    KC_TRNS,        KC_TRNS,
    KC_TRNS,  KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_TRNS,                          KC_TRNS,            KC_F4,                KC_F3,       KC_F2,      KC_F1,          KC_TRNS,
    KC_TRNS,  KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_TRNS,                          KC_TRNS,            KC_F8,                KC_F7,       KC_F6,      KC_F5,          KC_TRNS,
    KC_TRNS,  KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_TRNS, KC_BTN2,   KC_BTN1,      KC_TRNS,            KC_F12,               KC_F11,      KC_F10,     KC_F9,          KC_TRNS,
                      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_BTN1,            KC_TRNS,              KC_TRNS,     KC_TRNS,    KC_TRNS
)
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool s_pressed = false;
    static bool c_pressed = false;
    static bool combo_active = false;

    switch (keycode) {
        case KC_S:
            s_pressed = record->event.pressed;
            break;
        case KC_C:
            c_pressed = record->event.pressed;
            break;
    }

    if (s_pressed && c_pressed) {
        if (!combo_active) {
            combo_active = true;
            tap_code16(KC_BSLASH);
        }
        return false;
    } else {
        combo_active = false;
    }

    return true;
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_WH_D, KC_WH_U) },
    [_LOWER]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_WH_L, KC_WH_R) },
    [_RAISE]  = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_WH_L, KC_WH_R) },
    [_ADJUST] = { ENCODER_CCW_CW(KC_MS_L, KC_MS_R), ENCODER_CCW_CW(KC_MS_D, KC_MS_U) },
};
#endif

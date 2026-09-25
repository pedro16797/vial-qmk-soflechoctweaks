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

enum custom_keycodes {
    PWR_HOLD = QK_KB_0 // Hold 1s: sleep, or power off while on a non-base layer
};

// Vial resets its EEPROM on the first boot of every new build (random BUILD_ID),
// so defaults are re-applied whenever that happens
static void init_vial_defaults(void) {
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
#include "assets/mod_shift_off.h"
#include "assets/mod_ctrl_off.h"
#include "assets/mod_alt_off.h"
#include "assets/mod_gui_off.h"
#include "assets/lock_caps_on.h"
#include "assets/lock_caps_off.h"

#ifdef SPLIT_KEYBOARD
#    include "quantum/split_common/transactions.h"
#    include "quantum/split_common/split_util.h"
#endif

#ifdef RGB_MATRIX_ENABLE
#    include "rgb_matrix.h"
#    include "lib/lib8tion/lib8tion.h"
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
                    KC_LGUI, KC_LALT, KC_LCTL, KC_SPACE, MO(_LOWER),                         MO(_RAISE), KC_SPACE, KC_DEL, KC_PGUP, PWR_HOLD
),
[_LOWER] = LAYOUT(
    // Row 0
    KC_TRNS,  LSFT(KC_3), KC_GRV,     RALT(KC_4), RALT(KC_6), LSFT(KC_4),                       RALT(KC_LBRC), RALT(KC_RBRC), RALT(KC_GRV), KC_EQL, LSFT(KC_EQL), KC_MPRV,
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
    KC_TRNS,  LSFT(KC_3), KC_GRV,     RALT(KC_4), RALT(KC_6), LSFT(KC_4),                       RALT(KC_LBRC), RALT(KC_RBRC), RALT(KC_GRV), KC_EQL, LSFT(KC_EQL), KC_MNXT,
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
    KC_TRNS,  KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_TRNS,                          RM_NEXT, RM_VALU, RM_HUEU, RM_SPDU, RM_SATU, KC_TRNS,
    // Row 2
    KC_TRNS,  KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_TRNS,                          RM_PREV, RM_VALD, RM_HUED, RM_SPDD, RM_SATD, KC_TRNS,
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
static uint32_t thumb_led_mask = 0; // Local thumb row, bit = LED index - local_led_min

// Hold-to-sleep/power-off state, mirrored to the slave so both halves draw the progress sweep
#define POWER_HOLD_MS 1000
enum { HOLD_NONE, HOLD_SLEEP, HOLD_POWER };
typedef struct {
    uint8_t  mode;
    uint32_t start; // sync_timer time, shared by both halves
} power_hold_t;
static power_hold_t power_hold;
static bool         power_hold_fired, power_hold_dirty;

#ifdef OLED_ENABLE
// Extra glyphs in font_es.c
enum {
    ES_NTIL = 0x80, ES_NTIL_U, ES_CCED, ES_CCED_U, ES_ORD_M, ES_ORD_F, ES_MDOT,
    ES_EURO, ES_NOT, ES_IEXC, ES_IQUE, ES_ACUT, ES_DIAE, ES_ENTER
};

// What Windows set to Spanish (Spain) prints for KC_A..KC_SLSH: plain, Shift, AltGr
static const uint8_t PROGMEM es_chars[][3] = {
    {'a', 'A', 0},              // KC_A
    {'b', 'B', 0},              // KC_B
    {'c', 'C', 0},              // KC_C
    {'d', 'D', 0},              // KC_D
    {'e', 'E', ES_EURO},        // KC_E
    {'f', 'F', 0},              // KC_F
    {'g', 'G', 0},              // KC_G
    {'h', 'H', 0},              // KC_H
    {'i', 'I', 0},              // KC_I
    {'j', 'J', 0},              // KC_J
    {'k', 'K', 0},              // KC_K
    {'l', 'L', 0},              // KC_L
    {'m', 'M', 0},              // KC_M
    {'n', 'N', 0},              // KC_N
    {'o', 'O', 0},              // KC_O
    {'p', 'P', 0},              // KC_P
    {'q', 'Q', 0},              // KC_Q
    {'r', 'R', 0},              // KC_R
    {'s', 'S', 0},              // KC_S
    {'t', 'T', 0},              // KC_T
    {'u', 'U', 0},              // KC_U
    {'v', 'V', 0},              // KC_V
    {'w', 'W', 0},              // KC_W
    {'x', 'X', 0},              // KC_X
    {'y', 'Y', 0},              // KC_Y
    {'z', 'Z', 0},              // KC_Z
    {'1', '!', '|'},            // KC_1
    {'2', '"', '@'},            // KC_2
    {'3', ES_MDOT, '#'},        // KC_3
    {'4', '$', '~'},            // KC_4
    {'5', '%', ES_EURO},        // KC_5
    {'6', '&', ES_NOT},         // KC_6
    {'7', '/', 0},              // KC_7
    {'8', '(', 0},              // KC_8
    {'9', ')', 0},              // KC_9
    {'0', '=', 0},              // KC_0
    {ES_ENTER, ES_ENTER, 0},    // KC_ENT
    {0, 0, 0},                  // KC_ESC
    {0, 0, 0},                  // KC_BSPC
    {0, 0, 0},                  // KC_TAB
    {'_', '_', 0},              // KC_SPC
    {'\'', '?', 0},             // KC_MINS
    {ES_IEXC, ES_IQUE, 0},      // KC_EQL
    {'`', '^', '['},            // KC_LBRC
    {'+', '*', ']'},            // KC_RBRC
    {ES_CCED, ES_CCED_U, '}'},  // KC_BSLS
    {ES_CCED, ES_CCED_U, '}'},  // KC_NUHS
    {ES_NTIL, ES_NTIL_U, 0},    // KC_SCLN
    {ES_ACUT, ES_DIAE, '{'},    // KC_QUOT
    {ES_ORD_M, ES_ORD_F, '\\'}, // KC_GRV
    {',', ';', 0},              // KC_COMM
    {'.', ':', 0},              // KC_DOT
    {'-', '_', 0},              // KC_SLSH
};

static uint8_t es_char(uint16_t keycode) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    if (IS_QK_MODS(keycode)) {
        uint8_t m = QK_MODS_GET_MODS(keycode);
        mods |= (m & 0x10) ? (m & 0x0F) << 4 : m;
        keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
    }
    // Windows treats Ctrl+Alt as AltGr; any other Ctrl/Alt/Win combo is a shortcut, not text
    bool altgr = (mods & MOD_BIT(KC_RALT)) || ((mods & MOD_MASK_CTRL) && (mods & MOD_MASK_ALT));
    if (!altgr && (mods & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI))) return 0;
    bool shift = mods & MOD_MASK_SHIFT;

    switch (keycode) {
        case KC_A ... KC_SLSH: {
            bool cased = keycode <= KC_Z || keycode == KC_SCLN || keycode == KC_BSLS || keycode == KC_NUHS;
            if (cased && !altgr && host_keyboard_led_state().caps_lock) shift = !shift;
            return pgm_read_byte(&es_chars[keycode - KC_A][altgr ? 2 : shift]);
        }
        case KC_NUBS: return altgr ? 0 : (shift ? '>' : '<');
        case KC_KP_1 ... KC_KP_9: return '1' + (keycode - KC_KP_1);
        case KC_KP_0: return '0';
        case KC_PDOT: return '.';
        case KC_PSLS: return '/';
        case KC_PAST: return '*';
        case KC_PMNS: return '-';
        case KC_PPLS: return '+';
        case KC_PENT: return ES_ENTER;
        default:      return 0;
    }
}

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
        case _LOWER:  oled_write_raw_P(img_lyr_lower, 128); break;
        case _RAISE:  oled_write_raw_P(img_lyr_raise, 128); break;
        case _ADJUST: oled_write_raw_P(img_lyr_adjust, 128); break;
        default:      oled_write_raw_P(img_lyr_qwerty, 128); break;
    }

    // Layer 0-3, mods 4-11 (2 lines each), caps 12-15
    oled_set_cursor(0, 4);
    oled_write_raw_P((current_mods & MOD_MASK_SHIFT) ? img_mod_shift : img_mod_shift_off, 64);
    oled_set_cursor(0, 6);
    oled_write_raw_P((current_mods & MOD_MASK_CTRL) ? img_mod_ctrl : img_mod_ctrl_off, 64);
    oled_set_cursor(0, 8);
    oled_write_raw_P((current_mods & MOD_MASK_ALT) ? img_mod_alt : img_mod_alt_off, 64);
    oled_set_cursor(0, 10);
    oled_write_raw_P((current_mods & MOD_MASK_GUI) ? img_mod_gui : img_mod_gui_off, 64);
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

static void power_hold_slave_handler(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data) {
    if (in_len == sizeof(power_hold)) memcpy(&power_hold, in_data, sizeof(power_hold));
}

// Master only: fires the action once held long enough and pushes state changes to the slave
static void power_hold_task(void) {
    if (power_hold_dirty && is_transport_connected()) {
        if (transaction_rpc_send(RPC_ID_POWER_HOLD, sizeof(power_hold), &power_hold)) power_hold_dirty = false;
    }
    if (power_hold.mode == HOLD_NONE || power_hold_fired) return;
    if (sync_timer_elapsed32(power_hold.start) >= POWER_HOLD_MS) {
        power_hold_fired = true;
        register_code16(power_hold.mode == HOLD_POWER ? KC_PWR : KC_SLEP);
    }
}

void keyboard_post_init_user(void) {
    decay_timer = timer_read32();

    // Each half only scans and decays its own rows/LEDs
    const uint8_t split[] = RGB_MATRIX_SPLIT;
    bool          is_left = is_keyboard_left();
    local_row_min         = is_left ? 0 : MATRIX_ROWS / 2;
    local_led_min         = is_left ? 0 : split[0];
    local_led_max         = is_left ? split[0] : RGB_MATRIX_LED_COUNT;

    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        uint8_t led = g_led_config.matrix_co[local_row_min + 4][c];
        if (led != NO_LED) thumb_led_mask |= 1UL << (led - local_led_min);
    }

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
    } else {
        transaction_register_rpc(RPC_ID_POWER_HOLD, power_hold_slave_handler);
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

// Adjust layer light controls (top row raises, home row lowers), each lit with the result it would give
enum { PREVIEW_MODE, PREVIEW_VAL, PREVIEW_HUE, PREVIEW_SPD, PREVIEW_SAT };
static const struct {
    uint8_t row, col, kind;
    int8_t  dir;
} PROGMEM light_keys[] = {
    {6, 5, PREVIEW_MODE, 1}, {6, 4, PREVIEW_VAL, 1}, {6, 3, PREVIEW_HUE, 1}, {6, 2, PREVIEW_SPD, 1}, {6, 1, PREVIEW_SAT, 1},
    {7, 5, PREVIEW_MODE, -1}, {7, 4, PREVIEW_VAL, -1}, {7, 3, PREVIEW_HUE, -1}, {7, 2, PREVIEW_SPD, -1}, {7, 1, PREVIEW_SAT, -1},
};

// Defined in rgb_matrix_user.inc
uint8_t pastel_mode_flags(uint8_t mode);
rgb_t   pastel_led_color(uint8_t flags, uint8_t i, hsv_t hsv, uint8_t time);

static void light_key_previews(uint8_t led_min, uint8_t led_max) {
    hsv_t   cur   = rgb_matrix_get_hsv();
    uint8_t speed = rgb_matrix_get_speed();
    uint8_t mode  = rgb_matrix_get_mode();
    for (uint8_t k = 0; k < ARRAY_SIZE(light_keys); k++) {
        uint8_t led = g_led_config.matrix_co[pgm_read_byte(&light_keys[k].row)][pgm_read_byte(&light_keys[k].col)];
        if (led < led_min || led >= led_max) continue;
        uint8_t kind = pgm_read_byte(&light_keys[k].kind);
        bool    up   = (int8_t)pgm_read_byte(&light_keys[k].dir) > 0;
        hsv_t   hsv  = cur;
        uint8_t spd  = speed;
        uint8_t m    = mode;
        switch (kind) {
            case PREVIEW_MODE: // Same wrap-around as rgb_matrix_step()/step_reverse()
                m = up ? (m + 1 < RGB_MATRIX_EFFECT_MAX ? m + 1 : 1) : (m > 1 ? m - 1 : RGB_MATRIX_EFFECT_MAX - 1);
                break;
            case PREVIEW_VAL:
                hsv.v = up ? qadd8(hsv.v, RGB_MATRIX_VAL_STEP) : qsub8(hsv.v, RGB_MATRIX_VAL_STEP);
                if (hsv.v > RGB_MATRIX_MAXIMUM_BRIGHTNESS) hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
                break;
            case PREVIEW_HUE: hsv.h += up ? RGB_MATRIX_HUE_STEP : -RGB_MATRIX_HUE_STEP; break;
            case PREVIEW_SPD: spd = up ? qadd8(spd, RGB_MATRIX_SPD_STEP) : qsub8(spd, RGB_MATRIX_SPD_STEP); break;
            case PREVIEW_SAT: hsv.s = up ? qadd8(hsv.s, RGB_MATRIX_SAT_STEP) : qsub8(hsv.s, RGB_MATRIX_SAT_STEP); break;
        }
        // Same per-LED maths as the effects themselves (Adjust never reverses the drift)
        rgb_t rgb = pastel_led_color(pastel_mode_flags(m), led, hsv, (g_rgb_timer * spd) >> 10);
        rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // No master yet (PC off), keep the LEDs dark while the split watchdog retries
    if (!is_keyboard_master() && !split_watchdog_check()) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
        return false;
    }

    // Link lost: pulse the master's thumb row red (only after the halves have talked once)
    static bool link_seen = false;
    if (is_keyboard_master()) {
        if (split_watchdog_check()) link_seen = true;
        if (link_seen && !is_transport_connected()) {
            uint16_t t     = g_rgb_timer & 1023;
            uint8_t  level = ((t < 512 ? t : 1023 - t) * RGB_MATRIX_MAXIMUM_BRIGHTNESS) >> 9;
            for (uint8_t i = led_min; i < led_max; i++) {
                if (thumb_led_mask & (1UL << (i - local_led_min))) rgb_matrix_set_color(i, level, 0, 0);
            }
        }
    }

    if (get_highest_layer(layer_state) == _ADJUST) light_key_previews(led_min, led_max);

    // Power key progress: columns go dark (sleep) or red (power off) from right to left
    if (power_hold.mode != HOLD_NONE) {
        uint32_t elapsed = sync_timer_elapsed32(power_hold.start);
        if (elapsed > POWER_HOLD_MS) elapsed = POWER_HOLD_MS;
        uint8_t threshold = 225 - (elapsed * 225) / POWER_HOLD_MS; // LED x spans 0-224
        uint8_t red       = power_hold.mode == HOLD_POWER ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : 0;
        for (uint8_t i = led_min; i < led_max; i++) {
            if (g_led_config.point[i].x >= threshold) rgb_matrix_set_color(i, red, 0, 0);
        }
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == PWR_HOLD) {
        if (record->event.pressed) {
            power_hold.mode  = get_highest_layer(layer_state) != _QWERTY ? HOLD_POWER : HOLD_SLEEP;
            power_hold.start = sync_timer_read32();
        } else {
            if (power_hold_fired) unregister_code16(power_hold.mode == HOLD_POWER ? KC_PWR : KC_SLEP);
            power_hold.mode = HOLD_NONE;
        }
        power_hold_fired = false;
        power_hold_dirty = true;
        return false;
    }

#ifdef OLED_ENABLE
    // Show what the host will print; combos arrive with their output keycode
    if (is_keyboard_master() && record->event.pressed) {
        uint8_t c = es_char(keycode);
        if (c) add_to_buffer_at((char)c, (uint8_t)(timer_read32() % 5));
    }
#endif
    return true;
}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) power_hold_task();

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

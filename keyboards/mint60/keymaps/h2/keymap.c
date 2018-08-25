/* Copyright 2018 Eucalyn
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
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

enum custom_keycodes {
  RGBRST
};


// B C D F G M N P Q R S T W Y Z
/*
 * __ __ U __ __      __ __ __ __ __
 *  A  E  I O  __      __ __ K  __ __
 *   __ X  C V  __      __ H  J  L  __
 *
 *
 * // F,B足し。あまり変わらないが、Fの方がBより出現率が高い。が、Fの位置が良すぎる気も…
 *
 * __ __ U __ __      __ __ __ __ __
 *  A  E  I O  __      B  F  K  __ __
 *   __ X  C V  __      __ H  J  L  __
 *
 * // Q,Wは並んでいたほうが都合が良い。Ctrl+W,Qを考えると
 *
 * Q  W  U  __ __      __ __ __ __ __
 *  A  E  I  O  __      B  F  K  __ __
 *   __ X  C  V  __      __ H  J  L  __
 *
 * // R,T,D
 *
 * Q  W  U  __ __      __ __ D  __ __
 *  A  E  I  O  __      B  F  K  R  T
 *   __ X  C  V  __      __ H  J  L  __
 *
 * // G M N P S Y Z
 *
 * Q  W  U  S  __      __ M  D  N  __
 *  A  E  I  O  Z       B  F  K  R  T
 *   __ X  C  V  __      Y  H  J  L  __
 *
 * // G P / \ , .
 *
 * Q  W  U  S  G       /  M  D  N  P
 *  A  E  I  O  Z       B  F  K  R  T
 *   \  X  C  V  ,       Y  H  J  L  .
 *
 * // HJKLをQWERTYに
 * // B F R
 *
 * Q  W  U  S  G       /  M  D  N  P
 *  A  E  I  O  Z       H  J  K  L  T
 *   \  X  C  V  ,       Y  B  F  R  .
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( \
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,    KC_BSPC,  \
    KC_TAB,    KC_Q,    KC_W,    KC_U,    KC_S,    KC_G,   KC_SLASH,    KC_M,    KC_D,    KC_N,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,  \
    KC_LCTL,     KC_A,    KC_E,    KC_I,    KC_O,    KC_Z,      KC_H,    KC_J,    KC_K,    KC_L,    KC_T,    KC_QUOT,        KC_ENT,   \
    KC_LSFT,       KC_BSLASH,    KC_X,    KC_C,    KC_V,    KC_COMM,      KC_Y,    KC_B,    KC_F,    KC_R,    KC_DOT, KC_SCOLON,   KC_UP,  MO(1),   \
    KC_ESC,    KC_BSPACE,    KC_LGUI,    KC_LALT,    KC_SPC,        KC_RSFT, KC_ENT,  LALT(KC_GRV),               KC_LEFT,KC_DOWN,KC_RGHT \
  ),
  [1] = LAYOUT( \
    KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,    KC_DEL, \
    RGB_TOG,   RGBRST,  RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
    XXXXXXX,     RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, \
    _______,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, KC_PGUP, _______, \
    XXXXXXX,   _______,    _______,    _______,    XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX,                   KC_HOME, KC_PGDN, KC_END \
  )
};

const uint16_t PROGMEM fn_actions[] = {

};

// define variables for reactive RGB
bool TOG_STATUS = false;
int RGB_current_mode;


const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
          if (record->event.pressed) {
            register_code(KC_RSFT);
          } else {
            unregister_code(KC_RSFT);
          }
        break;
      }
    return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
  }
  return true;
}

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {

}

#include QMK_KEYBOARD_H
#include "process_keycode/process_magic.h"
#include "rgb_matrix_map.h"

enum custom_keycodes {
  KC_00 = SAFE_RANGE,
  KC_WINLK,    //Toggles Win key on and off
};

bool muted_dc = false; // trạng thái mute Discord

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // xử lý custom keycodes & một số hành vi khác
    if (record->event.pressed) {
        if (keycode == KC_00) {
            SEND_STRING("00");
            return false; // đã xử lý
        }
        if (keycode == KC_WINLK) {
            // Nếu GUI đang bật (no_gui == false) -> tắt GUI
            if (!keymap_config.no_gui) {
                process_magic(GU_OFF, record);
            } else {
                process_magic(GU_ON, record);
            }
            return false; // đã xử lý qua process_magic
        }
        // Toggle trạng thái muted_dc khi bấm Right Control
        if (keycode == KC_RCTL) {
            muted_dc = !muted_dc;
            // Không block keycode mặc định — trả về true để RCtrl vẫn gửi cho host.
        }
    }

    return true;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,          KC_MPLY,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_INS,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        _______, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,          KC_MUTE,
        _______, RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,            _______,
        _______, _______, RGB_VAD, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          _______,
        _______,          _______, RGB_HUI, _______, _______, _______, NK_TOGG, _______, _______, _______, _______,          _______, RGB_MOD, _______,
        _______, KC_WINLK, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),


};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
};
#endif

#ifdef RGB_MATRIX_ENABLE
    // Capslock indicator on Left side lights.
    bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
		rgb_matrix_set_color_all(RGB_NAUTILUS);
        if (host_keyboard_led_state().caps_lock) {
			if (!caps_active) {
				caps_active = true;
				caps_flash_on = true;
				caps_flasher = timer_read();
			}
			if (timer_elapsed(caps_flasher) > 500) {
				caps_flasher = timer_read();
				caps_flash_on = !caps_flash_on;
			}
            rgb_matrix_set_color(LED_CAPS, RGB_WHITE);
            if (caps_flash_on) {
				for (uint8_t i=0; i<sizeof(LED_SIDE_LEFT)/sizeof(LED_SIDE_LEFT[0]); i++) {
					rgb_matrix_set_color(LED_SIDE_LEFT[i], RGB_RED);
					rgb_matrix_set_color(LED_SIDE_RIGHT[i], RGB_RED);
				}
			} else {
				for (uint8_t i=0; i<sizeof(LED_SIDE_LEFT)/sizeof(LED_SIDE_LEFT[0]); i++) {
					rgb_matrix_set_color(LED_SIDE_LEFT[i], 0, 0, 0);
					rgb_matrix_set_color(LED_SIDE_RIGHT[i], 0, 0, 0);
				}
			}
        } else {
			caps_active = false;
		}
			if (muted_dc) {
				rgb_matrix_set_color(LED_RCTL, RGB_RED);
			}

			if (keymap_config.no_gui) {
				rgb_matrix_set_color(LED_LWIN, RGB_RED);
			}
		return true;		
    }
    void suspend_power_down_user(void) {
        rgb_matrix_set_suspend_state(true);
    }

    void suspend_wakeup_init_user(void) {
        rgb_matrix_set_suspend_state(false);
    }
#endif

void keyboard_post_init_keymap(void) {
    // keyboard_post_init_user() moved to userspace
    #ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    #endif
}
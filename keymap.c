#include QMK_KEYBOARD_H
#include "rgb_matrix_map.h"

enum custom_keycodes {
  KC_00 = SAFE_RANGE,
  KC_WINLK,    //Toggles Win key on and off
};
bool muted_dc = false; // ADD this near the begining of keymap.c
bool procces_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		if (!muted_dc) {
		muted_dc = true;
		register_code16(KC_RCTL);
		unregister_code(KC_RCTL);

	} else {
		muted_dc = false;
		register_code16(KC_RCTL);
		unregister_code(KC_RCTL);

		}
	}
	return true;
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_00:
        if (record->event.pressed) {
            // when keycode KC_00 is pressed
            SEND_STRING("00");
        } else {
            // when keycode KC_00 is released
        }
        break;
    case KC_WINLK:
        if (record->event.pressed) {
            if(!keymap_config.no_gui) {
                process_magic(GUI_OFF, record);
            } else {
                process_magic(GUI_ON, record);
            }
        } else  unregister_code16(keycode);
        break;
    }
    return true;
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the QK_BOOT key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,          KC_MPLY,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_PGUP,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGDN,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_END,
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
				rgb_matrix_set_color(LED_RCTL, RGB_RED); //light up RCtrl key when turn on
			}
			if (keymap_config.no_gui) {
				rgb_matrix_set_color(LED_LWIN, RGB_RED);  //light up Win key when disabled
			}
		return false;		
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
        rgb_matrix_set_color_all(RGB_NAUTILUS); // Default startup colour
    #endif
}
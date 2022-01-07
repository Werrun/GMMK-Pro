#pragma once

#ifdef RGB_MATRIX_ENABLE
    #define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_COLOR
	#define RGB_DISABLE_WHEN_USB_SUSPENDED true
#endif

#define FORCE_NKRO
#define USB_POLLING_INTERVAL_MS 1
#define DEBOUNCE 5
#define RGB_DISABLE_TIMEOUT 900000     // 15 minutes (5 * 60 * 1000ms)

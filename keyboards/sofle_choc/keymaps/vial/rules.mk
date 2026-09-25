OLED_ENABLE = yes
ENCODER_ENABLE = yes
CONSOLE_ENABLE = no
EXTRAKEY_ENABLE = yes
VIA_ENABLE = yes
VIAL_ENABLE = yes
ENCODER_MAP_ENABLE = yes
LTO_ENABLE = yes
BACKLIGHT_ENABLE = no
RGBLIGHT_ENABLE = no
VIALRGB_ENABLE = yes
QMK_SETTINGS = no
MOUSEKEY_ENABLE = yes
COMBO_ENABLE = yes
KEY_OVERRIDE_ENABLE = no
TAP_DANCE_ENABLE = no
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_CUSTOM_USER = yes
SPLIT_KEYBOARD = yes

# Handedness stored in EEPROM so USB can go into either half; CI builds each side with SIDE=left/right
ifeq ($(strip $(SIDE)), left)
    OPT_DEFS += -DEE_HANDS -DINIT_EE_HANDS_LEFT
else ifeq ($(strip $(SIDE)), right)
    OPT_DEFS += -DEE_HANDS -DINIT_EE_HANDS_RIGHT
endif

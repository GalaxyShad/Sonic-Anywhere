#ifndef SONIC_ANYWHERE_INPUT_H
#define SONIC_ANYWHERE_INPUT_H

typedef enum ButtonCode {
    BUTTON_CODE__START
} ButtonCode;

int input__is_btn_pressed(ButtonCode btn_code);

#endif // SONIC_ANYWHERE_INPUT_H

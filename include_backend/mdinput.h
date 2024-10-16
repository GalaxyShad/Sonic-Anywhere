#ifndef SONIC_ANYWHERE_MDINPUT_H
#define SONIC_ANYWHERE_MDINPUT_H

typedef enum MDButtonCode { MD_BUTTON_CODE__START } MDButtonCode;

int md_input__is_btn_pressed(MDButtonCode btn_code);

#endif // SONIC_ANYWHERE_MDINPUT_H

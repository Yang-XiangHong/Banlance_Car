#ifndef _CONTROL_H
#define _CONTROL_H

#include "stm32f10x.h"

void JoyStick_Order(void);

extern uint16_t L_X, L_Y , R_X, R_Y;
extern int flag;

#endif

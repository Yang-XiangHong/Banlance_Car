#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"
#include "led.h"
#include "encoder.h"
#include "inv_mpu.h"
#include "mpu6050.h"
#include "motor.h"

#include "control.h"

void TIM5_Int_Init(u16 arr,u16 psc);

#endif

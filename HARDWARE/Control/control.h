#ifndef _CONTROL_H
#define _CONTROL_H

#include "sys.h"
#include "motor.h"

int	Vertical_Ring_PD(float Angle,float Gyro);
int Vertical_speed_PI(int encoder_left,int encoder_right,float Movement );
int Vertical_turn_PD(u8 CCD,short yaw);

#endif

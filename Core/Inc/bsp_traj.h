#ifndef __BSP_TRAJ_H
#define __BSP_TRAJ_H
#include "main.h"
void Traj_Start_Move(float t1,float t2,float t3,float t4);
void Traj_Update(void);
void Set_Target_Angle(uint8_t axis_id,float angle);
void Set_All_Targets(float t1,float t2,float t3,float t4);
void Traj_Update(void);
void Traj_Init(void);
#endif
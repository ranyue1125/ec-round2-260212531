#include "bsp_traj.h"
#include "bsp_pwm.h"
#include <math.h>
#define LIMIT_MIN 10.0f
#define LIMIT_MAX 170.0f
uint8_t limit_flag=0;
#define MAX_SPEED 90.0f
#define ACCEL 180.0f
#define DT 0.001f
float target_angle[4]={90.0f,90.0f,90.0f,90.0f};
float current_angle[4]={90.0f,90.0f,90.0f,90.0f};
static float speed[4]={0};
static float axis_accel[4]={0};
static float axis_max_speed[4]={0};
static int state[4]={3};

void Traj_Init(void){
    for (int i=0; i<4; i++) {
        speed[i]=0;
        axis_accel[i]=ACCEL;
        axis_max_speed[i]=MAX_SPEED;
        state[i]=3;
        Set_Servo_Angle(i+1, current_angle[i]);
    }
}
void Set_Target_Angle(uint8_t axis_id,float angle){
    if(axis_id >=1 && axis_id <= 4){
        Set_All_Targets(
            (axis_id==1)?angle:target_angle[0],
            (axis_id==2)?angle:target_angle[1],
            (axis_id==3)?angle:target_angle[2],
            (axis_id==4)?angle:target_angle[3]
        );
    }
}
void Set_All_Targets(float t1,float t2,float t3,float t4){
    float targets[4]={t1,t2,t3,t4};
    float max_time=0;
    for(int i=0;i<4;i++){
        if (targets[i]<LIMIT_MIN) {
            targets[i]=LIMIT_MIN;
            limit_flag=1;
        }else if (targets[i]>LIMIT_MAX) {
            targets[i]=LIMIT_MAX;
            limit_flag=1;
        }
        target_angle[i]=targets[i];
        float dist=fabsf(target_angle[i]-current_angle[i]);
        float time_needed=dist/MAX_SPEED;
        if(time_needed>max_time)max_time=time_needed;
    }
for(int i=0;i<4;i++){
    float dist=fabsf(target_angle[i]-current_angle[i]);
    if(dist>0.1f&&max_time>0.01f){
        axis_max_speed[i]=dist/max_time;
        axis_accel[i]=axis_max_speed[i]*(ACCEL/MAX_SPEED);
    }else{
        axis_max_speed[i]=MAX_SPEED;
        axis_accel[i]=ACCEL;
    }
    speed[i]=0;
    state[i]=0;
}
}
void Traj_Update(void){
    for (int i=0; i<4; i++) {
    if (state[i]==3) continue;
    float delta=target_angle[i]-current_angle[i];
    float direction=(delta>0)?1.0f:-1.0f;
    float abs_delta=fabsf(delta);
    float decel_distance=(speed[i]*speed[i])/(2.0f*axis_accel[i]);
    if (abs_delta<0.2f) {
    current_angle[i]=target_angle[i];
    speed[i]=0;
    state[i]=3;
    }else if (abs_delta<=decel_distance) {
        state[i]=2;
        speed[i]-=axis_accel[i]*DT;
        if(speed[i]<0)speed[i]=0;
    }else if (speed[i]<axis_max_speed[i]) {
        state[i]=0;
        speed[i]+=axis_accel[i]*DT;
        if(speed[i]>axis_max_speed[i]) speed[i]=axis_max_speed[i];
    }else {
    state[i]=1;
    }
    if (state[i]!=3) {
        current_angle[i]+=speed[i]*direction*DT;
        Set_Servo_Angle(i+1, current_angle[i]);
    }
    }
}
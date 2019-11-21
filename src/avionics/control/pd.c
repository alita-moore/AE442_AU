//
// Created by Dragi on 11/20/2019.
//
float Kp;
float Kd;

typedef struct control{
    float s_ang[2];
    float torque[2];
    float omega[2];
    float theta[2];
}control;

void pd(control state){
    state.torque[0] = -Kp*state.theta[0] - Kd*state.omega[0];
    state.torque[1] = -Kp*state.theta[1] - Kd*state.omega[1];

    
}
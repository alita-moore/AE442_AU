//
// Created by Dragi on 11/20/2019.
//
float Kp;
float Kd;
float lim_s1[2];
float lim_s2[2];
float s_ang[2];
float torque[2];
float omega[2];
float theta[2];
float temp;
bool ignition;
float thrust;
float alt;

void setup_globals(){
    Kp = 10;
    Kd = 10;
    
    lim_s1[0] = 2;
    lim_s1[1] = 150;
    
    lim_s2[0] = 2;
    lim_s2[1] = 150;
    
    s_ang[0] = 0;
    s_ang[1] = 0;

    torque[0] = 0;
    torque[1] = 0;

    omega[0] = 0;
    omega[1] = 0;

    theta[0] = 0;
    theta[1] = 0;

    ignition = false;
}
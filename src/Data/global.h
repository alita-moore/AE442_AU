//
// Created by Dragi on 11/20/2019.
//
float lim_s1[2];
float lim_s2[2];

void setup_const(){
    lim_s1[0] = 2;
    lim_s1[1] = 150;
    lim_s2[0] = 2;
    lim_s2[1] = 150;
}

typedef struct mode{
    int control;
    int save;
    int ignite;
    int servo;
    int mode;
}mode;
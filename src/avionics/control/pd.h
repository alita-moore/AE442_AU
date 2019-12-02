extern float Kp;
extern float Kd;
extern debug bug;
extern float temp;
extern float thrust;
extern float lim_s1[2];
extern float lim_s2[2];

float pd(float thet, float omeg){
    if(bug.loop){
        Serial.println("control begins");
    }
    
    temp = -Kp*thet-Kd*omeg;
    if(bug.loop){
        Serial.println("temp set");
    }

    if(bug.control){
        Serial.print("omega: ");
        Serial.print(omeg);
        Serial.print("\t theta: ");
        Serial.print(thet);
        Serial.print("\t torque: ");
        Serial.println(temp);
    }

    return temp;
}

void get_ang(float torque[2], float s_ang[2]){
    s_ang[0] = (float) 1;
    s_ang[1] = (float) 1;

    if(bug.ang){
        Serial.println("angles found");
    }

    if(s_ang[0] > lim_s1[1]){
        s_ang[0] = lim_s1[1];
    } else if(s_ang[0] < lim_s1[0]){
        s_ang[0] = lim_s1[0];
    }

    if(s_ang[1] > lim_s2[1]){
        s_ang[1] = lim_s2[1];
    } else if(s_ang[1] < lim_s2[0]){
        s_ang[1] = lim_s2[0];
    }
}
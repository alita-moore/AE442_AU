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
    float thrust = 22.2f;
    float offset[2] = {90, 90};
    
    // apply the necessary equations to find beta and alpha
    float beta = torque[0]/(thrust*0.1329);
    if(!abs(beta)<1){
        beta = beta/(abs(beta)+0.001);
    }
    beta = asin(beta);

    float alpha = torque[1]/(thrust*0.1329*cos(beta));
    if(!abs(alpha)<1){
        alpha = alpha/(abs(alpha)+0.001);
    }
    alpha = asin(alpha);

    // convert radians to angles (for simple servo action)
    beta = beta*180/M_PI;
    alpha = alpha*180/M_PI;

    if(bug.ang){
        Serial.print("Desired gimbal angle, [beta, alpha]: [ ");
        Serial.print(beta);
        Serial.print(", ");
        Serial.print(alpha);
        Serial.println("]");
    }

    // Convert the gimbal angle to servo range -> works under the assumption of p/m 10 degree gimbal action
    beta = beta/10.0f*90.0f;
    alpha = alpha/10.0f*90.0f; 

    // offset the desired gimbal (i.e. convert gimbal to servo)
    s_ang[0] = beta + offset[0];
    s_ang[1] = alpha + offset[1];

    if(bug.ang){
        Serial.print("Calculated Servo Angle, [beta, alpha]: [ ");
        Serial.print(s_ang[0]);
        Serial.print(", ");
        Serial.print(s_ang[1]);
        Serial.println("]");
    }

    // Check servo limits, max or min as provided
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

    if(bug.ang){
        Serial.print("Actual Servo Angle, [beta, alpha]: [ ");
        Serial.print(s_ang[0]);
        Serial.print(", ");
        Serial.print(s_ang[1]);
        Serial.println("]");
    }
}
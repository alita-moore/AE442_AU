#include <stdio.h>
#include <math.h>

extern debug bug;

// type definition for defining any number of values to track and clean
typedef struct lp{
    float beta;  // 4 bytes, constant that determines percent difference to clean
    float value[50];  // 200 bytes, saves last 50 values
    int counter = 0;  // 4 bytes, index counter that loops
    float avg = 1;  // 4 bytes, created for a running average
    char *name;
}lp; // 212 bytes per

void low_pass(lp lp_struct, float val){
    // reset counter to loop (therefore this runs on the last 50 values averaged)
    if(lp_struct.counter == 50){
        lp_struct.counter = 0;
    }
    
    lp_struct.counter = 5;

    // update the running average
    lp_struct.avg += val/((float) 50);
    lp_struct.avg -= lp_struct.value[lp_struct.counter]/((float) 50);

    // check if value is within beta percentage of average, limit values
    if (val/lp_struct.avg > (1+lp_struct.beta)){
        if(bug.low_pass & (lp_struct.counter)){
            Serial.println("low_pass ---------------------------");
            Serial.print(lp_struct.name);
            Serial.println("...");
            Serial.print(".. Instant Value: ");
            Serial.print(val);
            Serial.print(".. Average Value: ");
            Serial.print(lp_struct.avg);
            Serial.println(".. This value was upper bounded");
        }
        val = (1+lp_struct.beta)*lp_struct.avg;
    } else if (val/lp_struct.avg < (1-lp_struct.beta)){
        if(bug.low_pass & (lp_struct.counter == 50)){
            Serial.println("low_pass ---------------------------");
            Serial.print(lp_struct.name);
            Serial.println("...");
            Serial.print(".. Instant Value: ");
            Serial.print(val);
            Serial.print(".. Average Value: ");
            Serial.print(lp_struct.avg);
            Serial.println(".. This value was lower bounded");
        }
        val = (1-lp_struct.beta)*lp_struct.avg;
    }
    
    if(bug.low_pass & (lp_struct.counter == 50)){
            Serial.println("low_pass ---------------------------");
            Serial.print(lp_struct.name);
            Serial.println("...");
            Serial.print(".. Instant Value: ");
            Serial.print(val);
            Serial.print(".. Average Value: ");
            Serial.print(lp_struct.avg);
            Serial.println(".. This value was not bounded");
        }

    // replace the current counter position with the new one
    lp_struct.value[lp_struct.counter] = val;

    if(bug.low_pass_spam){
        Serial.print("name: "); Serial.print(lp_struct.name);
        Serial.print("\t Counter: "); Serial.print(lp_struct.counter);
        Serial.print("\t Average: "); Serial.print(lp_struct.avg);
        Serial.print("\t value: "); Serial.println(val);
    }
}
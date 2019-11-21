#include <stdio.h>
#include <math.h>

typedef struct lp{
    float beta;  // 4 bytes
    float value[50];  // 200 bytes
    int counter;  // 4 bytes
    float sum;  // 4 bytes
}lp; // 212 bytes per

void low_pass(lp lp_struct, float t){
    if (lp_struct.counter == 50){
        lp_struct.counter = 0;
    }

    // update the running average
    lp_struct.sum += t/((float) 50);
    lp_struct.sum -= lp_struct.value[lp_struct.counter]/((float) 50);

    // replace the current counter position with the new one
    lp_struct.value[lp_struct.counter] = t;

    // check if value is within beta percentage of average, limit values
    if (t/lp_struct.sum > (1+lp_struct.beta)){
        t = (1+lp_struct.beta)*lp_struct.sum;
    } else if (t/lp_struct.sum < (1-lp_struct.beta)){
        t = (1-lp_struct.beta)*lp_struct.sum;
    }
}
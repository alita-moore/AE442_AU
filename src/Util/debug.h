#include <stdlib.h>

typedef struct debug{
    bool imu_AHRS = true;  // advanced debug info?
    bool imu = false;  // report outs to serial?
    bool low_pass = false; // says if a value was changed, what the average is, and how far off it was
    bool low_pass_spam = false; // will output every current value at every cycle
    bool loop = false; // outputs loop progress
    bool control = false; // outputs control outputs and inputs
    bool ang = false; // outputs ang inputs and outputs
    bool alt = false; // outputs the current relative altitude
}debug;
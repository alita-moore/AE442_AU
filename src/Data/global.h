// Controls
float Kp; // proportional controller term (theta)
float Kd; // derivative term (omega)

// General variables
float lim_s1[2]; // limits for servo 1
float lim_s2[2]; // limits for servo 2
float s_ang[2]; // the desired servo angle (0 -> 1, 1 -> 2) where 1 is x and 2 is y
float torque[2]; // the desired torques about the x and y axis
float omega[2]; // the omega in x and y (as calculated by the imu)
float theta[2]; // the x and y theta (as calculated from imu)
float temp; // a variable used for temporary necessities
float thrust; // the current thrust (we assume constant)
float alt; // the current altitude from the altimeter
int addr; // i don't know what this is
int loops; // number of loops performed
float pitch_zero; // pitch zeroing value
float roll_zero; // roll zeroing value
float gx_zero; // gyro x zeroing value
float gy_zero; // this is gyro y zero terms
float ignite_timer; // this will store the initial variable value for timing
float ignite_time; // this variable represents the number of milliseconds waited after arming

// boolean modes and other states
bool save; // saves to SSD every 15 loops
bool control; // performs control via PD controller
bool out_ignite; // allowance for ignition to even be checked -> quadruple redudance
bool out_servo; // allow outputs to access servos?
bool zero; // initialize?
    // ignition
    bool ignition; // final ignition command
    bool release; // whether or not the drone has been released, used to start the timer

// 'trash'
bool SD_open_cond; // was first used to minimize the number of times the SD is opened, did not warrant implementation

void setup_globals(){
    // Controls
    Kp = 5*(0.003);
    Kd = 0.003;
    
    // servo limits
    lim_s1[0] = 10;
    lim_s1[1] = 170;
    lim_s2[0] = 10;
    lim_s2[1] = 170;
    
    // zero all the global values
    s_ang[0] = 0;
    s_ang[1] = 0;
    torque[0] = 0;
    torque[1] = 0;
    omega[0] = 0;
    omega[1] = 0;
    theta[0] = 0;
    theta[1] = 0;
    alt = 0;
    addr = 0;
    loops = 0;
    pitch_zero = 0.0f;
    roll_zero = 0.0f;
    gx_zero = 0.0f;
    gy_zero = 0.0f;
    ignite_timer = 0.0f;
    ignite_time = 0.0f;

    // initialize the booleans
    zero = false;
    save = false;
    control = false;
    out_ignite = false;
    out_servo = false;
        // ignition
        ignition = false;
        release = false;

    // unused variables (trash but not deleted because unsure if actually trash)
    SD_open_cond = true;
}
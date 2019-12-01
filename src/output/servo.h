extern PWMServo S_1;
extern PWMServo S_2;

void setup_servos(int s1, int s2) {
  S_1.attach(s1);  // attaches the servo 1 on pin s1
  S_2.attach(s2);  // attaches the servo 2 on pin s2
}

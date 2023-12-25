#include <main.h>

float prev_fl_out = 0;
float prev_fr_out = 0;
float prev_bl_out = 0;
float prev_br_out = 0;

void Robot::setUpMotors() {
    pinModeFast(M1_INA, OUTPUT);
    pinModeFast(M2_INA, OUTPUT);
    pinModeFast(M3_INA, OUTPUT);
    pinModeFast(M4_INA, OUTPUT);

    pinMode(M1_PWM, OUTPUT);
    pinMode(M2_PWM, OUTPUT);
    pinMode(M3_PWM, OUTPUT);
    pinMode(M4_PWM, OUTPUT);

    pinMode(M1_CS, INPUT);
    pinMode(M2_CS, INPUT);
    pinMode(M3_CS, INPUT);
    pinMode(M4_CS, INPUT);
}

void Robot::setUpDribbler() {
    pinMode(DRIBBLER_PWM, OUTPUT);

    analogWriteFrequency(DRIBBLER_PWM, 4000); 
  
    analogWrite(DRIBBLER_PWM, 128);
    delay(3000);
}

void Robot::motorOut(int motor, float speed) {
    int INA = 0, dir = 0, pwm = 0;

    switch (motor) {
        case 1:
            INA = M1_INA;
            dir = speed > 0 ?  HIGH : LOW;
            pwm = M1_PWM;
        case 2:
            INA = M2_INA;
            dir = speed > 0 ?  LOW : HIGH;
            pwm = M2_PWM;
        case 3:
            INA = M3_INA;
            dir = speed > 0 ?  HIGH : LOW;
            pwm = M3_PWM;
        case 4:
            INA = M4_INA;
            dir = speed > 0 ?  LOW : HIGH;
            pwm = M4_PWM;
    }
    
    // stop motor from stalling out if speed is below minimum threshold
    analogWrite(pwm, abs(speed) > min_speed ? abs(speed) : 0);
    digitalWriteFast(INA, dir);
}

void Robot::move(float speed, float angle, float angVel) {
    float x_speed = sinf(degToRad(angle))*sinf(wheel_angle);
    float y_speed = cosf(degToRad(angle))*cosf(wheel_angle);

    float fl = (x_speed + y_speed) * speed;
    float fr = (-x_speed + y_speed) * speed;
    float bl = (-x_speed + y_speed) * speed;
    float br = (x_speed + y_speed) * speed;

    // calculate new speeds
    float new_fl_out = max_speed * fl * 255;
    float new_fr_out = max_speed * fr * 255;
    float new_bl_out = max_speed * bl * 255;
    float new_br_out = max_speed * br * 255;

    // check if the acceleration exceeds the maximum threshold
    float fl_out = abs(new_fl_out) - abs(prev_fl_out) < max_accel ? new_fl_out : prev_fl_out + max_accel;
    float fr_out = abs(new_fr_out) - abs(prev_fr_out) < max_accel ? new_fr_out : prev_fr_out + max_accel;
    float bl_out = abs(new_bl_out) - abs(prev_bl_out) < max_accel ? new_bl_out : prev_bl_out + max_accel;
    float br_out = abs(new_br_out) - abs(prev_br_out) < max_accel ? new_br_out : prev_br_out + max_accel;

    // update previous speeds
    prev_fl_out = fl_out;
    prev_fr_out = fr_out;
    prev_bl_out = bl_out;
    prev_br_out = br_out;

    #ifdef DEBUG
    Serial.print("1: ");
    Serial.print(fl_out);
    Serial.print("  2: ");
    Serial.print(fr_out);
    Serial.print("  3: ");
    Serial.print(bl_out);
    Serial.print("  4: ");
    Serial.println(br_out);
    #endif

    motorOut(1, round(fl_out));
    motorOut(2, round(fr_out));
    motorOut(3, round(bl_out));
    motorOut(4, round(br_out));
}

void Robot::moveToTargetPose() {
    float x_diff = target_pose.x - current_pose.x;
    float y_diff = target_pose.y - current_pose.y;
    float angle = atan2(y_diff, x_diff);
    float distance = sqrt(x_diff*x_diff + y_diff*y_diff);

    move(distance, angle, 0);
}
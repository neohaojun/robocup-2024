#include "main.h"
#include "HardwareSerial.h"

PacketSerial Layer1Serial;
PacketSerial ImuSerial;
PacketSerial TeensySerial;

Layer1RxDataUnion layer_1_rx_data;
ImuRxDataUnion imu_rx_data;
Teensy1TxDataUnion teensy_1_tx_data;

Robot robot;
Ball ball;
Goal yellow_goal;
Goal blue_goal;

unsigned long last_time = 0;
int real = 0;
int points = 0;

void setup()
{
  robot.base.setUp();
  robot.setUpSerial();
  pinModeFast(DRIBBLER_PWM, OUTPUT);
  pinModeFast(LIDAR_PWM, OUTPUT);

  robot.previous_pose.x = 0;
  robot.previous_pose.y = 0;
  robot.previous_pose.bearing = 0;

  last_time = millis();
  robot.dribbler.dribbling = true;
}

int ball_catchment = 0;

void loop()
{
#ifdef DEBUG
#else
  robot.updateSerial();
  robot.sendSerial();

  if (ball.in_catchment)
  {
    // Serial.println("Ball in catchment");
    ++ball_catchment;
  }
  else
  {
    ball_catchment = 0;
  }

  if (ball_catchment > 100)
  {
    robot.task = 1;
  }
  else if (ball.detected)
  {
    if (ball.current_pose.y < 1500)
    {
      robot.task = 2;
    }
    else
    {
      robot.task = 0;
    }
  }
  else
  {
    // robot.target_pose.x = 910;
    // robot.target_pose.y = 1215;
    // robot.target_pose.bearing = 0;
    // robot.moveToTargetPose();
    robot.task = 2;
  }

  switch (robot.task)
  {
  case 0:
    robot.orbitToBall();
    break;
  case 1:
    robot.orbitScore();
    break;
  case 2:
    robot.defendGoal();
    break;
  case 3:
    robot.moveToTargetPose();
    break;
  }

  robot.base.move(robot.move_data.speed, robot.move_data.target_angle, robot.move_data.target_bearing);

#endif
}

#include "main.h"
#include "Include/Shared/FlywheelTask.hpp"


double wantedFlywheelRPM = 0;

void setFlywheelRPM(int wantedRPM)
{
  wantedFlywheelRPM = wantedRPM;
}

void flywheelTask(void*)
{

  pidTune_t flywheelPIDParams = {0.01, 0.0, 0.0, 0.1, 0.9, 1};

  int flywheelRPM = 0;

  const double slewRate = 0.7;
  double lastPower = 0;
  double motorPower = 0;

  //Screen Tuner
  PIDScreenTuner tuneFlywheel(LV_HOR_RES, LV_VER_RES, LV_VER_RES/3);
  tuneFlywheel.initButton(0, &flywheelPIDParams.kP, "kP", 5);
  tuneFlywheel.initButton(65, &flywheelPIDParams.kD, "kD", 5);
  tuneFlywheel.initButton(130, &flywheelPIDParams.kF, "kF", 5);
  tuneFlywheel.initButton(195, &flywheelPIDParams.derivativeEma, "DE", 5);
  tuneFlywheel.initButton(260, &flywheelPIDParams.readingEma, "RE", 5);
  tuneFlywheel.initButton(325, &wantedFlywheelRPM, "RPM", 4, buttonIncrement, 500);
  tuneFlywheel.initButton(400, &tuneFlywheel.m_buttonMultiplier, "Multiplier", 6, buttonMultiply, 10);
  lv_obj_t* rpmGauge = tuneFlywheel.initGauge(0, "RPM", 2, 0, 3000);
  lv_obj_t* errorGauge = tuneFlywheel.initGauge(160, "Error", 1, -50, 50);
  lv_obj_t* powerGauge = tuneFlywheel.initGauge(320, "MotorPower", 1, -127, 127);
  //------------------------------------


  lib7842::velPID flywheelPID(flywheelPIDParams.kP, flywheelPIDParams.kD, flywheelPIDParams.kF, flywheelPIDParams.derivativeEma);
  lib7842::emaFilter rpmEma(flywheelPIDParams.readingEma);

  while(true)
  {
    flywheelPID.setGains(flywheelPIDParams.kP, flywheelPIDParams.kD, flywheelPIDParams.kF, flywheelPIDParams.derivativeEma);
    rpmEma.setGains(flywheelPIDParams.readingEma);

    flywheelRPM = rpmEma.filter(getFlywheelRPM());
    motorPower = flywheelPID.calculate(wantedFlywheelRPM, flywheelRPM);

    if(wantedFlywheelRPM <= 0) motorPower = 0;
    if(motorPower <= 0) motorPower = 0;
    if(motorPower > lastPower && lastPower < 10) lastPower = 10;
    if((motorPower - lastPower) > slewRate) motorPower = lastPower + slewRate;
    lastPower = motorPower;

    setFlywheelPower(motorPower);


    std::cout << "RPM: " << flywheelRPM << " Power: "<< motorPower << " Error: "<< flywheelPID.getError() << "\n";

    lv_gauge_set_value(rpmGauge, 0, wantedFlywheelRPM);
    lv_gauge_set_value(rpmGauge, 1, flywheelRPM);

    lv_gauge_set_value(errorGauge, 0, flywheelPID.getError());
    lv_gauge_set_value(powerGauge, 0, motorPower);

    pros::delay(20);
  }
}








// void flywheelTask(void*)
// {
//
//   pros::Motor m_flywheel(15, true);
//
//
// auto flywheelPID = IterativeControllerFactory::velPID(0.01, 1, 0.1);
//
// 	int slewRate = 1;
//   float motorPower = 0;
// 	float lastPower = 0;
//
// flywheelPID.setTarget(5);
//
// 	while(true)
// 	{
//
//
//     motorPower = flywheelPID.step(m_flywheel.get_actual_velocity()) * 127;
//
// std::cout << motorPower;
//
// 		if(motorPower < 0) motorPower = 0;
//
// 		if(motorPower > lastPower && lastPower < 20) lastPower = 20;
//
// 		if((motorPower - lastPower) > slewRate) motorPower = lastPower + slewRate;
//     lastPower = motorPower;
//
//
// std::cout << motorPower;
//
// 	m_flywheel.move(motorPower);
//
//
//
// 		pros::delay(50);
// 	}
// }

#pragma once

struct pidStruct
{
	float Kp;
	float Ki;
	float Kd;
	float Kf;

	int integralCap;
	int integralInner;
	int integralOuter;

	int Error;
  int totalError;
	int lastError;
	int lastTime;
	int lastIntegral;
  float derivative;

  EMAFilter raName;
};

class velPID
{

};

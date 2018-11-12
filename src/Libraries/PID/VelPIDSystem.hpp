#pragma once
#include "main.h"
#include "timingSystem.h"


namespace 7842Lib
{
	class velPID
	{
	private:

			double m_Kp = 0;
			double m_Kd = 0;
			double m_Kf = 0;

			double m_Error = 0;
			double m_lastError = 0;
			double m_lastTime = 0;
		  double m_derivative = 0;

			Timer m_timer();
		  EMAFilter m_dFilter();


	public:

velPID(double, double, double, double);
double calculate(double, double);
void setGains(double, double, double, double);
double getError();




	};
}

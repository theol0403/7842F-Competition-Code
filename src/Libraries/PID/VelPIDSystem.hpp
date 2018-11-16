#pragma once
#include "main.h"
#include "../Other/timingSystem.hpp"
#include "../Filter/emaFilter.hpp"
#include "../Other/miscUtils.hpp"


namespace lib7842
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

			7842lib::Timer m_timer;
		  emaFilter m_dFilter;


	public:

velPID(double, double, double, double);
double calculate(double, double);
void setGains(double, double, double, double);
double getError();




	};
}

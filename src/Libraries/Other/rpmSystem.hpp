#pragma once
#include "../Other/timingSystem.hpp"




 namespace lib7842
 {
   class rpmMeasure
   {
   private:

    lib7842::Timer m_timer;
    pros::ADIEncoder m_encoder;

    const double m_ticksPerRev = 360;
    const double m_flywheelEncoderRatio = 1;
    const int m_minRefresh = 20;

    double m_deltaTime = 0;
    double m_deltaEncoder = 0;

   	double m_lastTime = 0;
   	double m_lastEncoder = 0;

   	int m_RPM = 0;

   public:

   	rpmMeasure(int, bool, double, double);
    int calculate();



  };
 }

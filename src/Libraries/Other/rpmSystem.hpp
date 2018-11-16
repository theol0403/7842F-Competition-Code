#include "main.h"
#pragma once




 namespace lib7842
 {
   class rpmMeasure
   {
   private:

    Timer m_timer;
    pros::ADIEncoder m_encoder;

   	double m_lastTime = 0;
   	double m_lastEncoder = 0;

    double m_deltaTime = 0;
    double m_deltaEncoder = 0;

   	double m_ticksPerRev = 360;
    double m_flywheelEncoderRatio = 1;
   	int m_minRefresh = 20;

   	int m_RPM = 0;



   public:

   	rpmMeasure(int, bool, double, double);



  };
 }

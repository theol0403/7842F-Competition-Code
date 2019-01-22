#include "timingSystem.hpp"

 namespace lib7842
 {

   	Timer::Timer()
    : m_startTime(clock_t::now())
   	{
   	}

   	void Timer::reset()
   	{
   		m_startTime = clock_t::now();
   	}


   	double Timer::elapsed()
   	{
   		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(clock_t::now() - m_startTime).count();
   	}

 }

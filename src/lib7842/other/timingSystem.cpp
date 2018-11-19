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
   		return std::chrono::duration_cast<second_t>(clock_t::now() - m_startTime).count();
   	}

 }

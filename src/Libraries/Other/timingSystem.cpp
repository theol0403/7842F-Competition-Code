#include "main.h"
#include "timingSystem.h"

 namespace 7842Lib
 {

   public:

   	Timer::Timer()
    : m_startTime(clock_t::now())
   	{
   	}

   	void Timer::reset()
   	{
   		m_startTime = clock_t::now();
   	}

    double Timer::time()
   	{
   		return std::chrono::duration_cast<second_t>(clock_t::now()).count();
   	}

   	double Timer::elapsed()
   	{
   		return std::chrono::duration_cast<second_t>(clock_t::now() - m_startTime).count();
   	}

 }

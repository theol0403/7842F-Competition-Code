#pragma once

#include <chrono> // for std::chrono functions

 namespace lib7842
 {
   class Timer
   {
   private:

   	// Type aliases to make accessing nested type easier
   	using clock_t = std::chrono::high_resolution_clock;

   	std::chrono::time_point<clock_t> m_startTime;

   public:

   	Timer();
   	void reset();
   	double elapsed();

   };

 }

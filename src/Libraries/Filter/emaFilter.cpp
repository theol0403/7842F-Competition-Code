#include "main.h"

#include "emaFilter.hpp"

namespace lib7842
{


  EMAFilter::EMAFilter(double alpha)
  :
	m_alpha(alpha)
	{
	}

	double EMAFilter::filter(double readIn)
	{
		m_output = m_alpha * readIn + (1.0 - m_alpha) * m_outputOld;
		m_outputOld = m_output;
		return m_output;
	}

	double EMAFilter::filter(double readIn, double alpha)
	{
		m_alpha = alpha;
		return filter(readIn);
	}

	void EMAFilter::setGains(double alpha)
	{
		m_alpha = alpha;
	}



}

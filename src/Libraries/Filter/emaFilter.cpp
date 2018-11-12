#include "main.h"

#include "emaFilter.h"

namespace 7842Lib
{


  EMAFilter::EMAFilter(float alpha)
	m_alpha(alpha)
	{
	}

	float EMAFilter::filter(float readIn)
	{
		m_output = m_alpha * readIn + (1.0 - m_alpha) * m_outputOld;
		m_outputOld = m_output;
		return m_output;
	}

	float EMAFilter::filter(float readIn, float alpha)
	{
		m_alpha = alpha;
		return filterEMA(readIn);
		m_output = m_alpha * readIn + (1.0 - m_alpha) * m_outputOld;
	}

	void EMAFilter::setGains(float alpha)
	{
		m_alpha = alpha;
	}



}

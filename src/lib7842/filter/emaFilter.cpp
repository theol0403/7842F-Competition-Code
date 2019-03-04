#include "emaFilter.hpp"

namespace lib7842
{

  emaFilter::emaFilter(double alpha) : m_alpha(alpha) {}

	double emaFilter::filter(double readIn)
	{
		m_output = m_alpha * readIn + (1.0 - m_alpha) * m_outputOld;
		m_outputOld = m_output;
		return m_output;
	}

	double emaFilter::filter(double readIn, double alpha)
	{
		m_alpha = alpha;
		return filter(readIn);
	}

	void emaFilter::setGains(double alpha) { m_alpha = alpha; }

}

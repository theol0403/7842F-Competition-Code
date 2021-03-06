#pragma once

namespace lib7842 {
class emaFilter {

public:
  double m_alpha = 1.0;
  double m_output = 0.0;
  double m_outputOld = 0.0;

  emaFilter(double);
  double filter(double);
  double filter(double, double);
  void setGains(double);
};
} // namespace lib7842

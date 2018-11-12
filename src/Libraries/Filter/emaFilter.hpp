#include "main.h"

namespace lib7842
{
  class emaFilter
  {

  private:

    double m_alpha = 1.0;
    double m_output = 0.0;
    double m_outputOld = 0.0;

  public:

    emaFilter(double);
    double filter(double);
    double filter(double, double);
    void setGains(double);


  };
}

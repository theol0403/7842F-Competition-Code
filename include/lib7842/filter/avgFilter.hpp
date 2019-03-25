#pragma once
#include <vector>
#include <cstddef>

namespace lib7842
{
  class avgFilter
  {

  private:

    const int size;
    std::vector<double> data {0};
    std::size_t index = 0;
    double output = 0;

  public:

    avgFilter(int);

    double filter(double);
    double getOutput();

    void resetTo(double);
    void reset();

  };
}

#include "avgFilter.hpp"

namespace lib7842
{

  avgFilter::avgFilter(int isize) : size(isize) { data.resize(size); }

  double avgFilter::filter(double input) {
    data.at(index++) = input;
    if (index >= size) {
      index = 0;
    }

    output = 0.0;
    for(size_t i = 0; i < size; i++) {
      output += data.at(i);
    }

    output /= (double) size;

    return output;
  }
  

  double avgFilter::getOutput() {
    return output;
  }

  void avgFilter::resetTo(double input) {
    for(double &num : data) {
      num = input;
    }
  }

  void avgFilter::reset() {
    resetTo(0);
  }

}

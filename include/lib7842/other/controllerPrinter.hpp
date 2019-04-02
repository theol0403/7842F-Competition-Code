#pragma once
#include "main.h"
#include <string>

namespace lib7842
{
  class ControllerPrinter
  {

    Controller* controller = nullptr;
    const int line;
    Timer timer;

  public:

    ControllerPrinter(Controller*, int);
    void print(std::string);

  };

}

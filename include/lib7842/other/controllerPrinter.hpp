#pragma once
#include "main.h"
#include <string>

namespace lib7842
{
  class ControllerPrinter
  {

    Controller* controller = nullptr;
    int line = 0;
    Timer timer;

  public:

    ControllerPrinter(Controller*, int);
    void print(std::string);

  };

}

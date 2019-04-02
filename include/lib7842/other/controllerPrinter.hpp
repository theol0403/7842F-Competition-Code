#pragma once
#include "main.h"
#include <string>

namespace lib7842
{
  class ControllerPrinter
  {

  private:

    Controller* controller = nullptr;
    pros::Task task;

    std::array<std::string, 3> lines = {};

  public:

    ControllerPrinter(Controller*);
    void print(int, std::string);

    void run();
    static void taskFnc(void*);

  };

}

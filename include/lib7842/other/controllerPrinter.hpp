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
    bool paused = true;

  public:

    ControllerPrinter(Controller*);
    void print(int, std::string);
    void pause();

    void run();
    static void taskFnc(void*);

  };

}

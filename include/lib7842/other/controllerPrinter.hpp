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
    bool doRumble = false;
    std::string rumbleText = {};
    bool paused = true;

  public:

    ControllerPrinter(Controller*);
    void print(int, std::string);
    std::string get(int);
    void rumble(std::string);
    void pause();

    void run();
    static void taskFnc(void*);

  };

}

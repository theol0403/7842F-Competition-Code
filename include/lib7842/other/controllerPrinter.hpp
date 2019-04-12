#pragma once
#include "main.h"
#include <string>

namespace lib7842
{
  class ControllerPrinter
  {

  private:

    pros::Task task;
    std::map<pros::controller_id_e_t, std::array<std::string, 3>> lines = {};
    std::map<pros::controller_id_e_t, std::string> rumbleTexts = {};

  public:

    ControllerPrinter();

    void print(int, std::string, pros::controller_id_e_t = pros::E_CONTROLLER_MASTER);
    std::string get(int, pros::controller_id_e_t);
    void rumble(std::string, pros::controller_id_e_t);

    void run();
    static void taskFnc(void*);

  private:

    void m_print(pros::controller_id_e_t);

  };

}

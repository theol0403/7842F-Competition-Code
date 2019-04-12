#pragma once
#include "main.h"
#include <string>

namespace lib7842
{
  class ControllerPrinter
  {

  private:

    pros::Task task;

    std::array<std::string, 3> mLines = {};
    std::array<std::string, 3> pLines = {};

    std::string mRumbleText = "";
    std::string pRumbleText = "";

  public:

    ControllerPrinter();

    void print(int, std::string, pros::controller_id_e_t = pros::E_CONTROLLER_MASTER);
    std::string get(int, pros::controller_id_e_t);
    void rumble(std::string, pros::controller_id_e_t);

    void run();
    static void taskFnc(void*);

  private:

    void m_print(std::array<std::string, 3>&, pros::controller_id_e_t);

  };

}

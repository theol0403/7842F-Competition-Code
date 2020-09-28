#pragma once
#include "main.h"
#include <string>

namespace lib7842 {
class ControllerPrinter {

private:
  pros::Task task;
  std::array<std::array<std::string, 3>, 2> lines = {};
  std::array<std::array<std::string, 3>, 2> lastLines = {};
  std::array<std::string, 2> rumbleTexts = {};

public:
  ControllerPrinter();

  void print(int, std::string, pros::controller_id_e_t = pros::E_CONTROLLER_MASTER);
  void rumble(std::string, pros::controller_id_e_t = pros::E_CONTROLLER_MASTER);
  std::string get(int, pros::controller_id_e_t);
  void copy(int, pros::controller_id_e_t, pros::controller_id_e_t);

  void run();
  static void taskFnc(void*);
};

} // namespace lib7842

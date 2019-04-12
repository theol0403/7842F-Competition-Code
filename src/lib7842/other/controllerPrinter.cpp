#include "controllerPrinter.hpp"
using namespace pros::c;

namespace lib7842
{

  ControllerPrinter::ControllerPrinter() : task(taskFnc, this) {}

  void ControllerPrinter::print(int line, std::string str, pros::controller_id_e_t id) {
    lines[id].at(line) = str;
  }

  void ControllerPrinter::rumble(std::string str, pros::controller_id_e_t id) {
    rumbleTexts[id] = str;
  }
  
  std::string ControllerPrinter::get(int line, pros::controller_id_e_t id) {
    return lines[id].at(line);
  }


  void ControllerPrinter::m_print(pros::controller_id_e_t id) {
    const int maxWidth = 15;
    for(int i = 0; i < lines[id].size(); i++) {

      std::string str = lines[id].at(i);
      if(str.size() < maxWidth) {
        str.insert(str.end(), maxWidth - str.size(), ' ');
      } else {
        str.erase(str.begin() + maxWidth, str.end());
      }

      if(lines[id].at(i) == "") {
        controller_clear_line(id, i);
      } else {
        controller_set_text(id, i, 0, str.c_str());
      }

      pros::delay(52);

      if(rumbleTexts[id] != "") {
        controller_rumble(id, rumbleTexts[id].c_str());
        pros::delay(52);
        rumbleTexts[id] = "";
      }
    }
  }


  void ControllerPrinter::run() {

    while(true) {
      bool controllerConnected = false;

      if(pros::c::controller_is_connected(pros::E_CONTROLLER_MASTER)) {
        m_print(pros::E_CONTROLLER_MASTER);
        controllerConnected = true;
      }

      if(pros::c::controller_is_connected(pros::E_CONTROLLER_PARTNER)) {
        m_print(pros::E_CONTROLLER_PARTNER);
        controllerConnected = true;
      }

      if(!controllerConnected) {
        pros::delay(52);
      }
    }
  }

  void ControllerPrinter::taskFnc(void* input) {
    ControllerPrinter* that = static_cast<ControllerPrinter*>(input);
    that->run();
  }


}

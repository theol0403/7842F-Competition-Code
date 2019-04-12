#include "controllerPrinter.hpp"
using namespace pros::c;

namespace lib7842
{

  ControllerPrinter::ControllerPrinter() : task(taskFnc, this) {}

  void ControllerPrinter::print(int line, std::string str, pros::controller_id_e_t id) {
    if(id == pros::E_CONTROLLER_MASTER) {
      mLines.at(line) = str;
    } else {
      pLines.at(line) = str;
    }
  }

  std::string ControllerPrinter::get(int line, pros::controller_id_e_t id) {
    if(id == pros::E_CONTROLLER_MASTER) {
      return mLines.at(line);
    } else {
      return pLines.at(line);
    }
  }

  void ControllerPrinter::rumble(std::string str, pros::controller_id_e_t id) {
    if(id == pros::E_CONTROLLER_MASTER) {
      mRumbleText = str;
    } else {
      pRumbleText = str;
    }
  }


  void ControllerPrinter::m_print(std::array<std::string, 3>& lines, pros::controller_id_e_t id) {

    const int maxWidth = 15;

    for(int i = 0; i < lines.size(); i++) {

      std::string str = lines.at(i);
      if(str.size() < maxWidth) {
        str.insert(str.end(), maxWidth - str.size(), ' ');
      } else {
        str.erase(str.begin() + maxWidth, str.end());
      }

      if(lines.at(i) == "") {
        controller_clear_line(if, i);
      } else {
        controller_set_text(if, i, 0, str.c_str());
      }

      pros::delay(52);

      if(doRumble) {
        pros::delay(52);
        controller_rumble(if, rumbleText.c_str());
        pros::delay(52);
        doRumble = false;
      }
    }

  }


  void ControllerPrinter::run() {

    while(true) {

      if(!paused) {
        paused = true;



      } else {
        pros::delay(52);
      }
    }
  }


  void ControllerPrinter::taskFnc(void* input) {
    ControllerPrinter* that = static_cast<ControllerPrinter*>(input);
    that->run();
  }


}

#include "controllerPrinter.hpp"
using namespace pros::c;

namespace lib7842
{

  ControllerPrinter::ControllerPrinter(pros::controller_id_e_t icontroller)
  : controller(icontroller), task(taskFnc, this) {}

  void ControllerPrinter::print(int line, std::string str) {
    lines.at(line) = str;
    if(str != "") paused = false;
  }

  std::string ControllerPrinter::get(int line) {
    return lines.at(line);
  }

  void ControllerPrinter::rumble(std::string str) {
    rumbleText = str;
    doRumble = true;
    paused = false;
  }

  void ControllerPrinter::pause() {
    paused = true;
    controller_clear(controller);
  }


  void ControllerPrinter::run() {
    const int maxWidth = 20;
    while(true) {

      if(!paused) {
        paused = true;

        for(int i = 0; i < lines.size(); i++) {

          std::string str = lines.at(i);
          if(str.size() < maxWidth) {
            str.insert(str.end(), maxWidth - str.size(), ' ');
          } else {
            str.erase(str.begin() + maxWidth, str.end());
          }

          if(lines.at(i) == "") {
            controller_clear_line(controller, i);
          } else {
            controller_set_text(controller, i, 0, str.c_str());
          }

          pros::delay(52);

          if(doRumble) {
            pros::delay(52);
            controller_rumble(controller, rumbleText.c_str());
            pros::delay(52);
            doRumble = false;
          }
        }

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

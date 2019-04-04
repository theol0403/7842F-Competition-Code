#include "controllerPrinter.hpp"

namespace lib7842
{

  ControllerPrinter::ControllerPrinter(Controller* icontroller)
  : controller(icontroller), task(taskFnc, this) {
  }


  void ControllerPrinter::print(int line, std::string str) {
    lines.at(line) = str;
    paused = false;
  }

  void ControllerPrinter::rumble(std::string str) {
    rumbleText = str;
    doRumble = true;
  }

  void ControllerPrinter::pause() {
    paused = true;
    controller->clear();
  }


  void ControllerPrinter::run() {
    const int maxWidth = 20;
    while(true) {

      if(!paused) {

        for(int i = 0; i < lines.size(); i++) {

          std::string str = lines.at(i);
          if(str.size() < maxWidth) {
            str.insert(str.end(), maxWidth - str.size(), ' ');
          } else {
            str.erase(str.begin() + maxWidth, str.end());
          }

          controller->setText(i, 0, str);
          pros::delay(52);

          if(doRumble) {
            pros::delay(52);
            controller->rumble(rumbleText);
            pros::delay(52);
            doRumble = false;
          }
        }

      } else {
        pros::delay(20);
      }
    }
  }

  void ControllerPrinter::taskFnc(void* input)
  {
    ControllerPrinter* that = static_cast<ControllerPrinter*>(input);
    that->run();
  }


}

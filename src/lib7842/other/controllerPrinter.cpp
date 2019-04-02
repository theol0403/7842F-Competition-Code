#include "controllerPrinter.hpp"

namespace lib7842
{

  ControllerPrinter::ControllerPrinter(Controller* icontroller)
  : controller(icontroller), task(taskFnc, this) {
    controller->clear();
  }


  void ControllerPrinter::print(int line, std::string str) {
    lines.at(line) = str;
  }


  void ControllerPrinter::run() {
    const int maxWidth = 20;
    while(true) {
      for(int i = 0; i < lines.size(); i++) {
        std::string str = lines.at(i);
        if(str.size() < maxWidth) {
          str.insert(str.end(), maxWidth - str.size(), ' ');
        } else {
          str.erase(str.begin() + maxWidth, str.end());
        }
        controller->setText(i, 0, str);
        pros::delay(52);
      }
    }
    //std::cout << "Set Text: " << str << std::endl;
  }

  void ControllerPrinter::taskFnc(void* input)
  {
    pros::delay(500);
    ControllerPrinter* that = static_cast<ControllerPrinter*>(input);
    that->run();
  }


}

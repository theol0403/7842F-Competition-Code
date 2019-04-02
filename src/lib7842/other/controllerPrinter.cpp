#include "controllerPrinter.hpp"

namespace lib7842
{

  ControllerPrinter::ControllerPrinter(Controller* icontroller, int iline)
  : controller(icontroller), line(iline) {
    controller->clear();
  }

  void ControllerPrinter::print(std::string str) {
    const int maxWidth = 14;

    if(timer.repeat(52_ms)) {

      // if(str.size() < maxWidth) {
      //   str.insert(str.end(), maxWidth - str.size(), ' ');
      // } else {
      //   str.erase(str.begin() + maxWidth, str.end());
      // }

      controller->setText(line, 0, str);
      //std::cout << "Set Text: " << str << std::endl;
    }

  }


}

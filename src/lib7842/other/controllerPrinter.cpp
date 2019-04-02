#include "controllerPrinter.hpp"

namespace lib7842
{

  ControllerPrinter::ControllerPrinter(Controller* icontroller, int iline)
  : controller(icontroller), line(iline) {}

  void ControllerPrinter::print(std::string str) {
    const int maxWidth = 15;
    if(timer.getDtFromMark() > 50_ms) {
      timer.placeMark();
      if(str.size() < maxWidth) {
        str.insert(str.end(), maxWidth - str.size(), ' ');
      } else {
        str.erase(str.begin() + maxWidth, str.end());
      }
      controller->setText(line, 0, str);
    }
  }

}

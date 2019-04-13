#include "controllerPrinter.hpp"
using namespace pros::c;

namespace lib7842
{

  ControllerPrinter::ControllerPrinter() : task(taskFnc, this) {}

  void ControllerPrinter::print(int line, std::string str, pros::controller_id_e_t id) {
    //std::cout << "print: \"" << str << "\" at: " << id << std::endl;
    lines[id].at(line) = str;
  }

  void ControllerPrinter::rumble(std::string str, pros::controller_id_e_t id) {
    rumbleTexts[id] = str;
  }

  std::string ControllerPrinter::get(int line, pros::controller_id_e_t id) {
    return lines[id].at(line);
  }

  void ControllerPrinter::copy(int line, pros::controller_id_e_t source, pros::controller_id_e_t dest) {
    lines[dest].at(line) = lines[source].at(line);
  }

  void ControllerPrinter::run() {

    const int maxWidth = 15;

    while(true) {
      bool controllerConnected = false;

      for(int id = 0; id < lines.size(); id++) {
        if(pros::c::controller_is_connected((pros::controller_id_e_t)id)) {
          controllerConnected = true;

          for(int i = 0; i < lines[id].size(); i++) {

            std::string str = lines[id].at(i);
            if(str.size() < maxWidth) {
              str.insert(str.end(), maxWidth - str.size(), ' ');
            } else {
              str.erase(str.begin() + maxWidth, str.end());
            }

            if(lines[id].at(i) != "") {
              controller_set_text((pros::controller_id_e_t)id, i, 0, str.c_str());
            }

            pros::delay(52);

            if(rumbleTexts[id] != "") {
              controller_rumble((pros::controller_id_e_t)id, rumbleTexts[id].c_str());
              pros::delay(52);
              rumbleTexts[id] = "";
            }
          }

        }
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

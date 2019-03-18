#include "sdLogger.hpp"

namespace lib7842
{

  SDLogger::SDLogger(std::string name, modes mode) :
  path(findPath(name, mode)), writer(path), refreshTime(3_s) {
    timer.placeMark();
  }

  bool SDLogger::fileExists(std::string name) {
    FILE *file;
    if ((file = fopen(name.c_str(), "r"))){
      fclose(file);
      return true;
    }
    return false;
  }


  std::string SDLogger::findPath(std::string name, modes mode) {
    std::string path;
    switch(mode) {

      case count: {
        int fileNum = 0;
        if(fileExists(name)) {
          FILE* file = fopen(name.c_str(), "r");
          fscanf(file, "%d", &fileNum);
          fclose(file);
        } else {
          FILE* file = fopen(name.c_str(), "w");
          fprintf(file, "%d", 0);
          fclose(file);
        }
        do {
          path = "/usd/" + name + std::to_string(fileNum) + ".csv";
          fileNum++;
          std::cout << "Trying: " << path << std::endl;
        }
        while(fileExists(path));
        FILE* file = fopen(name.c_str(), "w");
        fprintf(file, "%d", fileNum);
        fclose(file);
        break;
      }

      case time: {
        pros::date_s_t date = pros::get_date();
        pros::time_s_t time = pros::get_time();
        path = "/usd/" + name + std::to_string(date.year) + "-" + std::to_string(date.month) + "-" + std::to_string(date.day) + "_" + std::to_string(time.hour) + "-" + std::to_string(time.min) + ".csv";
        break;
      }

    }

    std::cout << "Using: " << path << std::endl;
    return path;
  }

  void SDLogger::writeFields(std::vector<std::string> fields) {
    for(std::string &field : fields) {
      writer.WriteField(field, false);
    }
    writer.WriteTerminator();
  }

  void SDLogger::writeLine(std::vector<std::string> records) {
    writer.WriteRecord(records, true);

    if(timer.getDtFromMark() > refreshTime) {
      writer.Close();
      writer.Open(path);
      timer.placeMark();
      //std::cout << "REOPEN" << std::endl;
    }
  }






}

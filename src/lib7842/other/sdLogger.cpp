#include "sdLogger.hpp"

namespace lib7842
{

  SDLogger::SDLogger(std::string name, modes mode) :
  path(findPath(name, mode)), writer(path), refreshTime(2_s) {
    timer.placeMark();
    if(writer.error) writeFail(name);
  }

  bool SDLogger::fileExists(std::string name) {
    FILE *file;
    if ((file = fopen(name.c_str(), "r"))){
      fclose(file);
      return true;
    }
    return false;
  }

  void SDLogger::writeFail(std::string name) {
    std::cerr << name << ": SD Fail" << std::endl;
    writeError = true;
  }


  std::string SDLogger::findPath(std::string name, modes mode) {
    std::string path;
    switch(mode) {

      case count: {
        std::string indexFile = "/usd/" + name + "_index.txt";
        int fileNum = 0;
        if(fileExists(indexFile)) {
          FILE* file = fopen(indexFile.c_str(), "r");
          fscanf(file, "%d", &fileNum);
          fclose(file);
        } else {
          FILE* file = fopen(indexFile.c_str(), "w");
          if(!file) return "error";
          fprintf(file, "%d", 0);
          fclose(file);
        }
        do {
          path = "/usd/" + name + std::to_string(fileNum) + ".csv";
          //std::cout << "Trying: " << path << std::endl;
          fileNum++;
        }
        while(fileExists(path));
        FILE* file = fopen(indexFile.c_str(), "w");
        fprintf(file, "%d", fileNum);
        fclose(file);
        break;
      }

      case time: {
        pros::date_s_t date = pros::get_date();
        pros::time_s_t time = pros::get_time();
        //%Y%m%dT%H%M%S
        path = "/usd/" + name + std::to_string(date.year) + std::to_string(date.month) + std::to_string(date.day) + "T" + std::to_string(time.hour) + std::to_string(time.min) + std::to_string(time.sec) + ".csv";
        break;
      }

      case cout: {
        path = "/ser/sout";
        break;
      }

    }

    std::cout << "Using: " << path << std::endl;
    return path;
  }

  void SDLogger::writeFields(std::vector<std::string> fields) {
    if(!writeError) {
      for(std::string &field : fields) {
        writer.WriteField(field, false);
      }
      writer.WriteTerminator();
    }
  }

  void SDLogger::writeLine(std::vector<std::string> records) {
    if(!writeError) {
      writer.WriteRecord(records, true);

      if(timer.getDtFromMark() > refreshTime) {
        writer.Close();
        writer.Open(path);
        timer.placeMark();
      }
    }
  }

}

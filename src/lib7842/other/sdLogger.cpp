#include "sdLogger.hpp"

namespace lib7842
{

  SDLogger::SDLogger(std::string name) :
  path(findPath(name)), writer(path), refreshTime(3_s) {
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


  std::string SDLogger::findPath(std::string name) {
    int fileNum = 0;
    std::string path;
    do {
      path = "/usd/" + name + std::to_string(fileNum) + ".csv";
      fileNum++;
      std::cout << "Trying: " << path << std::endl;
    }
    while(fileExists(path));
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

#include "sdLogger.hpp"

namespace lib7842
{

  SDLogger::SDLogger(std::string folder, std::string name) :
  path(findPath(folder, name)), writer(path), refreshTime(3_s)
  {
    timer.placeMark();
  }

  bool SDLogger::fileExists(std::string name) {
    FILE *file;
    if (file = fopen(name.c_str(), "r")){
      fclose(file);
      return 1;
    }
    return 0;
  }

  std::string SDLogger::findPath(std::string folder, std::string name) {
    int fileNum = 0;
    std::string path;
    do {
      path = "usd/" + folder + "/" + name + std::to_string(fileNum) + ".csv";
      fileNum++;
    }
    while(!fileExists(path));
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

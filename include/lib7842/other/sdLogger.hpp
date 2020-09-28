#pragma once
#include "jaycsv/CSV.hpp"
#include "main.h"
#include <string>

namespace lib7842 {
class SDLogger {

public:
  enum modes { count, time, cout };

  SDLogger(std::string, modes = count);

  bool fileExists(std::string);
  std::string findPath(std::string, modes);
  void writeFail(std::string);

  void writeFields(std::vector<std::string>);
  void writeLine(std::vector<std::string>);

private:
  std::string path;
  jay::util::CSVwrite writer;
  const QTime refreshTime;

  Timer timer;
};
} // namespace lib7842

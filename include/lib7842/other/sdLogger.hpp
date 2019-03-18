#pragma once
#include "main.h"
#include <string>
#include "jaycsv/CSV.hpp"
#include <sys/stat.h>
#include <unistd.h>

namespace lib7842
{
  class SDLogger
  {
  private:

    std::string path;
    jay::util::CSVwrite writer;
    const QTime refreshTime;

    Timer timer;

  public:

    SDLogger(std::string, std::string);

    bool fileExists(std::string);
    std::string findPath(std::string, std::string);

    void writeField(std::vector<std::string>);
    void writeLine(std::vector<std::string>);


  };
}

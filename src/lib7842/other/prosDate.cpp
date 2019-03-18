#include "prosDate.hpp"

namespace lib7842
{

  typedef date_s_t date;
  typedef time_s_t time;

  void vexGetdate(date*);
  void vexGettime(time*);

  date_s_t get_date(void) {
    date_s_t d;
    vexGetdate((date*)(&d));
    return d;
  }

  time_s_t get_time(void) {
    time_s_t t;
    vexGettime((time*)(&t));
    return t;
  }

}

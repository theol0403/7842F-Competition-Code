#pragma once
#include "main.h"

namespace lib7842
{

  typedef struct {
    uint16_t year; // Year - 1980
    uint8_t day;
    uint8_t month; // 1 = January
  } date_s_t;

  typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t sec_hund; // hundredths of a second
  } time_s_t;


  /**
  * Gets the current date, as reported by VEXos.
  *
  * \return A data_s_t containing the Year, Month, and Day of the date.
  */
  date_s_t get_date(void);

  /**
  * Gets the current time, as reported by VEXos.
  *
  * \return A time_s_t containing the Hour, Minute, Second, and Hundredth of Second.
  */
  time_s_t get_time(void);

}

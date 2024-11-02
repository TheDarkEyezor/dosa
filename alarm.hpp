#ifndef ALARM_HPP
#define ALARM_HPP
#include "commands.hpp"

class alarm: public Command{
  private:
    int hour;
    int minute;

    int calculate_time_difference(int hour, int minute);
    void trigger_alarm_notification();
    
  public:
    int init (int hour, int minute);
    int init (int hour);
    int execute () override;
};

#endif

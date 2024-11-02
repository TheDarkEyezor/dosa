#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>
#include "alarm.hpp"


// Function to calculate the time difference in seconds
int alarm::calculate_time_difference(int hour, int minute = 0) {
  time_t now = time(nullptr);
  tm *current_time = localtime(&now);

  int current_hour = current_time->tm_hour;
  int current_minute = current_time->tm_min;
  int current_sec = current_time->tm_sec;

  int time_diff_minutes = (hour - current_hour) * 3600 + (minute - current_minute) * 60 + (0 - current_sec);

  // If the target time is before the current time, assume it's the next day
  if (time_diff_minutes < 0) {
      time_diff_minutes += 24 * 3600;
  }
  return time_diff_minutes; // Convert to seconds
}

// Function to display a macOS notification
void alarm::trigger_alarm_notification() {
  system("osascript -e 'display notification \"Time is up!\" with title \"Alarm\" sound name \"Glass\"'");
}

// Function to start the alarm
int alarm::execute () {
  int seconds_to_alarm = calculate_time_difference(hour, minute);
  std::cout << "Alarm set for " << hour << ":" << (minute < 10 ? "0" : "") << minute 
            << ". Waiting " << seconds_to_alarm << " seconds..." << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(seconds_to_alarm - 12));
  trigger_alarm_notification();
  return 0;
}

int alarm::init(int hour, int minute) {
  this->hour = hour;
  this->minute = minute;
  return 0;
}

int alarm::init(int hour) {
  init(hour, 0);
  return 0;
}
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "alarm.hpp"
#include "info.hpp"
#include "timeParser.cpp"

using namespace std;

Command* parse_command(const string& command) {
  if (command.find("alarm") != string::npos) {
    // Function to parse command and extract hour and minute
    auto result = TimeParser::parse(command);
    alarm* newAlarm = new alarm();

    if (result->minutes.has_value()) {
      if (result->minutes.has_value())
        newAlarm->init(result->hour, result->minutes.value());
      else 
        newAlarm->init(result->hour);
      return newAlarm;
    } else {
      cout << "alarm command recognised, but couldn't parse time" << endl;
    }
  } 
  else if (command.find("exit") != string::npos) {
    cout << "EEXXIITTIINNGG PPRROOGGRRAAMM!!" << endl;
    exit(0);
  } 
  else if (command.find("search") != string::npos) {
    info* newInfo = new info();
    cout << "Information command received. TODO implementing info retrieval" << endl;
  } 
  else {
    cout << "only accepting alarm commands rn" << endl;
  }
  return nullptr;
}

int main() {
  while (true) {
    string command;
    cout << "Enter your command: ";
    getline(cin, command);
    Command *newCommand = parse_command(command);

    if (!newCommand) {
      continue;
    }

    // Start task in a separate thread
    thread newCommandThread(&Command::execute, newCommand);

    // A stdout message to show clearly what the command it doing
    /* This is because especially with threads, having the execute function display when it's in a separate thread means it'll display at the wrong time*/
    newCommand->display();

    // Detach the alarm thread so it runs independently
    newCommandThread.detach();

    // Main thread can do other tasks here
    cout << "Alarm is set. Main program is free to do other tasks." << endl;
  }
  return 0;
}

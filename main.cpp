#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "alarm.hpp"


// Function to parse command and extract hour and minute
Command *parse_command(const std::string &command) {
    alarm emptyAlarm;
    std::istringstream stream(command);
    std::string temp;
    
    // Parsing "set an alarm for HH:MM today"
    while (stream >> temp) {
        if (temp.find(':') != std::string::npos) {
            size_t colon_pos = temp.find(':');
            int hour = std::stoi(temp.substr(0, colon_pos));
            int minute = std::stoi(temp.substr(colon_pos + 1));
            alarm newAlarm;
            newAlarm.init(hour, minute);
            return &newAlarm;
        }
    }
    return nullptr;
}

int main() {
    std::string command;
    std::cout << "Enter your command: ";
    std::getline(std::cin, command);

    Command *bruh = parse_command(command);
    // // Start alarm in a separate thread
    // std::thread alarm_thread([bruh]() { bruh->execute(); });

    // // Detach the alarm thread so it runs independently
    // alarm_thread.detach();

    bruh->execute();

    // Main thread can do other tasks here
    std::cout << "Alarm is set. Main program is free to do other tasks." << std::endl;
    
    // Example of another task in the main program
    for (int i = 1; i <= 60; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Main program working... (" << i << "s)\n";

    }
    // } else {
    //     std::cout << "Failed to parse command. Please use format 'set an alarm for HH:MM today'." << std::endl;
    // }

    return 0;
}

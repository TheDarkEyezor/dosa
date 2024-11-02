#include <iostream>
#include <string>
#include <regex>
#include <optional>
#include <cctype>
#include <algorithm>
#include <map>
#include <vector>

struct TimeResult {
  int hour;
  std::optional<int> minutes;
  
  void normalize() {
    // Special handling for 12 AM/PM
    if (hour == 12) {
      hour = 0;  // Will be adjusted later if PM
    }
  }
};

class TimeParser {
private:
  static std::string toLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
  }

  static void stripNonAlphanumeric(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), 
      [](char c) { return !std::isalnum(c); }), str.end());
  }

  static bool hasMultipleTimeIndicators(const std::string& timeLower) {
    int count = 0;
    std::vector<std::string> indicators = {
      "quarter past", "quarter to", "half past",
      "o'clock", "noon", "midnight"
    };
    
    for (const auto& indicator : indicators) {
      if (timeLower.find(indicator) != std::string::npos) count++;
    }
    
    // Check for HH:MM format
    if (std::regex_search(timeLower, std::regex("\\d+[:.]\\d+"))) count++;
    
    return count > 1;
  }

    static bool parseTextualTime(const std::string& timeLower, TimeResult& result) {
      // Handle special cases first
      if (timeLower == "noon") {
        result.hour = 12;
        result.minutes = 0;
        return true;
      }
      if (timeLower == "midnight") {
        result.hour = 0;
        result.minutes = 0;
        return true;
      }

      // Parse "quarter past", "half past", "quarter to"
      std::regex hourExtractor("(\\d+|noon|midnight)");
      std::smatch hourMatch;
      
      if (timeLower.find("quarter past") != std::string::npos ||
        timeLower.find("15 past") != std::string::npos) {
        if (std::regex_search(timeLower, hourMatch, hourExtractor)) {
          if (hourMatch[1] == "noon") result.hour = 12;
          else if (hourMatch[1] == "midnight") result.hour = 0;
          else result.hour = std::stoi(hourMatch[1]);
          result.minutes = 15;
          return true;
        }
      }
      
      if (timeLower.find("quarter to") != std::string::npos ||
        timeLower.find("15 to") != std::string::npos) {
        if (std::regex_search(timeLower, hourMatch, hourExtractor)) {
          if (hourMatch[1] == "noon") result.hour = 11;
          else if (hourMatch[1] == "midnight") result.hour = 23;
          else result.hour = std::stoi(hourMatch[1]) - 1;
          result.minutes = 45;
          return true;
        }
      }
      
      if (timeLower.find("half past") != std::string::npos ||
        timeLower.find("30 past") != std::string::npos) {
        if (std::regex_search(timeLower, hourMatch, hourExtractor)) {
          if (hourMatch[1] == "noon") result.hour = 12;
          else if (hourMatch[1] == "midnight") result.hour = 0;
          else result.hour = std::stoi(hourMatch[1]);
          result.minutes = 30;
          return true;
        }
      }

      return false;
    }

public:
    static std::optional<TimeResult> parse(std::string timeStr) {
      TimeResult result{0, std::nullopt};
      
      // Convert to lowercase for easier parsing
      std::string timeLower = toLowerCase(timeStr);
      
      // Reject if multiple time indicators are present
      if (hasMultipleTimeIndicators(timeLower)) {
        return std::nullopt;
      }

      // Try parsing textual formats first
      if (parseTextualTime(timeLower, result)) {
        // Check for PM
        if (timeLower.find("pm") != std::string::npos) {
          if (result.hour != 12) {  // Don't adjust if it's 12 PM
            result.hour += 12;
          }
        } else if (timeLower.find("am") != std::string::npos && result.hour == 12) {
          result.hour = 0;  // 12 AM = 0 hours
        }
        return result;
      }

      // Remove whitespace
      timeStr.erase(std::remove_if(timeStr.begin(), timeStr.end(), ::isspace), timeStr.end());
      
      // Check for AM/PM
      bool isPM = false;
      if (timeLower.find("pm") != std::string::npos) isPM = true;
      
      // Remove AM/PM from string
      timeStr.erase(std::remove_if(timeStr.begin(), timeStr.end(), 
        [](char c) { return std::isalpha(c); }), timeStr.end());
      
      // Pattern for "HH:MM" or "H:MM"
      std::regex timePattern("([0-9]{1,2})[:.]?([0-9]{2})?");
      std::smatch matches;
      
      if (std::regex_search(timeStr, matches, timePattern)) {
        // Hour is always present
        result.hour = std::stoi(matches[1]);
        
        // Minutes are optional
        if (matches[2].matched) {
          result.minutes = std::stoi(matches[2]);
        }
        
        // Handle AM/PM
        if (isPM) {
          if (result.hour != 12) {  // Don't adjust if it's 12 PM
            result.hour += 12;
          }
        } else if (timeLower.find("am") != std::string::npos && result.hour == 12) {
          result.hour = 0;  // 12 AM = 0 hours
        }
        
        result.normalize();
      } else {
        // If no pattern matched, try to extract just the hour
        std::string numericOnly;
        for (char c : timeStr) {
          if (std::isdigit(c)) numericOnly += c;
        }
        
        if (!numericOnly.empty()) {
          result.hour = std::stoi(numericOnly);
          if (isPM && result.hour != 12) {
            result.hour += 12;
          } else if (timeLower.find("am") != std::string::npos && result.hour == 12) {
            result.hour = 0;
          }
          result.normalize();
        } else {
          return std::nullopt;  // No valid time found
        }
      }

      // Validate hours and minutes
      if (result.hour < 0 || result.hour > 23) {
        return std::nullopt;
      }
      if (result.minutes.has_value() && 
        (result.minutes.value() < 0 || result.minutes.value() > 59)) {
        return std::nullopt;
      }
      
      return result;
    }
};

// int main() {
//   std::vector<std::string> testTimes = {
//     "5",
//     "5:30",
//     "5:30pm",
//     "12pm",      // Should be noon (12:00)
//     "12am",      // Should be midnight (00:00)
//     "5 o'clock",
//     "5.30",
//     "17:00",
//     "quarter past 5",
//     "quarter to 6",
//     "half past 3",
//     "noon",
//     "midnight",
//     "quarter past noon",
//     "half past midnight",
//     "invalid time",
//     "quarter past half past 5",  // Invalid - multiple indicators
//     "25:00",     // Invalid hour
//     "5:60",      // Invalid minute
//     "13pm",      // Invalid hour
//     "midnight pm" // Invalid combination
//     };
    
//   for (const auto& time : testTimes) {
//     auto result = TimeParser::parse(time);
//     std::cout << "Input: " << time << " -> ";
//     if (result.has_value()) {
//       std::cout << "Hour: " << result->hour;
//       if (result->minutes.has_value()) {
//         std::cout << ", Minutes: " << result->minutes.value();
//       }
//     } else {
//       std::cout << "Invalid time format";
//     }
//     std::cout << std::endl;
//   }
  
//   return 0;
// }
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class TimeUtils{
  private:
  inline static std::string Remove(const std::string &str, char c) {
    std::string ret(str);
    ret.erase(std::remove(ret.begin(), ret.end(), c), ret.end());
    return ret;
  }

  inline static void Split(const std::string &str, char d,
                    std::vector<std::string> *out) {
    std::istringstream ss(str);
    std::string s;
    while (std::getline(ss, s, d)) {
        out->push_back(s);
    }
  }

  inline static int ToInt(const std::string &str) {
    return std::strtol(str.c_str(), NULL, 10);
  }

  inline static int GetMaxDay(int year, int month) {
    switch (month) {
      case 1: return 31;
      case 2: return ((year % 4 == 0)? 29 : 28);
      case 3: return 31;
      case 4: return 30;
      case 5: return 31;
      case 6: return 30;
      case 7: return 31;
      case 8: return 31;
      case 9: return 30;
      case 10: return 31;
      case 11: return 30;
      case 12: return 31;
      default: return 0;
    }
  }

  inline static std::string FormatNumber(int num, int size) {
    std::stringstream ss;
    ss << num;
    std::string tmp = ss.str();
    std::string ret = tmp;
    int remain = size - tmp.size();
    while (remain > 0) {
      ret = std::string("0") + ret;
      remain--;
    }
    return ret;
  }

  public:
  inline  static int TimeStringToSeconds(const std::string &time) {
    std::vector<std::string> time_parts;
    Split(time, ':', &time_parts);
    int hour = ToInt(time_parts[0]);
    int minute = ToInt(time_parts[1]);
    int second = ToInt(time_parts[2]);
    return second + 60 * minute + 60 * 60 * hour;
  }

  std::string static GetEndDate(const std::string &start_date, const std::string &elapsed_time) {
    std::vector<std::string> parts;
    Split(start_date, ' ', &parts);
    std::string &date = parts[0];
    std::string &time = parts[1];

    std::vector<std::string> time_parts, date_parts;
    Split(date, '/', &date_parts);
    Split(time, ':', &time_parts);
    int year, month, day;
    if (date_parts.size() > 2) {
      year = ToInt(date_parts[0]);
      month = ToInt(date_parts[1]);
      day = ToInt(date_parts[2]);
    } else {
      year = 2016;
      month = ToInt(date_parts[0]);
      day = ToInt(date_parts[1]);
    }
    int hour = ToInt(time_parts[0]);
    int minute = ToInt(time_parts[1]);
    int second = ToInt(time_parts[2]);

    std::vector<std::string> elapsed_parts;
    Split(elapsed_time, ':', &elapsed_parts);
    int elapsed_hour = ToInt(elapsed_parts[0]);
    int elapsed_minute = ToInt(elapsed_parts[1]);
    int elapsed_second = ToInt(elapsed_parts[2]);

    int total_second = elapsed_second + second;
    int a_minute = 0;
    if (total_second >= 60) {
      a_minute = 1;
      total_second -= 60;
    }
    int total_minute = elapsed_minute + minute + a_minute;
    int a_hour = 0;
    if (total_minute >= 60) {
      a_hour = 1;
      total_minute -= 60;
    }
    int total_hour = elapsed_hour + hour + a_hour;
    int a_day = 0;
    if (total_hour >= 24) {
      a_day = 1;
      total_hour -= 24;
    }
    int total_day = day + a_day;
    int max_day = GetMaxDay(year, month);
    int a_month = 0;
    if (total_day > max_day) {
      total_day = 1;
      a_month = 1;
    }
    int total_month = month + a_month;
    int a_year = 0;
    if (total_month > 12) {
      a_year = 1;
      total_month = 1;
    }
    int total_year = year + a_year;

    std::stringstream ss;
    ss << total_year << "/" << FormatNumber(total_month, 2) << "/" << FormatNumber(total_day, 2) << " ";
    ss << FormatNumber(total_hour, 2) << ":" << FormatNumber(total_minute, 2) << ":" << FormatNumber(total_second, 2);
    return ss.str();
  }

};

//int main(int argc, char **argv) {
//  if (argc < 3)
//   return 1;

//  std::string start_date = Remove(argv[1], '"');
//  std::cout << "Start date: " << start_date << std::endl;
//  std::string elapsed_time = Remove(argv[2], '"');
//  std::cout << "Elapsed time: " << elapsed_time << std::endl;

//  std::string end_date = GetEndDate(start_date, elapsed_time);
//  std::cout << "End date: " << end_date << std::endl;
//  return 0;
//}

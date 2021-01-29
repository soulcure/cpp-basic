#ifndef TIME_UTIL_H
#define TIME_UTIL_H
#include <string>
#include <ctime>

#include <sstream>


namespace songtzu {
    std::string timestamp_datetime();
    std::string timestamp_str();
    int timestamp_int();

    template <class T>
    inline std::string to_string (const T& t){
        std::stringstream ss;
        ss << t;
        return ss.str();
    }




}





#endif // TIME_UTIL_H

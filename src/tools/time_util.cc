#include "time_util.h"


using namespace std;

namespace songtzu {
    std::string timestamp_datetime(){
        std::string datetime;
        char datatimetmp[25] = "";
        time_t t;
        tm *tmp;

        t = time(nullptr);
        tmp = localtime(&t);

        strftime(datatimetmp, 24, "%Y-%m-%d %H:%M:%S", tmp);
        datetime = datatimetmp;

        return datetime;
    }
    string timestamp_str(){
        return to_string(time(nullptr));
    }

    int timestamp_int(){
        return time(nullptr);
    }

}


#ifndef LOGIC_TOOLS_H
#define LOGIC_TOOLS_H

#include "typedef.h"
#include <string>
#include <vector>

namespace songtzu {
    std::string build_topic(int _topic);

    int convert_str_to_topics(std::string _topic, std::vector<Topic_t> &_vec );

}


#endif // LOGIC_TOOLS_H

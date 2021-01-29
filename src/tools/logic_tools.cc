#include "logic_tools.h"
#include "string_split.h"
#include "typedef.h"

namespace songtzu {
    std::string build_topic(int _topic){
        char topic_cstr[TOPIC_LENGTH+1] = {'\0'} ;
        sprintf( topic_cstr, TOPIC_MACRO, _topic);
        std::string topic = topic_cstr;
        return topic ;
        //return std::to_string(_topic);
    }


    int convert_str_to_topics(std::string _topic, std::vector<Topic_t> &_vec ){
        std::vector<Topic_t> str_vector ;
        songtzu::split(_topic, ',', str_vector);
        for(std::vector<Topic_t>::iterator it= str_vector.begin(); it!=str_vector.end(); ++it){
            char topic_cstr[TOPIC_LENGTH+1] = {'\0'} ;
            sprintf( topic_cstr, TOPIC_MACRO, atoi(it->c_str()));
            _vec.push_back(topic_cstr);
        }
        return 0;
    }
}

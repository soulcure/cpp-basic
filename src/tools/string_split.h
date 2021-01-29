#ifndef STRING_SPLIT_H
#define STRING_SPLIT_H

#include <string>
#include <sstream>
#include <vector>


/***************************************************************
 * split a string by a char.
 * usage:
 *      std::vector<std::string> x = split("one:two::three", ':');
 * author@xiesong
 * songtzu@126.com
 * time@4.20.2016.
 *
 *
 */
namespace songtzu {

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) ;


    std::vector<std::string> split(const std::string &s, char delim) ;


    void str_trim(std::string & _str);

}

#endif // STRING_SPLIT_H

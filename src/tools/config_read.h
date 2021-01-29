#ifndef _H_CONFIGREAD_H
#define _H_CONFIGREAD_H

#include <string>
#include <map>


/**
 * author@xiesong.4.11.2016
 * ---------
 *       [new]
 *       name=xiesong
 * ---------
 *	key:"name"
 *	section:"new"
 */

class ConfigRead{
public:
	ConfigRead();
    ConfigRead(std::string _filename);
    ~ConfigRead(){}

	/**
	 * default is 0.
	 */
    int readInt(std::string _section,
                std::string _key);
				
	/**
	 * default is emtpy.
	 */
    std::string readString(std::string _section,
					std::string _key);
	
    void SetFilePath(std::string _path);
	
private:
    std::string configpath_ ;
    std::map<std::string , std::string> configmap_ ;
	
    void ReadContent();
	
    void trim(std::string & _str){
        _str.erase(_str.find_last_not_of(" \n\r\t")+1);
    }



};

#endif

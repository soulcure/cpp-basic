

#include "config_read.h"


#include <string.h>


#include <stdio.h>
#include <iostream>

#include <fstream>

#include <cstdlib>
#include <ctype.h>
#ifdef WIN32
#include <Windows.h>
#include <stdio.h>
#else

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#endif


using namespace std;


//#,' ',
//section_key value

/**
 *database=ip,port
 * [database]
 * ip=
 * port=
 *
 */
void ConfigRead::ReadContent(){
	/*
	std::ifstream t( configpath );
	std::stringstream buffer;
	buffer << t.rdbuf();
    content = buffer.str(); */
    std::ifstream file( configpath_ );
    string temp;
    string loop_section = "";
    string key ="";
	while(std::getline(file, temp)) {

        for ( string::iterator it=temp.begin(); it!=temp.end(); ++it){

            if(' '==*it){
                continue;
            }else if('#'==*it){
                if(!key.empty())
                    configmap_.insert(std::pair< string , string >(loop_section+"_"+key,temp.substr(it-temp.begin(),string::npos )));
                break;
            }else if('['==*it){
                std::size_t found = temp.find_first_of("]", it - temp.begin());
                 if (found!= string::npos){
                    int beginindex = it-temp.begin() + 1;
                    loop_section = temp.substr(beginindex, found- beginindex );
//                    cout << "loop_section:" << loop_section <<endl ;
                 }
                 break ;
            }else if('='==*it){
                // mymap.insert ( std::pair<char,int>('a',100) );
                if(!key.empty()){
                    string v = temp.substr(it-temp.begin() + 1,string::npos ) ;
                    configmap_.insert(std::pair<string , string >(loop_section+"_"+key, v));
//                    cout  << key <<":" << v << endl;
                    key = "";
                    break ;
                }

            }
            else{
                key.append(1, *it);
            }

        }

	}
}

ConfigRead::ConfigRead(){
    configpath_ = "config.ini";
    ReadContent();
}
ConfigRead::ConfigRead(std::string _filename){
    configpath_ = _filename ;
    ReadContent();
	
}

/**
 * default is 0.
 */
int ConfigRead::readInt(std::string _section,
            std::string _key){
    string compositekey = _section + "_" + _key ;
    std::map<string, string>::iterator it = configmap_.find(compositekey) ;
    if( configmap_.end() != it ){
        string val =it->second ;
        trim(val);
         return std::stoi( val );
    }
    return 0;
}
			
/**
 * default is emtpy.
 */
std::string ConfigRead::readString(std::string _section,
                std::string _key){
    string compositekey = _section + "_" + _key ;
    std::map<string, string>::iterator it = configmap_.find(compositekey) ;
    if( configmap_.end() != it ){
        string val =it->second ;
        trim(val);
        return val;
    }

    return "";
}
	
void ConfigRead::SetFilePath(std::string _path){
    configmap_.clear();
    configpath_ = _path ;
    ReadContent();
}




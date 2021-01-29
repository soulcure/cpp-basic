#@songtzu
#songtzu@126.com
#4.11.2016
PROJECT(service_php_proxy)
set (CMAKE_CXX_STANDARD 11)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


CMAKE_MINIMUM_REQUIRED(VERSION 2.8)



#proto_src
FILE(GLOB proto_src "../../proto/*.proto")

INCLUDE_DIRECTORIES(".")
FILE(GLOB root "./*.cc")
#tools dir
INCLUDE_DIRECTORIES("../tools/")
FILE(GLOB tools "../tools/*.c*")
#threadpool dir
INCLUDE_DIRECTORIES("../threadpool/")
FILE(GLOB threadpool "../threadpool/*.c*")

#proto
INCLUDE_DIRECTORIES("../proto")
FILE(GLOB proto "../proto/*.c*")

#net
INCLUDE_DIRECTORIES("../net/tcp/server")
FILE( GLOB net "../net/tcp/server/*.c*" )

#tcpclient
INCLUDE_DIRECTORIES("../net/tcp/client/")
FILE(GLOB tcpclient "../net/tcp/client/*.c*")

#netbase
INCLUDE_DIRECTORIES("../net/")
FILE(GLOB pdu "../net/*.c*")

#NANO MSG
INCLUDE_DIRECTORIES("../nanomsg")

#msg pub&sub
INCLUDE_DIRECTORIES("../zmq/")
FILE(GLOB subpub "../zmq/*.c*")
#zmq
INCLUDE_DIRECTORIES("../zmq/include/")
 
LINK_DIRECTORIES("/usr/local/lib/")

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE(service_php_proxy ${DIR_SRCS} ${root}  ${proto_src} ${pdu} ${tcpclient} ${tools} ${threadpool} ${proto} ${net} ${subpub})

TARGET_LINK_LIBRARIES(service_php_proxy  -lpthread protobuf zmq)

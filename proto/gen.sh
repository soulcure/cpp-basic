#!/bin/sh

#生成pb对象的脚本
#

SRC_DIR=./
DST_DIR=./gen

#C++
mkdir -p $DST_DIR/cpp
protoc -I=$SRC_DIR --cpp_out=$DST_DIR/cpp/ $SRC_DIR/*.proto

#JAVA
mkdir -p $DST_DIR/java
protoc -I=$SRC_DIR --java_out=$DST_DIR/java/ $SRC_DIR/*.proto

#PYTHON
mkdir -p $DST_DIR/python
protoc -I=$SRC_DIR --python_out=$DST_DIR/python/ $SRC_DIR/*.proto

#php version
#the php script should fork from my own repo.
#since I fix some bug for php compiler.
#https://github.com/rotencode/php-protobuf
mkdir -p $DST_DIR/php
files=`find ./ -name "*.proto"`
for file in $files
do
    echo $file
    php ./php/protoc-php.php -t $DST_DIR/php $file
done


if [ "${1}" = "cp" ];
then
    cp $DST_DIR/cpp/* ../src/proto/
    #cp ../cpp/* ../src/proto/
    echo "heloo"
fi

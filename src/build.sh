#!/bin/sh


buildclean()
{
    echo "we got info ${0}, ${1}, ${2}"
    if [ "${1}" = "yes" ];
    then
        echo "remove old cmake cache"
        rm ./CMakeFiles/ -rf
        rm ./CMakeCache.txt
    else
        echo "no"
    fi
    if [ "${2}" = "yes" ];
    then
        cmake .
        make
        echo "build up"
    else
        echo "no"
    fi

}

build="no"
clean="no"


echo "OPTIND starts at $OPTIND"
while getopts ":bc" optname
do
    case "$optname" in
    "b")
        echo "Option $optname is specified"
        build="yes"
        ;;
    "c")
        echo "Option $optname has value $OPTARG"
        clean="yes"
        ;;
    "?")
        echo "Unknown option $OPTARG"
        ;;
    ":")
        echo "No argument value for option $OPTARG"
        ;;
    *)
        # Should not occur
        echo "Unknown error while processing options"
        ;;
    esac
    echo "OPTIND is now $OPTIND"
done

echo "${build}, ${clean}"


#gnome-terminal -t "title-name" -x bash -c "sh ./run.sh;exec bash;"
cd ./service_mq/
buildclean "${clean}" "${build}"

gnome-terminal -x bash -c "echo service_mq;./service_mq;exec bash;"
echo "service_mq"
sleep 1s


cd ../
cd ./service_route/
buildclean "${clean}" "${build}"
 
gnome-terminal  -x bash -c "echo service_route;./service_route;exec bash;"
echo "service_route"
sleep 1s



cd ../
cd ./service_logic/
buildclean "${clean}" "${build}" 
gnome-terminal -x bash -c "echo service_logic;./service_logic;exec bash;"
echo "service_logic"


cd ../
cd ./service_com/
buildclean "${clean}" "${build}" 
gnome-terminal -x bash -c "echo service_com;./service_com;exec bash;"
echo "service_com"

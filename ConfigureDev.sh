#/bin/bash


mkdir build;
cd build;
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DCHIP8_BUILD_APP=ON ..;


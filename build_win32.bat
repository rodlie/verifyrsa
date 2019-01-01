@echo off
rd /s /q build
mkdir build
cd build
cmake -DOPENSSL_ROOT_DIR=openssl -G"Visual Studio 12 2013" ..
cmake --build . --config Release
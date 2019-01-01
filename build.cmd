mkdir win32
cd win32
cmake -DOPENSSL_ROOT_DIR=openssl -G"Visual Studio 12 2013" ..
cmake --build . --config Release

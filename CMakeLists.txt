cmake_minimum_required(VERSION 2.8)

project(VerifyRSA)

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
set(LIB_TYPE STATIC)
if(BUILD_SHARED_LIBS)
set(LIB_TYPE SHARED)
endif(BUILD_SHARED_LIBS)

if(COMPILER_MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

add_library(VerifyRSA ${LIB_TYPE} verifyrsa.cpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(VerifyRSA OpenSSL::SSL)
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER( VerifyRSA
    BASE_NAME VerifyRSA
    EXPORT_MACRO_NAME VerifyRSA_EXPORT
    EXPORT_FILE_NAME VerifyRSA_Export.h
    STATIC_DEFINE VerifyRSA_BUILT_AS_STATIC
)

add_executable(rsa-sign rsa-sign.cpp)
target_link_libraries(rsa-sign VerifyRSA)
add_executable(rsa-verify rsa-verify.cpp)
target_link_libraries(rsa-verify VerifyRSA)


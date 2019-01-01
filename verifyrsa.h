#ifndef VERIFYRSA_H
#define VERIFYRSA_H

#if defined (_WIN32)
#if defined(VerifyRSA_EXPORTS)
#define MYLIB_EXPORT __declspec(dllexport)
#else
#define MYLIB_EXPORT __declspec(dllimport)
#endif
#else
#define MYLIB_EXPORT
#endif

#include <iostream>

class MYLIB_EXPORT VerifyRSA
{
public:
    VerifyRSA();
    char* sign(std::string privateKey, std::string plainText);
    bool verify(std::string publicKey, std::string plainText, char* signatureBase64);
};

#endif // VERIFYRSA_H

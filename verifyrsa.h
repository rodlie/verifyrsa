// https://github.com/rodlie/verifyrsa
// BSD 3-Clause License

#ifndef VERIFYRSA_H
#define VERIFYRSA_H

#if defined(EXPORT_DLL)
#if defined(_WIN32)
#if defined(VerifyRSA_EXPORTS)
#define MYLIB_EXPORT __declspec(dllexport)
#else
#define MYLIB_EXPORT __declspec(dllimport)
#endif
#else
#define MYLIB_EXPORT
#endif
#else
#define MYLIB_EXPORT
#endif

#include <iostream>

class VerifyRSA
{
public:
    static MYLIB_EXPORT char* sign(std::string privateKey,
                                   std::string plainText);
    static MYLIB_EXPORT bool verify(std::string publicKey,
                                    std::string plainText,
                                    char* signatureBase64);
    static MYLIB_EXPORT bool generate(std::string key,
                                      int bits = 2048);
};

#endif // VERIFYRSA_H

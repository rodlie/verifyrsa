// https://github.com/rodlie/verifyrsa
// BSD 3-Clause License

#include "verifyrsa.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc<3) {
        std::cout << "VerifyRSA - https://github.com/rodlie/verifyrsa" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Usage: rsa-verify <public key> <text file>" << std::endl;
        return 0;
    }

    std::string key, plain, asc;
    std::ifstream keyFile, textFile, ascFile;
    keyFile.open(argv[1]);
    if (keyFile.is_open()) {
        std::stringstream ss;
        ss << keyFile.rdbuf();
        key = ss.str();
        keyFile.close();
    }
    textFile.open(argv[2]);
    if (textFile.is_open()) {
        std::stringstream ss;
        ss << textFile.rdbuf();
        plain = ss.str();
        textFile.close();
    }
    std::string ascFilename = argv[2];
    ascFilename.append(".asc");
    ascFile.open(ascFilename.c_str());
    if (ascFile.is_open()) {
        std::stringstream ss;
        ss << ascFile.rdbuf();
        asc = ss.str();
        ascFile.close();
    }

    if (key.empty() || plain.empty() || asc.empty()) {
        std::cerr << "Missing key, text or asc." << std::endl;
        return 1;
    }
    
    std::vector<char> cstr(asc.c_str(), asc.c_str() + asc.size() + 1);
    if (!VerifyRSA::verify(key, plain, cstr.data())) {
        std::cerr << "Bad signature!" << std::endl;
        return 1;
    }
    std::cout << "Valid signature!" << std::endl;
    return 0;
}

// https://github.com/rodlie/verifyrsa
// BSD 3-Clause License

#include "verifyrsa.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc<3) {
        std::cout << "VerifyRSA - https://github.com/rodlie/verifyrsa" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Usage: rsa-sign <private key> <text file>" << std::endl;
        return 0;
    }

    std::string key, plain;
    std::ifstream keyFile, textFile;
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

    if (key.empty() || plain.empty()) {
        std::cerr << "Missing key or text." << std::endl;
        return 1;
    }

    char* signature = VerifyRSA::sign(key, plain);
    if (sizeof(signature)<1) {
        std::cerr << "Unable to sign file!" << std::endl;
        return 1;
    }

    std::ofstream ascFile;
    std::string filename = argv[2];
    filename.append(".asc");
    std::ofstream os(filename.c_str());
    if (!os) {
        std::cerr << "Failed to write signature!" << std::endl;
        return 1;
    }
    else { os << signature; }
    return 0;
}

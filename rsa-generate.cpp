// https://github.com/rodlie/verifyrsa
// BSD 3-Clause License

#include "verifyrsa.h"
#include <string>
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc<2) {
        std::cout << "verifyrsa - https://github.com/rodlie/verifyrsa" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Usage: rsa-generate <key suffix> <bits>" << std::endl;
        return 0;
    }
    int bits = argc>2?atoi(argv[2]):2048;
    if (!VerifyRSA::generate(argv[1], bits)) {
        std::cerr << "Unable to generate keys!" << std::endl;
        return 1;
    }
    return 0;
}

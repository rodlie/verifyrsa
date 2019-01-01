// https://github.com/rodlie/verifyrsa
// BSD 3-Clause License

#include "verifyrsa.h"
#include <string>
#include <iostream>
#include <stdlib.h>

#ifdef WITH_FLTK
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>
#endif

int main(int argc, char* argv[]) {
    if (argc<2) {
        std::cout << "verifyrsa - https://github.com/rodlie/verifyrsa" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "Usage: rsa-generate <key suffix> <bits>" << std::endl;
#ifdef WITH_FLTK
        Fl_Native_File_Chooser fc;
        fc.title("Save RSA keys to ...");
        fc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
        std::string filename;
        switch (fc.show()) {
        case -1:
            fl_alert("%s",fc.errmsg());
            return 1;
        default:
            filename = fc.filename();
        }
        if (filename.empty()) { return 1; }
        if (!VerifyRSA::generate(filename)) {
            fl_alert("%s", "Unable to generate keys!");
            return 1;
        }
#endif
        return 0;
    }
    int bits = argc>2?atoi(argv[2]):2048;
    if (!VerifyRSA::generate(argv[1], bits)) {
        std::cerr << "Unable to generate keys!" << std::endl;
        return 1;
    }
    return 0;
}

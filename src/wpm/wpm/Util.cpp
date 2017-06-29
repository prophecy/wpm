//
//  Util.cpp
//  wpm
//
//  Created by Adawat Chanchua on 6/29/2560 BE.
//  Copyright © 2560 Adawat Chanchua. All rights reserved.
//

#include "Util.hpp"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

std::string execute(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

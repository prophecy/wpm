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
#include <map>

#include "rapidjson.h"
#include "document.h"

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// High-level APIs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

std::string getJsonString(rapidjson::Document& document, std::string key) {
    
    if (document.HasMember(key.c_str())) {
        
        std::string value = document[key.c_str()].GetString();
        std::cout << key << ": " << value << std::endl;
        
        return value;
    }
    
    return "";
}

void createDependenciesMap(rapidjson::Value& dep, std::map<std::string, std::string>& map) {
    
    rapidjson::Value::MemberIterator depIt;
    for (depIt = dep.MemberBegin(); depIt != dep.MemberEnd(); ++depIt) {
        
        std::string key = depIt->name.GetString();
        std::string value = depIt->value.GetString();
        
        std::cout << "Key: " << key << " Value: " << value << std::endl;
        
        map.insert(std::pair<std::string, std::string>(key, value));
    }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Low-level APIs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

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

std::string execute(std::string cmd) {
    
    return execute(cmd.c_str());
}

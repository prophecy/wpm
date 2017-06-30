//
//  Util.hpp
//  wpm
//
//  Created by Adawat Chanchua on 6/29/2560 BE.
//  Copyright © 2560 Adawat Chanchua. All rights reserved.
//

#ifndef Util_hpp
#define Util_hpp

#include <map>
#include <string>

#include "rapidjson.h"
#include "document.h"

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// High-level APIs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

std::string getJsonString(rapidjson::Document& document, std::string key);
void createDependenciesMap(rapidjson::Value& dep, std::map<std::string, std::string>& map);

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Low-level APIs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// OS command execution wrapper
std::string execute(const char* cmd);
std::string execute(std::string cmd);

#endif /* Util_hpp */

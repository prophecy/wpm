//
//  main.cpp
//  wPackage
//
//  Created by Adawat Chanchua on 7/3/2560 BE.
//  Copyright © 2560 Adawat Chanchua. All rights reserved.
//

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Lib in the Wonderland
#include "Util.hpp"

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// C/C++ lib

#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Rapid JSON lib

#include "rapidjson.h"
#include "document.h"

using namespace rapidjson;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Function declaration

void executeCommand(Value& commandCollection, string packageName, string projectPath);

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Constants
const string VERSION = "0.0.0.2";
const string CONF = "conf.json";
const string WONDER_MODULE = "wonder_modules";

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int main(int argc, const char * argv[]) {
    
    /***********************************************
     Params order note (From argc, argv convention):
     [0] File path
     [1] Package path
     [1] wonder_config
     ***********************************************/
    
    // Check package name existence
    if (argc <= 1) {
        
        cout << "package name NOT found!" << endl;
        return -1;
    }
    
    string packageName = argv[1];
    
    // Check wonder_config existence
    if (argc <= 2) {
        
        cout << "wonderconf NOT found!" << endl;
        return -1;
    }
    
    // Parse wonderconf to JSON document
    string wonderConfPath = argv[2];
    
    // Check is wonderconf exist
    string wonderconfLs = execute(string("ls ") + wonderConfPath);
    
    // Conf NOT exist
    if (wonderconfLs.empty()) {
        
        cout << "wonderconf.json is NOT exist!" << endl;
        return -1;
    }
    
    // Read wonderconf
    string wonderConfStr = execute(string("cat ") + string("./") + wonderConfPath);
    
    Document wonderConf;
    wonderConf.Parse<0>(wonderConfStr.c_str());
    
    // Get dependencies object
    if (!wonderConf.HasMember("dependencies")) {
        
        cout << "wonderconf does NOT have dependencies!" << endl;
        return -1;
    }
    
    Value& dependencies = wonderConf["dependencies"];
    
    // Check is conf exist
    string confPath = string("./") + string(WONDER_MODULE) + string("/") + packageName + string("/") + string(CONF);
    
    string confLs = execute(string("ls ") + confPath);
    
    // Conf NOT exist
    if (confLs.empty()) {
        
        cout << "conf.json is NOT exist!" << endl;
        return -1;
    }
    
    // Read conf
    string confStr = execute(string("cat ") + confPath);
    
    // Parse conf to JSON document
    Document conf;
    conf.Parse(confStr.c_str());
    
    // Execute command
    rapidjson::Value::MemberIterator depIt;
    for (depIt = dependencies.MemberBegin(); depIt != dependencies.MemberEnd(); ++depIt) {
        
        // Get OS name
        string osName = depIt->name.GetString();
        Value& osConf = dependencies[osName.c_str()];
        
        if (!osConf.HasMember("projectPath")) {
            
            cout << "projectPath does NOT specified for " << osName << endl;
            continue;
        }
        
        string projectPath = osConf["projectPath"].GetString();
        
        // Get command from OS name
        Value& commands = conf[osName.c_str()];
        
        // Execute commands
        for (SizeType i=0; i < commands.Size(); ++i) {
            
            executeCommand(commands[i], packageName, projectPath);
        }
    }
    
    return 0;
}

void executeCommand(Value& commandCollection, string packageName, string projectPath) {
    
    string command = commandCollection["command"].GetString();
    
    if (command.compare("copy") == 0) {
        
        string src = commandCollection["src"].GetString();
        string dst = commandCollection["dst"].GetString();
        
        string srcAbs = string("./") + string(WONDER_MODULE) + string("/") + packageName + string("/") + src;
        string dstAbs = projectPath + dst;
        
        string cmd = string("cp -Rf ") + srcAbs + string(" ") + dstAbs;
        cout << "cmd: " << cmd << endl;
        string exeResult = execute(cmd);
        
        cout << exeResult << endl;
    }
    else if (command.compare("exec") == 0) {
        
        string parm = commandCollection["parm"].GetString();
        
        string exeResult = execute(parm);
        
        cout << exeResult << endl;
    }
}

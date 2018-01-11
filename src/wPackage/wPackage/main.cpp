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

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <fstream>

using namespace std;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Rapid JSON lib

#include "rapidjson.h"
#include "document.h"

using namespace rapidjson;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Function declaration

void executeCommand(Value& commandCollection, string packageName, string projectPath, string subCommand);

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
     [1] Package name
     [2] wonder_config
     [3] subCommand
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
    
    // Parse sub command (Optional)
    string subCommand;
    
    if (argc > 3) {
     
        subCommand = argv[3];
    }
    
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
        
        // Get platform name
        string platformName = depIt->name.GetString();
        Value& platformConf = dependencies[platformName.c_str()];
        
        if (!platformConf.HasMember("projectPath")) {
            
            cout << "projectPath does NOT specified for " << platformName << endl;
            continue;
        }
        
        string projectPath = platformConf["projectPath"].GetString();
        
        // Get command from platform name
        if (!conf.HasMember(platformName.c_str())) {
            
            cout << platformName << " does NOT exist." << endl;
            continue;
        }
        
        Value& commands = conf[platformName.c_str()];
        
        // Execute commands
        for (SizeType i=0; i < commands.Size(); ++i) {
            
            executeCommand(commands[i], packageName, projectPath, subCommand);
        }
    }
    
    return 0;
}

void executeCommand(Value& commandCollection, string packageName, string projectPath, string subCommand) {
    
    string command = commandCollection["command"].GetString();
    
    // Specify mode
    const int MODE_NORMAL = 0;
    const int MODE_REVERSE_COPY = 1;
    
    int operMode = MODE_NORMAL;
    
    if (subCommand.compare("-reverse_copy") == 0)
        operMode = MODE_REVERSE_COPY;
    
    bool isCopyCommand = command.compare("copy") == 0;
    
    if (command.compare("copy") == 0 &&
        (operMode == MODE_NORMAL || operMode == MODE_REVERSE_COPY)) {
    
        string src = commandCollection["src"].GetString();
        string dst = commandCollection["dst"].GetString();
        
        // Set absolute paths by mode
        string srcAbs;
        string dstAbs;
        
        switch (operMode) {
            case MODE_REVERSE_COPY:
                
                srcAbs = projectPath + dst;
                dstAbs = string("./") + string(WONDER_MODULE) + string("/") + packageName + string("/") + src;
                break;
            case MODE_NORMAL:
            default:
                
                srcAbs = string("./") + string(WONDER_MODULE) + string("/") + packageName + string("/") + src;
                dstAbs = projectPath + dst;
                break;
        }
        
        string cmd = string("cp -Rf ") + srcAbs + string(" ") + dstAbs;
        cout << "cmd: " << cmd << endl;
        string exeResult = execute(cmd);
        
        cout << exeResult << endl;
    }
    else if (command.compare("exec") == 0 && operMode == MODE_NORMAL) {
        
        string parm = commandCollection["parm"].GetString();
        
        string exeResult = execute(parm);
        
        cout << exeResult << endl;
    }
    else if (command.compare("settings.gradle") == 0) {
        
        if (commandCollection.HasMember("upsert")) {
            
            // Supported settings.gradle format
            //   include ':app', ':wondertoolkit'
            
            // Vars
            string insertModule = commandCollection["upsert"].GetString();
            string relativePath = "";
            
            if (commandCollection.HasMember("relative_path"))
                relativePath = commandCollection["relative_path"].GetString();
            
            // Read settings.gradle file
            string settingsGradlePath = projectPath + relativePath + string("settings.gradle");
            
            cout << "settings.gradle path: " << settingsGradlePath << endl;
            
            string settingsGradleStr = execute(string("cat ") + string("./") + settingsGradlePath);
            
            // Gradle header
            vector<char>header;
            
            // Get all module (As token)
            vector<string> modules;
            vector<char> tmpModule;
            
            enum STATE {
                STATE_SEEK_HEAD = 0,
                STATE_SEEK,
                STATE_READ,
                STATE_END,
            };
            
            int currState = STATE_SEEK_HEAD;
            
            // Read character 1 by 1, to get all modules (The characters between single quote)
            for (int i=0; i<settingsGradleStr.length(); ++i) {
                
                char curr = settingsGradleStr[i];
                
                if (currState == STATE_SEEK_HEAD) {
                    
                    if (curr == '\'')
                        currState = STATE_READ;
                    else if (curr != ' ')
                        header.push_back(curr);
                }
                else if (currState == STATE_SEEK) {
                    
                    if (curr == '\'')
                        currState = STATE_READ;
                }
                else if (currState == STATE_READ) {
                
                    if (curr == '\'')
                        currState = STATE_END;
                    else if (curr != ':') {
                        
                        tmpModule.push_back(curr);
                    }
                }
                
                if (currState == STATE_END) {
                    
                    // Convert vector to string
                    std::string tStr(tmpModule.begin(), tmpModule.end());
                    
                    // Add token
                    modules.push_back(tStr);
                    
                    // Clear tmp
                    tmpModule.clear();
                    
                    // Go seek
                    currState = STATE_SEEK;
                }
            }
            
            // Token matching
            bool isInsert = true;
            for (int i=0; i<modules.size(); ++i) {
                
                string currModule = modules[i];
                
                // If the given module is matched =>
                //     Do not insert & break matching
                if (currModule.compare(insertModule) == 0) {
                
                    isInsert = false;
                    break;
                }
            }
            
            // Check to insert module
            if (isInsert) {
                
                // Convert header to string
                std::string headerStr(header.begin(), header.end());
                
                // Out string
                string outStr = headerStr + string(" ");
                
                modules.push_back(insertModule);
                
                // Convert module to file
                for (int i=0; i<modules.size(); ++i) {
                    
                    string currModule = modules[i];
                    
                    outStr = outStr + string("':") + currModule + string("'");
                    
                    if (i < modules.size() - 1)
                        outStr = outStr + string(", ");
                }
                
                // Remove file
                remove(settingsGradlePath.c_str());
                
                // Create file again
                ofstream gradleFile;
                gradleFile.open (settingsGradlePath);
                
                if (gradleFile.is_open()) {
                
                    gradleFile << outStr;
                    gradleFile.close();
                }
            }
        }
    }
}

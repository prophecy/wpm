//
//  main.cpp
//  wpm
//
//  Created by Adawat Chanchua on 6/29/2560 BE.
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

string view(int argc, const char* argv[]);
string install(int argc, const char* argv[]);
string uninstall(int argc, const char* argv[]);
string update(int argc, const char* argv[]);
string npm(int argc, const char* argv[]);
string exec(int argc, const char* argv[]);
string help(int argc, const char* argv[]);
string version(int argc, const char* argv[]);
string run(int argc, const char* argv[]);

void showWelcomeMessage();

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Constants
const string VERSION = "0.0.2.0";
const string WONDER_MODULE = "wonder_modules";
const string WONDER_TEMP_MODULE = "wonder_modules/tmp";
const string WONDER_CONF = "wonderconf.json";

const string MODE_LOCAL = "MODE_LOCAL";
const string MODE_GLOBAL = "MODE_GLOBAL";

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Shared vars

map<string, void*> commandMap;
map<string, string> descMap;
string mode;

void addCommand(string name, void* commandFunctionPointer, string desc) {
    
    commandMap.insert(pair<string, void*>(name, commandFunctionPointer));
    descMap.insert(pair<string, string>(name, desc));
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int main(int argc, const char * argv[]) {
    
    /***********************************************
     Params order note (From argc, argv convention):
        [0] File path
        [1] command
        [2][And more] command params
     ***********************************************/
    
    // Set mode from command
    if (string(argv[0]).compare("wpm") == 0)
        mode = MODE_GLOBAL;
    else
        mode = MODE_LOCAL;
    
    // Show info if hitting wpm without any command
    if (argc <= 1) {
    
        showWelcomeMessage();
        return 0;
    }
    
    // Map all command
    addCommand("view",      (void*)&view,       "View package info");
    addCommand("install",   (void*)&install,
               string("Install package") +
               string("\n\t -clean: Remove repository before downloading it."));
    addCommand("uninstall", (void*)&uninstall,  "Uninstall package");
    addCommand("update",    (void*)&update,     "Update package");
    addCommand("npm",       (void*)&npm,        "Call npm");
    addCommand("exec",      (void*)&exec,       "Execute OS command");
    addCommand("help",      (void*)&help,       "Show helpful info");
    addCommand("version",   (void*)&version,    "Show wpm version");
    addCommand("run",       (void*)&run,
               string("Execute command(s) inside the package") +
               string("\n\t -reverse_copy: Execute reverse copy") +
               string("\n\t The list below is conf.json's parameters:") +
               string("\n\t\t copy: Copy file/directory") +
               string("\n\t\t\t src: Source location") +
               string("\n\t\t\t dst: Destination location") +
               string("\n\t\t exec: Execute OS command\n") +
               string("\n\t\t\t parm: The command to be executed") +
               string("\n\t\t settings.gradle: Modify settings.gradle (For Android)") +
               string("\n\t\t\t insert: Insert module")
               );
    
    string command = argv[1];
    string result = "";
    
    if (commandMap[command]) {
        
        void* f = commandMap[command];
        result = ((string(*)(int, const char* []))f)(argc, argv);
    }
    else {
        
        result = string("-wpm: ") + command + string(": command not found");
    }
    
    // Print out result
    cout << result << endl;
    
    return 0;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Function implementation

string view(int argc, const char* argv[]) {
    
    cout << "To view something" << endl;
    return "";
}

string install(int argc, const char* argv[]) {
    
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // Execute sub command
    if (argc > 2) {
        
        string subCommand = argv[2];
        
        // Clean if need
        if (subCommand.compare("-clean") == 0) {
            
            execute(string("rm -rf " + string(WONDER_MODULE)));
        }
    }
    
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // Create wonder_modules directory if NOT exist
    execute(string("mkdir -p ") + string(WONDER_MODULE));
    
    // Read wonderconf.json
    string wonderConfStr = execute(string("cat ") + string(WONDER_CONF));
    
    // Parse to JSON document
    Document wonderConf;
    wonderConf.Parse<0>(wonderConfStr.c_str());
    
    string name = getJsonString(wonderConf, "name");
    string description = getJsonString(wonderConf, "description");
    string host = getJsonString(wonderConf, "host");
    string version = getJsonString(wonderConf, "version");
    
    // Get dependency objects
    Value& dependencies = wonderConf["dependencies"];
    
    rapidjson::Value::MemberIterator depIt;
    for (depIt = dependencies.MemberBegin(); depIt != dependencies.MemberEnd(); ++depIt) {
        
        // Get OS name
        std::string osName = depIt->name.GetString();
    
        cout << "osName: " << osName << endl;
        
        // Get OS object
        Value& osDep = dependencies[osName.c_str()];
        
        // Parse to map
        map<string, string> osDepMap;
        createDependenciesMap(osDep, osDepMap);
        
        // Download dependency
        typedef std::map<std::string, std::string>::iterator it_type;
        for(it_type iterator = osDepMap.begin(); iterator != osDepMap.end(); iterator++) {
            
            string name = iterator->first;
            string version = iterator->second;
            
            // Check is module exist
            string existantResult = execute("ls " + string(WONDER_MODULE) + string("/") + name);
            
            cout << "Existant Result: " << existantResult << endl;
            
            // Directory exist
            if (!existantResult.empty()) {
             
                cout << existantResult << " is exist!" << endl;
                continue;
            }
            
            // Validate URL
            string checkCommand = string("curl -o - -s -w \"%{http_code}\n\" " +
                                         host + name + string(".zip"));
            string checkResult = execute(checkCommand);
            
            // Do not download if invalid
            if (checkResult.compare("404") == 0 || checkResult.compare("404\n") == 0) {

                cout << name << " not found!" << endl;
            }
            else {
                
                // Create tmp dir
                execute(string("mkdir -p ") + string(WONDER_TEMP_MODULE));
                
                // Start download
                string downloadCommand = string("curl ") + host + name + string(".zip") +
                 " --output ./" + WONDER_TEMP_MODULE + "/" + name + ".zip";
                 
                cout << "Download cmd: " << downloadCommand << endl;
                 
                execute(downloadCommand);
                
                // Extract
                string zipSrc = string("./") + string(WONDER_TEMP_MODULE) +
                                    string("/") + name + string(".zip");
                string zipDst = string("./") + string(WONDER_TEMP_MODULE) +
                                    string("/") + name;
                string unzipCommand = string("unzip ") + zipSrc + string(" -d ") + zipDst;
                
                execute(unzipCommand);
                
                // Move
                string moduleDst = string("./") + string(WONDER_MODULE) + string("/") + name;
                execute("mv " + zipDst + " " + moduleDst);
                
                // Delete tmp dir
                execute("rm -rf " + string(WONDER_TEMP_MODULE));
            }
        }
    }
    
    return "Success";
}

string uninstall(int argc, const char* argv[]) {
    
    cout << "To uninstall something" << endl;
    return "";
}

string update(int argc, const char* argv[]) {
    
    cout << "To update something" << endl;
    return "";
}

string npm(int argc, const char* argv[]) {
    
    stringstream ss;
    
    for (int i=1; i<argc; ++i) {
    
        ss << argv[i];
        
        if (i < argc -1)
            ss << " ";
    }
    
    cout << "Execute npm with command: " << ss.str().c_str() << endl;
    
    return execute(ss.str().c_str());
}

string exec(int argc, const char* argv[]) {
    
    stringstream ss;
    
    for (int i=2; i<argc; ++i) {
        
        ss << argv[i];
        
        if (i < argc -1)
            ss << " ";
    }
    
    cout << "Execute OS with command: " << ss.str().c_str() << endl;
    
    return execute(ss.str().c_str());
}

string help(int argc, const char* argv[]) {
 
    stringstream ss;
    
    ss << "Usage: wpm <command>" << endl;
    
    typedef std::map<string, void*>::iterator it_type;
    for(it_type iterator = commandMap.begin(); iterator != commandMap.end(); iterator++) {
        
        string key = iterator->first;
        
        ss << key << "\t\t" << descMap[key] << endl;
    }
    
    return ss.str().c_str();
}

string version(int argc, const char* argv[]) {
    
    return VERSION;
}

string run(int argc, const char* argv[]) {
    
    int curArgIndex = 2;
    
    // Validate package name
    if (argc <= curArgIndex)
        return "Package name NOT found!";
    
    string packageName = argv[curArgIndex];
    
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // Check sub command
    string subCommand;
    subCommand.clear();
    
    if (argc > ++curArgIndex) {
        
        string curSubCommand = argv[curArgIndex];
        
        cout << "curSubCommand: " << curSubCommand << endl;
        
        // Reverse command
        if (curSubCommand.compare("-reverse_copy") == 0)
            subCommand = curSubCommand;
    }
    
    // Execute package commands
    string wpmPackageInit = "";
    
    // wpmPckageInit is an executable name (For wpmPackage)
    if (mode.compare(MODE_LOCAL) == 0)
        wpmPackageInit = "./wpmPackageInit";
    else if (mode.compare(MODE_GLOBAL) == 0)
        wpmPackageInit = "wpmPackageInit";
    else {
        cout << "wpmPackageInit has error! mode " << mode << " does NOT support!" << endl;
    }
    
    string cmd00 = wpmPackageInit;
    string cmd01Space = " ";
    string cmd01 = packageName + string(" ") + "./" + string(WONDER_CONF);
    string cmd02Space = subCommand.empty() ? string("") : string(" ");
    string cmd02 = subCommand.empty() ? string("") : subCommand;
    
    string executingCommand = cmd00 + cmd01Space + cmd01 + cmd02Space + cmd02;
    
    cout << "run command: " << executingCommand << endl;
    
    return execute(executingCommand);
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void showWelcomeMessage() {
    
    cout << "                                                                                " << endl;
    cout << "                     N                                                     dhh  " << endl;
    cout << "                   Nhsd                                             NdhysssooN  " << endl;
    cout << "                  y+o+d           dhysso++o++++osyh N         N dysoossyyyyod   " << endl;
    cout << "                s++ss/d     Ndyo+oooooooooooo+oooooooosyh dhso+ssssysyyyyyoy    " << endl;
    cout << "             Ns//ooss/d  Nho++ooooooooo+oo+ooooossssssssossosssyyysyyyyyyooN    " << endl;
    cout << "            h++++oo+/s do:/++ooooo+////+ooosssosooooossyyyyyyyyyyyyyysyyso      " << endl;
    cout << "          No/+ooo+/odh+/+++++++//+shdNN          N hysossyyyyyyyyyyyyyys+h      " << endl;
    cout << "          +/+oo+/+dd+/+o++++//oh N                 dNNNdyosyyyyyyyyyyyyss       " << endl;
    cout << "         //+ooo/s s:/++o+/:/y                    yossyhdNNhsoyyyyyyyyyso        " << endl;
    cout << "        +:++++/h +:/++++//hN                   yoosyyyyssh Nhoosyyyyss+d        " << endl;
    cout << "      N+::/osoh +:++++/:sN                   s+osssyyyyyyysy Ns+syyyso+d        " << endl;
    cout << "      h:+///oNN+:/+++/:h                   s++osssyyyyyyhhhysh y+ssssyo/        " << endl;
    cout << "     N//+++:+Ny:///+::d                 Ns//+oooossyyyhhhhhhyoy h/ossys+o       " << endl;
    cout << "     h:+++/: N://///-y                    yyyyhhhhhsshhhhhhhyyoN s+ssyys/d      " << endl;
    cout << "    No/+++:+ y:++//:+                              hshhhhhhyshN   oossss+o      " << endl;
    cout << "  h++/++++-hN//++//-h                              hyhhhhyyy      hosysso/N     " << endl;
    cout << "  s/+o++//:  :/+++/:N                              hyhhyyy         oossso/      " << endl;
    cout << "  s:+o/:shdNd:++++/:N                              hyyyy          Noossso/d     " << endl;
    cout << "  o:+++//// h:+ooo/:N                               sy            Nsossso/      " << endl;
    cout << "  h+//++++:hd:+oooo:d                                              oosso+/N     " << endl;
    cout << "     :/++o/o //ooos+o        NNNN    N                            y+ssoo/o      " << endl;
    cout << "     /:+oo+: y:+ssso/y Nysssoooooo++oN                           d++oooo:d      " << endl;
    cout << "     h:/ooo/+N/+sssso+h y/+ososssss+y                            o+ooo+/o       " << endl;
    cout << "      +/oo++sN /osysso+h d+oyssssss/h                          do+ooo+//N       " << endl;
    cout << "       /+//ossyd/osssssosNNhsssyyys+                         Ny/+oo++//         " << endl;
    cout << "       h/+oooo+yd/+ssyyyssdNNhsssyssN                      Nh+/+oo++/+          " << endl;
    cout << "        y:+ssoo  y+syyyyyyyydNNdysoy                     Nho++ooo++:sN          " << endl;
    cout << "         h/+o+  hoosyyyyyyhhyyhdNN                   NNds++osoo++/+dNNNNNNNNN   " << endl;
    cout << "           ++  hosssyyyhhhhhhhhysyd N          NNNdhys++ooosoo+//y y+++++++ohN  " << endl;
    cout << "           N  d+ossyyyyyyyhhhhhhyssssyyyyhhhyssoo++oossssooo+/oh y/:/+///:o     " << endl;
    cout << "             d+ossssyssssyhyyyyyyyssyyyssssssososooosooo++/oydds//++oo+/o       " << endl;
    cout << "            d++ssooosyd NNd ddhyssssossssssosssooo+++//+shdhs//+oooo++od        " << endl;
    cout << "           d//++sydNN dhyyhhyyNN  ddysoo++++++oooosyhdd  o//++++++//sd          " << endl;
    cout << "           ooydN    dsssyhhhssdssyhhd       Nddhhyso+//:s+:++++//+yN            " << endl;
    cout << "         N            Ndysssssssyyyyysssso+N +oo+++o+o++:/:///+y                " << endl;
    cout << "                          Ndhsoossyyyyssss+ooooooooo++////oydN                  " << endl;
    cout << "                               N dhhysoosss+ooo+/++ooyhdN                       " << endl;
    cout << "                                       /+o++++/:d                               " << endl;
    cout << "                                       N                                        " << endl;
    cout << "Welcome to wpm (Wonder Package Manager)" << endl;
    cout << "wpm is about to make life easier like coding in the wonderland!" << endl << endl;
}

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
// Function declaration

string view(int argc, const char* argv[]);
string install(int argc, const char* argv[]);
string uninstall(int argc, const char* argv[]);
string update(int argc, const char* argv[]);
string npm(int argc, const char* argv[]);
string exec(int argc, const char* argv[]);
string help(int argc, const char* argv[]);
string version(int argc, const char* argv[]);

void showWelcomeMessage();

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Constants
const string VERSION = "0.001";

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Shared vars

map<string, void*> commandMap;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int main(int argc, const char * argv[]) {
    
    /***********************************************
     Params order note (From argc, argv convention):
        [0] File path
        [1] command
        [2][And more] command params
     ***********************************************/
    
    // Show info if hitting wpm without any command
    if (argc <= 1) {
    
        showWelcomeMessage();
        return 0;
    }
    
    // Map all command
    commandMap.insert(pair<string, void*>("view", (void*)&view));
    commandMap.insert(pair<string, void*>("install", (void*)&install));
    commandMap.insert(pair<string, void*>("uninstall", (void*)&uninstall));
    commandMap.insert(pair<string, void*>("update", (void*)&update));
    commandMap.insert(pair<string, void*>("npm", (void*)&npm));
    commandMap.insert(pair<string, void*>("exec", (void*)&exec));
    commandMap.insert(pair<string, void*>("help", (void*)&help));
    commandMap.insert(pair<string, void*>("version", (void*)&version));
    
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
    
    cout << "To install something" << endl;
    return "";
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
        
        ss << key << "\t\tJust a command in wpm" << endl;
    }
    
    return ss.str().c_str();
}

string version(int argc, const char* argv[]) {
    
    return VERSION;
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

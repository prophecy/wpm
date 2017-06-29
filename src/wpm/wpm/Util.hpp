//
//  Util.hpp
//  wpm
//
//  Created by Adawat Chanchua on 6/29/2560 BE.
//  Copyright © 2560 Adawat Chanchua. All rights reserved.
//

#ifndef Util_hpp
#define Util_hpp

#include <string>

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// High-level APIs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// Low-level APIs
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// OS command execution wrapper
std::string execute(const char* cmd);

#endif /* Util_hpp */

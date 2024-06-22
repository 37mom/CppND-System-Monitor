#include <string>
#include <chrono>
#include <sstream>
#include<iomanip>

#include "format.h"

using std::string;
using std::to_string;

// DONE
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    string HH , MM , SS;

    std::chrono::seconds s{seconds};
    std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(s);
    s -= std::chrono::duration_cast<std::chrono::seconds>(h); 
    std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(s);
    s -= std::chrono::duration_cast<std::chrono::seconds>(m); 

    std::stringstream ss{};


    ss << std::setw(2) << std::setfill('0') << h.count()     // HH
     << std::setw(1) << ":"                                    // :
     << std::setw(2) << std::setfill('0') << m.count()   // MM
     << std::setw(1) << ":"                                    // :
     << std::setw(2) << std::setfill('0') << s.count();  // SS

  return ss.str();
}
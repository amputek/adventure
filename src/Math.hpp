//
//  Math.hpp
//  Adventure
//
//  Created by Rob Dawson on 04/05/2016.
//
//
#ifndef Math_hpp
#define Math_hpp

#include <stdio.h>
using namespace ci;
using namespace std;


namespace math {
    extern float distance(vec2 a, vec2 b);
    extern float getAngle(vec2 a, vec2 b);
}

namespace debug {
    extern bool show();
    extern void log( string s );
}


#endif /* Math_hpp */

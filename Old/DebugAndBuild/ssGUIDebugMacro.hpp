#ifndef SSGUI_DEBUG_MACRO
#define SSGUI_CONFIG

#include <string>
#include <iostream>

// https://stackoverflow.com/questions/16683146/can-macros-be-overloaded-by-number-of-arguments
// Overloadable Marco function =======================================================================
#define CAT( A, B ) A ## B
#define SELECT( NAME, NUM ) CAT( NAME ## _, NUM )
#define COMPOSE( NAME, ARGS ) NAME ARGS

#define GET_COUNT( _0, _1, _2, _3, _4, _5, _6 /* ad nauseam */, COUNT, ... ) COUNT
#define EXPAND() ,,,,,, // 6 commas (or 7 empty tokens)
#define VA_SIZE( ... ) COMPOSE( GET_COUNT, (EXPAND __VA_ARGS__ (), 0, 6, 5, 4, 3, 2, 1) )

#define VA_SELECT( NAME, ... ) SELECT( NAME, VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)
// ======================================================================= Overloadable Marco function



#define DEBUG_LINE( ... ) VA_SELECT( DEBUG_LINE, __VA_ARGS__ )

#define DEBUG_LINE_0()\
{\
    std::string fileName = __FILE__;\
    std::size_t found = fileName.find_last_of("/\\");\
    std::cout<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<"\n";\
}

#define DEBUG_LINE_1(debugText)\
{\
    std::string fileName = __FILE__;\
    std::size_t found = fileName.find_last_of("/\\");\
    std::cout<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<": "<<debugText<<"\n";\
}


#endif
#ifndef SSGUI_CONFIG
#define SSGUI_CONFIG

#include "ssGUI/DebugAndBuild/ssGUIDebugMacro.hpp"


#define USE_DEBUG 0         //This will show which function is not exited if crashes
#define DEBUG_STATE 0       //This will print all the state of each update (Pre-Update, Render, Post-Render, etc..)
#define REFRESH_CONSOLE 0   //Normally you have this on when DEBUG_STATE is on in order to refresh the console otherwise it will be spammed
#define SLOW_UPDATE 0        //This will slow down the update for easier debugging
#define USE_SFML_TIME 0     //Can use SFML time function instead, but it's limited to int32 which could be not enough though

#if !USE_DEBUG

    #define FUNC_DEBUG_LINE(debugText)

#else

    #define FUNC_DEBUG_LINE( ... ) VA_SELECT( FUNC_DEBUG_LINE, __VA_ARGS__ )

    #define FUNC_DEBUG_LINE_1(debugText) std::cout<<__FILE__<<" in "<<__func__<<" on "<<__LINE__<<": "<<debugText<<"\n";

#endif

//Macros are defined here to indicate which backend to be used
#define USE_SFML_BACKEND


#endif
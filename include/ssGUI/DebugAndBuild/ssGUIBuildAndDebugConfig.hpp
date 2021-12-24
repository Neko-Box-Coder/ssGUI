#ifndef SSGUI_CONFIG
#define SSGUI_CONFIG


#include "ssGUI/DebugAndBuild/ssGUIDebugMacro.hpp"
#include <string>

#define USE_DEBUG 0         //This will show which function is not exited if crashes
#define DEBUG_STATE 0       //This will print all the state of each update (Pre-Update, Render, Post-Render, etc..)
#define REFRESH_CONSOLE 0   //Normally you have this on when DEBUG_STATE is on in order to refresh the console otherwise it will be spammed
#define SLOW_UPDATE 0        //This will slow down the update for easier debugging
#define USE_SFML_TIME 0     //Can use SFML time function instead, but it's limited to int32 which could be not enough though

#if !USE_DEBUG

    #define FUNC_DEBUG_ENTRY()

    #define FUNC_DEBUG_EXIT()


#else
    extern int TabSpace;
    
    inline std::string TabAdder(int tabAmount)
    {
        std::string returnString = "";
        for(int tab = 0; tab < tabAmount; tab++)
        {
            returnString += "|  ";
        }

        return returnString;
    }

    #define FUNC_DEBUG_ENTRY() std::cout<<TabAdder(TabSpace++)<<typeid(*this).name()<<" in "<<__func__<<" on "<<__LINE__<<": "<<"Entry"<<"\n";

    #define FUNC_DEBUG_EXIT() std::cout<<TabAdder(--TabSpace)<<typeid(*this).name()<<" in "<<__func__<<" on "<<__LINE__<<": "<<"Exit"<<"\n";

#endif

//Macros are defined here to indicate which backend to be used
#define USE_SFML_BACKEND


#endif
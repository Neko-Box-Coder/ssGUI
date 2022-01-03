#ifndef SSGUI_CONFIG
#define SSGUI_CONFIG


#include "ssGUI/DebugAndBuild/ssGUIDebugMacro.hpp"
#include <string>

#define USE_DEBUG 1         //This will show which function is not exited if crashes
#define DEBUG_STATE 0       //This will print all the state of each update (Pre-Update, Render, Post-Render, etc..)
#define REFRESH_CONSOLE 0   //Normally you have this on when DEBUG_STATE is on in order to refresh the console otherwise it will be spammed
#define SLOW_UPDATE 0        //This will slow down the update for easier debugging
#define USE_SFML_TIME 0     //Can use SFML time function instead, but it's limited to int32 which could be not enough though

#if !USE_DEBUG

    #define FUNC_DEBUG_ENTRY( ... ) VA_SELECT( FUNC_DEBUG_ENTRY, __VA_ARGS__ )
    #define FUNC_DEBUG_EXIT( ... ) VA_SELECT( FUNC_DEBUG_EXIT, __VA_ARGS__ )

    #define FUNC_DEBUG_ENTRY_0()
    #define FUNC_DEBUG_EXIT_0()

    #define FUNC_DEBUG_ENTRY_1(customFunc)
    #define FUNC_DEBUG_EXIT_1(customFunc)

    #define DEBUG_EXIT_PROGRAM()

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
    
    #undef DEBUG_LINE
    #define DEBUG_LINE( ... ) VA_SELECT( DEBUG_LINE, __VA_ARGS__ )

    #undef DEBUG_LINE_0
    #define DEBUG_LINE_0()\
    {\
        std::string fileName = __FILE__;\
        std::size_t found = fileName.find_last_of("/\\");\
        std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<"\n";\
    }

    #undef DEBUG_LINE_1
    #define DEBUG_LINE_1(debugText)\
    {\
        std::string fileName = __FILE__;\
        std::size_t found = fileName.find_last_of("/\\");\
        std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<": "<<debugText<<"\n";\
    }

    #define FUNC_DEBUG_ENTRY( ... ) VA_SELECT( FUNC_DEBUG_ENTRY, __VA_ARGS__ )
    #define FUNC_DEBUG_EXIT( ... ) VA_SELECT( FUNC_DEBUG_EXIT, __VA_ARGS__ )

    #define FUNC_DEBUG_ENTRY_0()\
    {\
        std::string fileName = __FILE__;\
        std::size_t found = fileName.find_last_of("/\\");\
        std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<": Entry\n";\
        TabSpace++;\
    }

    #define FUNC_DEBUG_EXIT_0()\
    {\
        TabSpace--;\
        std::string fileName = __FILE__;\
        std::size_t found = fileName.find_last_of("/\\");\
        std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<": Exit\n";\
    }

    #define FUNC_DEBUG_ENTRY_1(customFunc)\
    {\
        std::string fileName = __FILE__;\
        std::size_t found = fileName.find_last_of("/\\");\
        std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<customFunc<<" on "<<__LINE__<<": Entry\n";\
        TabSpace++;\
    }

    #define FUNC_DEBUG_EXIT_1(customFunc)\
    {\
        TabSpace--;\
        std::string fileName = __FILE__;\
        std::size_t found = fileName.find_last_of("/\\");\
        std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<customFunc<<" on "<<__LINE__<<": Exit\n";\
    }

    #define DEBUG_EXIT_PROGRAM() std::exit(EXIT_FAILURE);

#endif

//Macros are defined here to indicate which backend to be used
#define USE_SFML_BACKEND


#endif
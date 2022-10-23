#ifndef SSGUI_CONFIG
#define SSGUI_CONFIG

#include "ssLogger/ssLog.hpp"
// #include "ssGUI/DebugAndBuild/ssGUIDebugMacro.hpp"
#include <string>
#include <stack>

#define SSGUI_USE_DEBUG 0         //This will show which function is not exited if crashes
#define SSGUI_DEBUG_STATE 0       //This will print all the state of each update (Pre-Update, Render, Post-Render, etc..)
#define SSGUI_REFRESH_CONSOLE 0   //Normally you have this on when DEBUG_STATE is on in order to refresh the console otherwise it will be spammed
#define SSGUI_SLOW_UPDATE 0        //This will slow down the update for easier debugging
#define SSGUI_USE_SFML_TIME 0     //Can use SFML time function instead, but it's limited to int32 which could be not enough though

// #if !SSGUI_USE_DEBUG

//     #define FUNC_DEBUG_ENTRY( ... ) VA_SELECT( FUNC_DEBUG_ENTRY, __VA_ARGS__ )
//     #define FUNC_DEBUG_EXIT( ... ) VA_SELECT( FUNC_DEBUG_EXIT, __VA_ARGS__ )

//     #define FUNC_DEBUG_ENTRY_0()
//     #define FUNC_DEBUG_EXIT_0()

//     #define FUNC_DEBUG_ENTRY_1(customFunc)
//     #define FUNC_DEBUG_EXIT_1(customFunc)

//     #define DEBUG_EXIT_PROGRAM()

// #else
//     extern int TabSpace;
//     extern std::stack<std::string> FuncNameStack; 
    
//     inline std::string TabAdder(int tabAmount)
//     {
//         std::string returnString = "";
//         for(int tab = 0; tab < tabAmount; tab++)
//         {
//             returnString += "|  ";
//         }

//         return returnString;
//     }
    
//     #undef DEBUG_LINE
//     #define DEBUG_LINE( ... ) VA_SELECT( DEBUG_LINE, __VA_ARGS__ )

//     #undef DEBUG_LINE_0
//     #define DEBUG_LINE_0()\
//     {\
//         std::string fileName = __FILE__;\
//         std::size_t found = fileName.find_last_of("/\\");\
//         std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<"\n";\
//     }

//     #undef DEBUG_LINE_1
//     #define DEBUG_LINE_1(debugText)\
//     {\
//         std::string fileName = __FILE__;\
//         std::size_t found = fileName.find_last_of("/\\");\
//         std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<": "<<debugText<<"\n";\
//     }

//     #define FUNC_DEBUG_ENTRY( ... ) VA_SELECT( FUNC_DEBUG_ENTRY, __VA_ARGS__ )
//     #define FUNC_DEBUG_EXIT( ... ) VA_SELECT( FUNC_DEBUG_EXIT, __VA_ARGS__ )

//     #define FUNC_DEBUG_ENTRY_0()\
//     {\
//         std::string fileName = __FILE__;\
//         std::size_t found = fileName.find_last_of("/\\");\
//         std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<": Entry\n";\
//         FuncNameStack.push(__func__);\
//         TabSpace++;\
//     }

//     #define FUNC_DEBUG_EXIT_0()\
//     {\
//         if(FuncNameStack.empty() || FuncNameStack.top() != __func__)\
//         {\
//             std::cout<<"FUNC_DEBUG_EXIT is expecting "<<FuncNameStack.top()<<". "<<__func__<<" is found instead.\n";\
//             std::exit(EXIT_FAILURE);\
//         }\
//         FuncNameStack.pop();\
//         TabSpace--;\
//         std::string fileName = __FILE__;\
//         std::size_t found = fileName.find_last_of("/\\");\
//         std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<__func__<<" on "<<__LINE__<<": Exit\n";\
//     }

//     #define FUNC_DEBUG_ENTRY_1(customFunc)\
//     {\
//         std::string fileName = __FILE__;\
//         std::size_t found = fileName.find_last_of("/\\");\
//         std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<customFunc<<" on "<<__LINE__<<": Entry\n";\
//         FuncNameStack.push(customFunc);\
//         TabSpace++;\
//     }

//     #define FUNC_DEBUG_EXIT_1(customFunc)\
//     {\
//         if(FuncNameStack.empty() || FuncNameStack.top() != customFunc)\
//         {\
//             std::cout<<"FUNC_DEBUG_EXIT is expecting "<<FuncNameStack.top()<<". "<<customFunc<<" is found instead.\n";\
//             std::exit(EXIT_FAILURE);\
//         }\
//         FuncNameStack.pop();\
//         TabSpace--;\
//         std::string fileName = __FILE__;\
//         std::size_t found = fileName.find_last_of("/\\");\
//         std::cout<<TabAdder(TabSpace)<<fileName.substr(found+1)<<" in "<<customFunc<<" on "<<__LINE__<<": Exit\n";\
//     }

//     #define DEBUG_EXIT_PROGRAM() std::exit(EXIT_FAILURE);

// #endif

//Macros are defined here to indicate which backend to be used
#define USE_SFML_BACKEND


#endif
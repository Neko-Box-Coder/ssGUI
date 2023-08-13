#ifndef SSGUI_MOCK_MACRO_H
#define SSGUI_MOCK_MACRO_H

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#define SSGUI_MOCK_PASSTHROUGH(funcOp)\
do\
{\
    if(UnderlyingInterface != nullptr)\
        UnderlyingInterface->funcOp;\
}\
while(0)

#define SSGUI_MOCK_PASSTHROUGH_AND_RETURN(funcOp)\
do\
{\
    if(UnderlyingInterface != nullptr)\
    {\
        UnderlyingInterface->funcOp;\
        return;\
    }\
}\
while(0)


//#define SSGUI_MOCK_ENABLE_LOG
#ifdef SSGUI_MOCK_ENABLE_LOG
    #define SSGUI_MOCK_INTERNAL_LOG_RESULT(res) ssLOG_LINE("Mock function returned with: "<<res)
#else
    #define SSGUI_MOCK_INTERNAL_LOG_RESULT(res)
#endif

#define SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(funcOp, returnType)\
do\
{\
    if(UnderlyingInterface != nullptr)\
    {\
        returnType result = UnderlyingInterface->funcOp;\
        SSGUI_MOCK_INTERNAL_LOG_RESULT(result);\
        return result;\
    }\
}\
while(0)

#define SSGUI_MOCK_PASSTHROUGH_AND_RETURN_IF_FALSE(funcOp)\
do\
{\
    if(UnderlyingInterface != nullptr)\
    {\
        bool result = UnderlyingInterface->funcOp;\
        if(!result)\
        {\
            SSGUI_MOCK_INTERNAL_LOG_RESULT(result);\
            return false;\
        }\
    }\
}\
while(0)

#ifdef SSGUI_MOCK_ENABLE_LOG
    // =======================================================================
    // Macros for allowing overloadable Macro functions
    // =======================================================================

    // https://stackoverflow.com/questions/16683146/can-macros-be-overloaded-by-number-of-arguments
    #define INTERNAL_ssGUI_CAT( A, B ) A ## B
    #define INTERNAL_ssGUI_SELECT( NAME, NUM ) INTERNAL_ssGUI_CAT( NAME ## _, NUM )
    #define INTERNAL_ssGUI_COMPOSE( NAME, ARGS ) NAME ARGS

    #define INTERNAL_ssGUI_GET_COUNT( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20 /* ad nauseam */, COUNT, ... ) COUNT
    #define INTERNAL_ssGUI_EXPAND() ,,,,,,,,,,,,,,,,,,,, // 20 commas (or 21 empty tokens)
    #define INTERNAL_ssGUI_ESCAPE_BRACKET(...) __VA_ARGS__ ()
    #define INTERNAL_ssGUI_CAT_ALLOW_GARBAGE(_1, ...) _1 __VA_ARGS__
    #define INTERNAL_ssGUI_ESCAPED_EXPAND(...) INTERNAL_ssGUI_CAT_ALLOW_GARBAGE(INTERNAL_ssGUI_EXPAND, INTERNAL_ssGUI_ESCAPE_BRACKET __VA_ARGS__ ())
    #define INTERNAL_ssGUI_VA_SIZE( ... ) INTERNAL_ssGUI_COMPOSE( INTERNAL_ssGUI_GET_COUNT, (INTERNAL_ssGUI_ESCAPED_EXPAND(__VA_ARGS__), 0, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1) )
    //#define INTERNAL_ssGUI_VA_SIZE( ... ) INTERNAL_ssGUI_COMPOSE( INTERNAL_ssGUI_GET_COUNT, (INTERNAL_ssGUI_EXPAND __VA_ARGS__ (), 0, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1) )

    #ifndef _MSC_VER
        #define INTERNAL_ssGUI_VA_SELECT( NAME, ... ) INTERNAL_ssGUI_SELECT( NAME, INTERNAL_ssGUI_VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)
    #else
        //MSVC workaround: https://stackoverflow.com/questions/48710758/how-to-fix-variadic-macro-related-issues-with-macro-overloading-in-msvc-mic
        //This is honestly retarded.
        #define INTERNAL_ssGUI_VA_ARGS_FIX( macro, args ) macro args
        #define INTERNAL_ssGUI_VA_SELECT( NAME, ... ) INTERNAL_ssGUI_VA_ARGS_FIX(INTERNAL_ssGUI_SELECT, ( NAME, INTERNAL_ssGUI_VA_SIZE( __VA_ARGS__ ) )) (__VA_ARGS__)
    #endif

    #define SSGUI_MOCK_LOG_FUNCTION_CALL(...) INTERNAL_ssGUI_VA_SELECT(INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL, __VA_ARGS__)
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_0() ssLOG_LINE();ssLOG_LINE("Mock function called");
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_1(_1) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_0()\
    ssLOG_LINE(#_1<<": "<<_1);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_2(_1, _2) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_1(_1)\
    ssLOG_LINE(#_2<<": "<<_2);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_3(_1, _2, _3) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_2(_1, _2)\
    ssLOG_LINE(#_3<<": "<<_3);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_4(_1, _2, _3, _4) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_3(_1, _2, _3) \
    ssLOG_LINE(#_4<<": "<<_4);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_5(_1, _2, _3, _4, _5) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_4(_1, _2, _3, _4) \
    ssLOG_LINE(#_5<<": "<<_5);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_6(_1, _2, _3, _4, _5, _6) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_5(_1, _2, _3, _4, _5) \
    ssLOG_LINE(#_6<<": "<<_6);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_7(_1, _2, _3, _4, _5, _6, _7) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_6(_1, _2, _3, _4, _5, _6) \
    ssLOG_LINE(#_7<<": "<<_7);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_8(_1, _2, _3, _4, _5, _6, _7, _8) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_7(_1, _2, _3, _4, _5, _6, _7) \
    ssLOG_LINE(#_8<<": "<<_8);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_8(_1, _2, _3, _4, _5, _6, _7, _8) \
    ssLOG_LINE(#_9<<": "<<_9);
    
    #define INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    INTERNAL_SSGUI_MOCK_LOG_FUNCTION_CALL_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) \
    ssLOG_LINE(#_10<<": "<<_10);
    
#else
    #define SSGUI_MOCK_LOG_FUNCTION_CALL(...)
#endif


#define SSGUI_MOCK_DECLARE_VARIABLE_GETTER(variableType, variableName)\
inline variableType& GetMock ## variableName () { return variableName; }

#endif

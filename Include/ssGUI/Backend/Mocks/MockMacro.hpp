#ifndef SSGUI_MOCK_MACRO_H
#define SSGUI_MOCK_MACRO_H

#include "ssLogger/ssLog.hpp"

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


#define SSGUI_MOCK_PASSTHROUGH_AND_RETURN_FUNC(funcOp)\
do\
{\
    if(UnderlyingInterface != nullptr)\
    {\
        auto result = UnderlyingInterface->funcOp;\
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
    #define SSGUI_MOCK_LOG_FUNCTION_CALL() ssLOG_LINE("Mock function called");
#else
    #define SSGUI_MOCK_LOG_FUNCTION_CALL()
#endif


#define SSGUI_MOCK_DECLARE_VARIABLE_GETTER(variableType, variableName)\
inline variableType& GetMock ## variableName () { return variableName; }

#endif

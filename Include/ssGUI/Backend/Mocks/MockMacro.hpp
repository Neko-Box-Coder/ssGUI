#ifndef SSGUI_MOCK_MACRO_H
#define SSGUI_MOCK_MACRO_H

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
        return UnderlyingInterface->funcOp;\
}\
while(0)

#define SSGUI_MOCK_PASSTHROUGH_AND_RETURN_IF_FALSE(funcOp)\
do\
{\
    if(UnderlyingInterface != nullptr)\
    {\
        bool result = UnderlyingInterface->funcOp;\
        if(!result)\
            return false;\
    }\
}\
while(0)

#endif

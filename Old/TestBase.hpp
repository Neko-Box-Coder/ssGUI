#ifndef SSGUI_TEST_BASE_H
#define SSGUI_TEST_BASE_H

#include <exception>

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#define SSGUI_TEST_FILE_NAME()\
[]()\
{\
    std::string ssGUITestfileName = __FILE__;\
    std::size_t ssGUITestfound = ssGUITestfileName.find_last_of("/\\");\
    std::size_t ssGUITestExtfound = ssGUITestfileName.rfind(".c");\
    return ssGUITestfileName.substr(ssGUITestfound+1, ssGUITestExtfound - ssGUITestfound - 1);\
}()



#ifdef _WIN32
    #include <windows.h>
    #define SSGUI_TEST_TITLE(title)\
    SetConsoleOutputCP(CP_UTF8);\
    TestName = title;\
    printf("==========================================================\n");\
    printf("🚀 %s\n", title.c_str());\
    printf("==========================================================\n");
#else
    #define SSGUI_TEST_TITLE(title)\
    TestName = title;\
    printf("==========================================================\n");\
    printf("🚀 %s\n", title.c_str());\
    printf("==========================================================\n");
#endif


#define SSGUI_TEST_OUTPUT_ASSERT(name, assert)\
{\
    std::string titleString;\
    std::string resultString;\
    try\
    {\
        bool result = assert;\
        titleString = std::string("🧪 ")+std::string(name);\
        if(result)\
        {\
            resultString = "Passed ✅";\
            ssGUITestSuccess++;\
        }\
        else\
        {\
            resultString = "Failed ❌";\
            ssGUITestFailed++;\
        }\
    }\
    catch(std::exception& e)\
    {\
        titleString = std::string(name);\
        resultString = std::string("Error Catched: ")+std::string(e.what());\
        ssGUITestFailed++;\
    }\
    catch(...)\
    {\
        titleString = std::string(name);\
        resultString = std::string("Error occured");\
        ssGUITestFailed++;\
        throw;\
    }\
    if(titleString.size() < 50)\
        printf("%-50s %20s\n", (titleString+":").c_str(), resultString.c_str());\
    else\
        printf("%-60s %20s\n", (titleString+":").c_str(), resultString.c_str());\
}

#define SSGUI_TEST_OUTPUT_SKIP(name)\
{\
    std::string titleString= std::string("🧪 ")+std::string(name);\
    std::string resultString = "Skipped 🚫";\
    printf("%-50s %21s\n", (titleString+":").c_str(), resultString.c_str());\
}

#define SSGUI_TEST_INIT()\
namespace\
{\
    int ssGUITestSuccess = 0;\
    int ssGUITestFailed = 0;\
    std::string TestName = "";\
}

#define SSGUI_TEST_END()\
int ssGUITotal = ssGUITestSuccess + ssGUITestFailed;\
std::cout<<"\nResults:\n";\
std::cout<<ssGUITestSuccess<<"/"<<ssGUITotal<<" tests passed\n";\
if(ssGUITestFailed > 0)\
{\
    std::cout<<TestName<<" has failed some tests 😟\n\n";\
    return EXIT_FAILURE;\
}\
else\
{\
    std::cout<<TestName<<" has passed all tests 😄\n\n";\
    return EXIT_SUCCESS;\
}

#endif
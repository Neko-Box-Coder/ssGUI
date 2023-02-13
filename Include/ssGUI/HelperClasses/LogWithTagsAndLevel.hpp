#ifndef LOG_WITH_TAGS_AND_LEVEL_H
#define LOG_WITH_TAGS_AND_LEVEL_H

#include "ssLogger/ssLog.hpp"

#define ssGUI_LOG_LEVEL_DEBUG 4
#define ssGUI_LOG_LEVEL_INFO 3
#define ssGUI_LOG_LEVEL_WARNING 2
#define ssGUI_LOG_LEVEL_ERROR 1

//#define ssGUI_LOG_LEVEL 1
//#define ssGUI_LOG_TAG 20


#define ssGUI_MANAGER_TAG 1
#define ssGUI_IMAGE_UTIL_TAG 2
#define ssGUI_BACKEND_TAG 3
#define ssGUI_DATA_TAG 4
#define ssGUI_EXT_TAG 5
#define ssGUI_GUI_OBJECT_TAG 6



#if defined (ssGUI_LOG_TAG)
    #define ssGUI_DEBUG(tag, ...) if(tag == ssGUI_LOG_TAG){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("🐞 DEBUG: "););ssLOG_LINE(__VA_ARGS__); }
    #define ssGUI_INFO(tag, ...) if(tag == ssGUI_LOG_TAG){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("💬 INFO: "););ssLOG_LINE(__VA_ARGS__); }
    #define ssGUI_WARNING(tag, ...) if(tag == ssGUI_LOG_TAG){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("⚠️ WARNING: "););ssLOG_LINE(__VA_ARGS__); }
    #define ssGUI_ERROR(tag, ...) if(tag == ssGUI_LOG_TAG){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("☢️ ERROR: "););ssLOG_LINE(__VA_ARGS__); }

#elif defined (ssGUI_LOG_LEVEL)

    #if ssGUI_LOG_LEVEL >= ssGUI_LOG_LEVEL_DEBUG
        #define ssGUI_DEBUG(tag, ...){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("🐞 DEBUG: "););ssLOG_LINE(__VA_ARGS__); }
    #else
        #define ssGUI_DEBUG(...) {}
    #endif

    #if ssGUI_LOG_LEVEL >= ssGUI_LOG_LEVEL_INFO
        #define ssGUI_INFO(tag, ...){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("💬 INFO: "););ssLOG_LINE(__VA_ARGS__); }
    #else
        #define ssGUI_INFO(...) {}
    #endif

    #if ssGUI_LOG_LEVEL >= ssGUI_LOG_LEVEL_WARNING
        #define ssGUI_WARNING(tag, ...){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("⚠️ WARNING: "););ssLOG_LINE(__VA_ARGS__); }
    #else
        #define ssGUI_WARNING(...) {}
    #endif

    #if ssGUI_LOG_LEVEL >= ssGUI_LOG_LEVEL_ERROR
        #define ssGUI_ERROR(tag, ...){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("☢️ ERROR: "););ssLOG_LINE(__VA_ARGS__); }
    #else
        #define ssGUI_ERROR(...) {}
    #endif

#else

    #define ssGUI_DEBUG(...) {}
    #define ssGUI_INFO(...) {}
    #define ssGUI_WARNING(...) {}
    #define ssGUI_ERROR(...) {}

#endif

//#undef ssLOG_LINE

#endif
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
    #define ssGUI_DEBUG(tag, ...) do{ if(tag == ssGUI_LOG_TAG){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("🐞 DEBUG: "););ssLOG_LINE(__VA_ARGS__); } } while(0)
    #define ssGUI_INFO(tag, ...) do{ if(tag == ssGUI_LOG_TAG){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("💬 INFO: "););ssLOG_LINE(__VA_ARGS__); } } while(0)
    #define ssGUI_WARNING(tag, ...) do{ if(tag == ssGUI_LOG_TAG){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("❗ WARNING: "););ssLOG_LINE(__VA_ARGS__); } } while(0)
    #define ssGUI_ERROR(tag, ...) do{ if(tag == ssGUI_LOG_TAG){ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("⛔ ERROR: "););ssLOG_LINE(__VA_ARGS__); } } while(0)

#elif defined (ssGUI_LOG_LEVEL)

    #if ssGUI_LOG_LEVEL >= ssGUI_LOG_LEVEL_DEBUG
        #define ssGUI_DEBUG(tag, ...) do{ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("🐞 DEBUG: "););ssLOG_LINE(__VA_ARGS__); } while(0)
    #else
        #define ssGUI_DEBUG(...) do{} while(0)
    #endif

    #if ssGUI_LOG_LEVEL >= ssGUI_LOG_LEVEL_INFO
        #define ssGUI_INFO(tag, ...) do{ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("💬 INFO: "););ssLOG_LINE(__VA_ARGS__); } while(0)
    #else
        #define ssGUI_INFO(...) do{} while(0)
    #endif

    #if ssGUI_LOG_LEVEL >= ssGUI_LOG_LEVEL_WARNING
        #define ssGUI_WARNING(tag, ...) do{ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("❗ WARNING: "););ssLOG_LINE(__VA_ARGS__); } while(0)
    #else
        #define ssGUI_WARNING(...) do{} while(0)
    #endif

    #if ssGUI_LOG_LEVEL >= ssGUI_LOG_LEVEL_ERROR
        #define ssGUI_ERROR(tag, ...) do{ INTERNAL_ssLOG_THREAD_SAFE_OP(ssLOG_PREPEND("⛔ ERROR: "););ssLOG_LINE(__VA_ARGS__); } while(0)
    #else
        #define ssGUI_ERROR(...) do{} while(0)
    #endif

#else
    #define ssGUI_DEBUG(...) do{} while(0)
    #define ssGUI_INFO(...) do{} while(0)
    #define ssGUI_WARNING(...) do{} while(0)
    #define ssGUI_ERROR(...) do{} while(0)

#endif

#if ssGUI_USE_LOG_FUNC
    #define ssGUI_LOG_FUNC(...) ssLOG_FUNC(__VA_ARGS__)
    #define ssGUI_LOG_FUNC_ENTRY(...) ssLOG_FUNC_ENTRY(__VA_ARGS__)
    #define ssGUI_LOG_FUNC_EXIT(...) ssLOG_FUNC_EXIT(__VA_ARGS__)
#else
    #define ssGUI_LOG_FUNC(...) do{} while(0)
    #define ssGUI_LOG_FUNC_ENTRY(...) do{} while(0)
    #define ssGUI_LOG_FUNC_EXIT(...) do{} while(0)
#endif

#endif
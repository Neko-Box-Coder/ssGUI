#ifndef SSGUI_STATIC_DEFAULT_WRAPPER
#define SSGUI_STATIC_DEFAULT_WRAPPER

#include "ssGUI/Factory.hpp"

namespace ssGUI 
{
    template<typename T>
    class StaticDefaultWrapper                                                  //Internal static wrapper for deallocating static variables
    {
        public:
            T* Obj = nullptr;
            bool ssGUIDefault = false;
            StaticDefaultWrapper() = default;
            inline ~StaticDefaultWrapper()
            {
                if(Obj != nullptr && ssGUIDefault)
                ssGUI::Factory::Dispose(Obj);
            };
    };
}

#endif
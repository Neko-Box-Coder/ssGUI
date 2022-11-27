#ifndef SSGUI_STATIC_DEFAULT_WRAPPER_H
#define SSGUI_STATIC_DEFAULT_WRAPPER_H

#include <functional>
#include <vector>

#include "ssGUI/Factory.hpp"

namespace ssGUI 
{
    template<typename T>
    class StaticDefaultWrapper                                                  //Internal static wrapper for deallocating static variables
    {
        public:
            T* Obj = nullptr;
            bool ssGUIDefault = false;

            std::vector<std::function<void()>> CleanUpFunc;

            inline StaticDefaultWrapper() = default;
            inline ~StaticDefaultWrapper()
            {
                if(CleanUpFunc.empty())
                {
                    if(Obj != nullptr && ssGUIDefault)
                        ssGUI::Factory::Dispose(Obj);
                }
                else
                {
                    for(int i = 0; i < CleanUpFunc.size(); i++)
                        CleanUpFunc[i]();
                }
            };
    };
}

#endif
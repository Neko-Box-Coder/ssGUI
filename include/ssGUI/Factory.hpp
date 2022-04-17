#ifndef SSGUI_Factory
#define SSGUI_Factory

//namespace: ssGUI
namespace ssGUI
{
    //class: Factory
    //<Extension>s & <EventCallback>s must be created and disposed via this class.
    //You can also create GUI Objects via this class but it is not required.
    class Factory
    {
        public:
            template<typename T> 
            //function: Create
            static T* Create()
            {
                //static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value);
                return new T();
            };

            template<typename T>
            //function: Dispose
            static void Dispose(T* extension)
            {
                //static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value);
                delete extension;
            };
    };
}

#endif
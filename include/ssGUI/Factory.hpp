#ifndef SSGUI_Factory
#define SSGUI_Factory

//namespace: ssGUI
namespace ssGUI
{
    //class: ssGUI::Factory
    //<Extension>s & <EventCallback>s must be created and disposed via this class.
    //You can also create GUI Objects via this class but it is not required.
    class Factory
    {
        public:
            //function: Create
            //Creates the object on the heap
            template<typename T> 
            static T* Create()
            {
                //static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value);
                return new T();
            };

            //function: Dispose
            //Delete the object allocated on the heap
            template<typename T>
            static void Dispose(T* extension)
            {
                //static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value);
                delete extension;
            };
    };
}

#endif
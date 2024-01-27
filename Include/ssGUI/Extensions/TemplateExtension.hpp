#ifndef SSGUI_TEMPLATE_EXTENSION_HPP
#define SSGUI_TEMPLATE_EXTENSION_HPP

#include "ssGUI/Extensions/Extension.hpp"

namespace ssGUI 
{
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::TemplateExtension
    A template Extension. Use this to create new Extension Classes.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;        //See <BindToObject>
        bool Enabled;                       //See <IsEnabled>
    =================================================================
    ============================== C++ ==============================
    TemplateExtension::TemplateExtension() :    Container(nullptr),
                                                Enabled(true)
    {}
    =================================================================
    */
    class TemplateExtension : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            TemplateExtension& operator=(TemplateExtension const& other);
        
        protected:
            ssGUI::GUIObject* Container;        //See <BindToObject>
            bool Enabled;                       //See <IsEnabled>

            TemplateExtension();
            virtual ~TemplateExtension() override;
            TemplateExtension(TemplateExtension const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(   ssGUI::Backend::DrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset) override;

        public:
            //====================================================================
            //Group: Constants
            //====================================================================

            //string: EXTENSION_NAME
            static const std::string EXTENSION_NAME;

            //====================================================================
            //Group: Overrides
            //====================================================================

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;
            
            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(   bool isPreUpdate, 
                                            ssGUI::Backend::SystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw( bool isPreRender, 
                                        ssGUI::Backend::DrawingInterface* drawingInterface, 
                                        ssGUI::GUIObject* mainWindow, 
                                        glm::vec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() const override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(const ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            //See <Extension::Clone>
            virtual TemplateExtension* Clone() override;
    };
}

}

#endif
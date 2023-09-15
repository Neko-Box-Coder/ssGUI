#include "ssGUI/Extensions/TemplateExtension.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

namespace ssGUI
{

namespace Extensions
{
    TemplateExtension::TemplateExtension() :    Container(nullptr),
                                                Enabled(true)
    {}

    TemplateExtension::~TemplateExtension()
    {}

    TemplateExtension::TemplateExtension(TemplateExtension const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
    }
    
    void TemplateExtension::ConstructRenderInfo()
    {}

    void TemplateExtension::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset)
    {}

    const std::string TemplateExtension::EXTENSION_NAME = "Template Extension";
    
    bool TemplateExtension::IsEnabled() const
    {
        return Enabled;
    }

    void TemplateExtension::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }
    
    void TemplateExtension::Internal_Update(bool isPreUpdate, 
                                            ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();
        
        //This is function is executed twice, one before the Container GUI object update and one after.
        //You can use the isPreUpdate variable to decide when to execute the extension update
        if(!isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
            return;
    }

    void TemplateExtension::Internal_Draw(  bool isPreRender, 
                                            ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                            ssGUI::GUIObject* mainWindow, 
                                            glm::vec2 mainWindowPositionOffset)
    {
    }

    std::string TemplateExtension::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void TemplateExtension::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void TemplateExtension::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::TemplateExtension* original = static_cast<ssGUI::Extensions::TemplateExtension*>(extension);
        Enabled = original->IsEnabled();
    }

    ObjectsReferences* TemplateExtension::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    TemplateExtension* TemplateExtension::Clone()
    {
        TemplateExtension* temp = new TemplateExtension(*this);
        return temp;
    }
}

}
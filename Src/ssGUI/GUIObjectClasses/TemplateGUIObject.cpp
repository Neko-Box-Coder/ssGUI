#include "ssGUI/GUIObjectClasses/TemplateGUIObject.hpp"

namespace ssGUI
{
    TemplateGUIObject::TemplateGUIObject(TemplateGUIObject const& other) : Widget(other)
    {
    }
    
    void TemplateGUIObject::ConstructRenderInfo()
    {
    }
    
    const std::string TemplateGUIObject::ListenerKey = "Template GUI Object";
    
    TemplateGUIObject::TemplateGUIObject()
    {
    }

    TemplateGUIObject::~TemplateGUIObject()
    {
        
    }
    
    ssGUI::Enums::GUIObjectType TemplateGUIObject::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET;
    }
    
    TemplateGUIObject* TemplateGUIObject::Clone(bool cloneChildren)
    {
        ssGUI_LOG_FUNC();
        TemplateGUIObject* temp = new TemplateGUIObject(*this);
        CloneExtensionsAndEventCallbacks(temp);   

        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
                return nullptr;
        }

        return temp;
    }
}
#ifndef SSGUI_TEMPLATE_GUI_OBJECT_HPP
#define SSGUI_TEMPLATE_GUI_OBJECT_HPP

#include "ssGUI/GUIObjectClasses/Widget.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: TemplateGUIObject
    A template GUI Object. Use this to create new GUI Object Classes.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
    =================================================================
    ============================== C++ ==============================
    TemplateGUIObject::TemplateGUIObject()
    {
    }
    =================================================================
    */
    class TemplateGUIObject : public Widget
    {
        private:
            TemplateGUIObject& operator=(TemplateGUIObject const& other) = default;
        
        protected:
            TemplateGUIObject(TemplateGUIObject const& other);
    
            virtual void ConstructRenderInfo() override;

        public:
            static const std::string ListenerKey;

            TemplateGUIObject();
            virtual ~TemplateGUIObject() override;

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual TemplateGUIObject* Clone(bool cloneChildren) override;
        
    };
}

#endif
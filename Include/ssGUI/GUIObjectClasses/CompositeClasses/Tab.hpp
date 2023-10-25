#ifndef SSGUI_TAB_HPP
#define SSGUI_TAB_HPP

#include "ssGUI/Enums/Direction.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"

//namespace: ssGUI
namespace ssGUI
{
    /*class: Tab
    A template GUI Object. Use this to create new GUI Object Classes.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
    =================================================================
    ============================== C++ ==============================
    Tab::Tab()
    {
    }
    =================================================================
    */
    class Tab : public StandardButton
    {
        private:
            Tab& operator=(Tab const& other) = default;
        
        protected:
            glm::u8vec4 TabColor = glm::u8vec4(100, 100, 100, 255);
            glm::u8vec4 SelectedColor = glm::u8vec4(127, 127, 127, 255);
            glm::u8vec4 UnderlineColor = glm::u8vec4(255, 255, 255, 127);

            float UnderlineThickness = 3;
            ssGUI::Enums::Direction UnderlineDirection = ssGUI::Enums::Direction::BOTTOM;
            
            ssGUIObjectIndex CloseButton;
            int CloseButtonShapeIds[2];

            ssGUIObjectIndex ContentObject;
            bool Selected = false;
        
            static glm::vec2 DefaultCloseButtonSize;
        
            Tab(Tab const& other);
    
            virtual void SetCloseSymbolColor(glm::u8vec4 color);

            virtual void InitializeCloseButton();
            
            virtual void MainLogic( ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            //====================================================================
            //Group: Constants
            //====================================================================

            //string: ListenerKey
            static const std::string ListenerKey;

            Tab();
            virtual ~Tab() override;

            virtual void SetAssociatedContent(ssGUI::GUIObject* content);

            virtual ssGUI::GUIObject* GetAssociatedContent() const;

            virtual void SetTabColor(glm::u8vec4 color);
            
            virtual glm::u8vec4 GetTabColor() const;
            
            virtual void SetSelectedColor(glm::u8vec4 color);
            
            virtual glm::u8vec4 GetSelectedColor() const;
            
            virtual void SetUnderlineColor(glm::u8vec4 color);
            
            virtual glm::u8vec4 GetUnderlineColor() const;
            
            virtual void SetUnderlineDirection(ssGUI::Enums::Direction direction);
            
            virtual ssGUI::Enums::Direction GetUnderlineDirection() const;
            
            virtual void SetUnderlineThickness(float thickness);
            
            virtual float GetUnderlineThickness() const;
            
            virtual void SetCloseButton(ssGUI::Button* button);
            
            virtual ssGUI::Button* GetCloseButton() const;
            
            virtual void SelectTab(bool select);
            
            virtual bool IsTabSelected() const;

            virtual void SetTabIcon(ssGUI::Image* icon);
            
            virtual ssGUI::Image* GetTabIcon() const;
            
            virtual void SetTabTitleObject(ssGUI::Text* title);
            
            virtual ssGUI::Text* GetTabTitleObject() const;

            
            //====================================================================
            //Group: Overrides
            //====================================================================

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual Tab* Clone(bool cloneChildren) override;
        
    };
}

#endif
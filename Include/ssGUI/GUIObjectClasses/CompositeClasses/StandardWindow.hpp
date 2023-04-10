#ifndef SSGUI_STANDARD_WINDOW_H
#define SSGUI_STANDARD_WINDOW_H

#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/ssGUITags.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class Text;
    class Button;
    class Image;

    /*class: ssGUI::StandardWindow
    A standard window that contains window icon, title and close button. With addtional visual and funcational extensions added.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        int HorizontalPadding;              //See <GetHorizontalPadding>
        int VerticalPadding;                //See <GetVerticalPadding>
        bool AdaptiveTitleColor;            //See <SetAdaptiveTitleColor>
        glm::ivec4 TitleColorDifference;    //See <GetAdaptiveTitleColorDifference>
        bool AdaptiveTitleContrast;         //See <IsAdaptiveTitleContrast>
        bool AutoFontSize;                  //See <IsAutoFontSize>
        float FontSizeMultiplier;           //See <GetAutoFontSizeMultiplier>
        
        ssGUIObjectIndex WindowTitle;       //See <GetWindowTitleObject>
        ssGUIObjectIndex WindowIcon;        //See <GetWindowIconGUIObject>
        ssGUIObjectIndex CloseButton;       //See <GetCloseButtonObject>

        static ssGUI::ImageData* DefaultIcon;   //(Internal variable) Default window icon image
    =================================================================
    ============================== C++ ==============================
    StandardWindow::StandardWindow() :  HorizontalPadding(5),
                                        VerticalPadding(4),
                                        AdaptiveTitleColor(true),
                                        TitleColorDifference(),
                                        AdaptiveTitleContrast(true),
                                        AutoFontSize(true),
                                        FontSizeMultiplier(0.8),
                                        WindowTitle(-1),
                                        WindowIcon(-1),
                                        CloseButton(-1)
    {        
        ssLOG_FUNC_ENTRY();
        SetMinSize(glm::vec2(100, 100));
        SetTitlebarHeight(26);
        
        //Setup title
        auto windowTitle = new ssGUI::Text();
        windowTitle->SetUserCreated(false);
        windowTitle->SetHeapAllocated(true);
        windowTitle->SetParent(this, true);
        windowTitle->SetMinSize(glm::vec2(5, 5));
        windowTitle->SetNewTextColor(glm::u8vec4(255, 255, 255, 255));
        windowTitle->SetText("Window");
        WindowTitle = CurrentObjectsReferences.AddObjectReference(windowTitle);
        SetAdaptiveTitleColor(true);    //Setting it here so that eventcallback is added
        SetAdaptiveTitleColorDifference(glm::ivec4(150, 150, 150, 0));
        
        auto windowIcon = new ssGUI::Image();
        windowIcon->SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);
        windowIcon->SetUserCreated(false);
        windowIcon->SetHeapAllocated(true);
        windowIcon->SetParent(this, true);
        windowIcon->SetMinSize(glm::vec2(5, 5));
        WindowIcon = CurrentObjectsReferences.AddObjectReference(windowIcon);

        InitiateDefaultResources();
        if(DefaultIcon != nullptr)
            windowIcon->SetImageData(DefaultIcon);

        //Setup button
        auto closeButton = new ssGUI::Button();
        closeButton->SetUserCreated(false);
        closeButton->SetHeapAllocated(true);
        closeButton->SetParent(this, true);
        closeButton->SetMinSize(glm::vec2(5, 5));
        closeButton->RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);

        //Change button shape to circle
        auto shapeEx = closeButton->AddExtension<ssGUI::Extensions::Shape>();
        shapeEx->RemoveGUIObjectShape(0);
        int circleId = shapeEx->AddAdditionalCircle(glm::vec2(), closeButton->GetSize(), glm::u8vec4(255, 127, 127, 255), false);

        //Add outline to button
        auto closeButtonOutline = closeButton->AddExtension<ssGUI::Extensions::Outline>();
        closeButtonOutline->SetOutlineThickness(4);
        closeButtonOutline->SetOutlineColor(glm::u8vec4(255, 127, 127, 255));
        closeButton->SetButtonColor(glm::u8vec4(255, 127, 127, 255));

        //Setup button event
        auto buttonEvent = closeButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        buttonEvent->RemoveEventListener(ssGUI::Button::ListenerKey, closeButton);
        buttonEvent->AddEventListener
        (
            ListenerKey, this,
            [circleId](ssGUI::EventInfo info)
            {
                auto closeButtonObj = static_cast<ssGUI::Button*>(info.EventSource);
                auto shape = static_cast<ssGUI::Extensions::Shape*>(info.EventSource->GetExtension(ssGUI::Extensions::Shape::EXTENSION_NAME));
                int amount = 60;
                
                static_assert((int)ssGUI::Enums::ButtonState::COUNT == 6, "Make sure this is updated");
                switch(closeButtonObj->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), 
                            closeButtonObj->GetButtonColor() + glm::u8vec4(0, 0, 0, 0), false);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), 
                            closeButtonObj->GetButtonColor() + glm::u8vec4(0, amount, amount, 0), false);
                        break;
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                        break;
                    case ssGUI::Enums::ButtonState::CLICKING:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), 
                            closeButtonObj->GetButtonColor() + glm::u8vec4(0, amount * 2, amount * 2, 0), false);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                        static_cast<ssGUI::Window*>(info.EventSource->GetParent())->Close();
                        break;
                    case ssGUI::Enums::ButtonState::NOT_INTERACTABLE:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), 
                            closeButtonObj->GetButtonColor() + glm::u8vec4(0, -amount, -amount, 0), false);
                        break;
                }
                
            }
        );

        //Update button's shape size when button's size is changed
        auto shapeEvent = closeButton->AddEventCallback(ssGUI::Enums::EventType::SIZE_CHANGED);
        shapeEvent->AddEventListener
        (
            ListenerKey, this,
            [circleId](ssGUI::EventInfo info)
            {
                auto shape = static_cast<ssGUI::Extensions::Shape*>(info.EventSource->GetExtension(ssGUI::Extensions::Shape::EXTENSION_NAME));
                shape->SetAdditionalCircle(circleId, glm::vec2(), info.EventSource->GetSize(), glm::u8vec4(255, 127, 127, 255), false);
            }
        );
        CloseButton = CurrentObjectsReferences.AddObjectReference(closeButton);

        //Add rounded corners to window
        auto rc = AddExtension<ssGUI::Extensions::RoundedCorners>();
        rc->ClearTargetShapes();
        rc->AddTargetVertex(0);
        rc->AddTargetVertex(1);
        rc->AddTargetVertex(2);
        rc->AddTargetVertex(3);
        rc->AddTargetVertex(4);
        rc->AddTargetVertex(5);

        //Make window dockable
        AddExtension<ssGUI::Extensions::Dockable>();
        
        //Add outline to window
        auto windowOutline = AddExtension<ssGUI::Extensions::Outline>();
        windowOutline->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));

        //Add shadow to window
        auto shadowEx = AddExtension<ssGUI::Extensions::BoxShadow>();
        shadowEx->SetBlurRadius(20);
        shadowEx->SetSizeOffset(glm::vec2(10, 10));
        RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);

        UpdateTitleText();
        UpdateIconImage();
        UpdateCloseButton();

        ssLOG_FUNC_EXIT();
    }
    
    ssGUI::ImageData* StandardWindow::DefaultIcon = nullptr;
    =================================================================
    */
    class StandardWindow : public Window
    {
        private:
            StandardWindow& operator=(StandardWindow const& other) = default;
        
        protected:
            int HorizontalPadding;              //See <GetHorizontalPadding>
            int VerticalPadding;                //See <GetVerticalPadding>
            bool AdaptiveTitleColor;            //See <SetAdaptiveTitleColor>
            glm::ivec4 TitleColorDifference;    //See <GetAdaptiveTitleColorDifference>
            bool AdaptiveTitleContrast;         //See <IsAdaptiveTitleContrast>
            bool AutoFontSize;                  //See <IsAutoFontSize>
            float FontSizeMultiplier;           //See <GetAutoFontSizeMultiplier>
            
            ssGUIObjectIndex WindowTitle;       //See <GetWindowTitleObject>
            ssGUIObjectIndex WindowIcon;        //See <GetWindowIconGUIObject>
            ssGUIObjectIndex CloseButton;       //See <GetCloseButtonObject>

            static ssGUI::ImageData* DefaultIcon;   //(Internal variable) Default window icon image

            StandardWindow(StandardWindow const& other);

            virtual void UpdateTitleText();
            virtual void UpdateIconImage();
            virtual void UpdateCloseButton();

        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            StandardWindow();
            virtual ~StandardWindow() override;

            /*function: SetWindowTitleObject
            Sets the window title text object.
            You can use <GUIObject::SetUserCreated> and <GUIObject::SetHeapAllocated> to allow 
            Standard Window to manage the lifetime of the text object instead.*/ 
            virtual void SetWindowTitleObject(ssGUI::Text* text);

            //function: GetWindowTitleObject
            //Returns the pointer to the window title text object. Nullptr if it doesn't exist
            virtual ssGUI::Text* GetWindowTitleObject() const;

            //function: SetAutoFontSize
            //Set is the font size controlled automatically by the titlebar height
            virtual void SetAutoFontSize(bool autoSize);

            //function: IsAutoFontSize
            //Returns is the font size controlled automatically by the titlebar height
            virtual bool IsAutoFontSize() const;

            //function: SetAutoFontSizeMultiplier
            //Set the font size multiplier to have a finer adjustment over it
            virtual void SetAutoFontSizeMultiplier(float multiplier);

            //function: GetAutoFontSizeMultiplier
            //Gets the font size multiplier to have a finer adjustment over it
            virtual float GetAutoFontSizeMultiplier() const;

            /*function: SetWindowIconGUIObject
            Sets the window icon image object.
            You can use <GUIObject::SetUserCreated> and <GUIObject::SetHeapAllocated> to allow 
            Standard Window to manage the lifetime of the text object instead.*/
            virtual void SetWindowIconGUIObject(ssGUI::Image* image);

            //function: GetWindowIconGUIObject
            //Returns the pointer to the window icon image object. Nullptr if it doesn't exist
            virtual ssGUI::Image* GetWindowIconGUIObject() const;
            
            //TODO: Add a way to set default window icon

            /*function: SetCloseButtonObject
            Sets the close button object.
            You can use <GUIObject::SetUserCreated> and <GUIObject::SetHeapAllocated> to allow 
            Standard Window to manage the lifetime of the text object instead.*/
            virtual void SetCloseButtonObject(ssGUI::Button* button);

            //function: GetCloseButtonObject
            //Returns the pointer to the close button object. Nullptr if it doesn't exist
            virtual ssGUI::Button* GetCloseButtonObject() const;

            //function: SetHorizontalPadding
            //Sets the horizontal padding for the icon, title and close button objects, in pixels
            virtual void SetHorizontalPadding(int padding);

            //function: GetHorizontalPadding
            //Returns the horizontal padding for the icon, title and close button objects, in pixels
            virtual int GetHorizontalPadding() const;

            //function: SetVerticalPadding
            //Sets the vertical padding for the icon, title and close button objects, in pixels
            virtual void SetVerticalPadding(int padding);

            //function: GetVerticalPadding
            //Returns the vertical padding for the icon, title and close button objects, in pixels
            virtual int GetVerticalPadding() const;

            //function: SetAdaptiveTitleColor
            //Sets if the title text color "adapts" to the titlebar color dynamically.
            //You need to call this function after setting the title text color to update the color difference stored here.
            virtual void SetAdaptiveTitleColor(bool adaptive);

            //function: SetAdaptiveTitleColor
            //Returns if the title text color "adapts" to the titlebar color dynamically
            virtual bool IsAdaptiveTitleColor() const;

            //function: SetAdaptiveTitleContrast
            //Sets if the title text color is opposite to the titlebar color
            virtual void SetAdaptiveTitleContrast(bool contrast);

            //function: IsAdaptiveTitleContrast
            //Returns if the title text color is opposite to the titlebar color
            virtual bool IsAdaptiveTitleContrast() const;

            //function: SetAdaptiveTitleColorDifference
            //Sets the title text color difference to titlebar color manually (TitleTextColor-TitlebarColor)
            //This is normally set automatically when you call <SetAdaptiveTitleColor>
            virtual void SetAdaptiveTitleColorDifference(glm::ivec4 difference);

            //function: GetAdaptiveTitleColorDifference
            //Gets the title text color difference to titlebar color (TitleTextColor-TitlebarColor)
            virtual glm::ivec4 GetAdaptiveTitleColorDifference() const;

            //function: SetTitlebarColor
            //See <Window::SetTitlebarColor>
            virtual void SetTitlebarColor(glm::u8vec4 color) override;

            //function: SetTitlebar
            //See <Window::SetTitlebar>
            virtual void SetTitlebar(bool set) override;

            //function: SetClosable
            //See <Window::SetClosable>
            virtual void SetClosable(bool closable) override;

            //function: GetType
            //See <GUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Window::Clone>
            virtual StandardWindow* Clone(bool cloneChildren) override;
            
            //function: InitiateDefaultResources
            //See <GUIObject::InitiateDefaultResources>
            virtual void InitiateDefaultResources() override;
            
            //function: CleanUpDefaultIcon
            //Deallocates default default window icon. This is handled automatically in <ssGUIManager>
            static void CleanUpDefaultIcon();
    };
}


#endif
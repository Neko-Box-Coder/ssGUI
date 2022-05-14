#ifndef SSGUI_STANDARD_WINDOW
#define SSGUI_STANDARD_WINDOW

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
        int HorizontalPadding;
        int VerticalPadding;
        bool AdaptiveTitleColor;
        glm::ivec4 TitleColorDifference;
        bool AdaptiveTitleContrast;
        
        ssGUIObjectIndex WindowTitle;
        ssGUIObjectIndex WindowIcon;
        ssGUIObjectIndex CloseButton;
    =================================================================
    ============================== C++ ==============================
    StandardWindow::StandardWindow() : HorizontalPadding(5), VerticalPadding(4), AdaptiveTitleColor(true), TitleColorDifference(0, 0, 0, 0), AdaptiveTitleContrast(true), 
                                        AutoFontSize(true), FontSizeMultiplier(0.8), WindowTitle(-1), WindowIcon(-1), CloseButton(-1)
    {        
        FUNC_DEBUG_ENTRY();
        SetMinSize(glm::vec2(100, 100));
        
        //Setup title
        auto windowTitle = new ssGUI::Text();
        windowTitle->SetUserCreated(false);
        windowTitle->SetHeapAllocated(true);
        windowTitle->SetParent(this);
        windowTitle->SetMinSize(glm::vec2(5, 5));
        windowTitle->SetText("Window");
        windowTitle->SetTextColor(glm::u8vec4(255, 255, 255, 255));
        WindowTitle = CurrentObjectsReferences.AddObjectReference(windowTitle);
        SetAdaptiveTitleColor(true);    //Setting it here so that eventcallback is added
        SetAdaptiveTitleColorDifference(glm::ivec4(255, 255, 255, 0));

        auto windowIcon = new ssGUI::Image();
        windowIcon->SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);
        windowIcon->SetUserCreated(false);
        windowIcon->SetHeapAllocated(true);
        windowIcon->SetParent(this);
        windowIcon->SetMinSize(glm::vec2(5, 5));
        WindowIcon = CurrentObjectsReferences.AddObjectReference(windowIcon);

        auto data = ssGUI::Factory::Create<ssGUI::ImageData>();
        data->LoadFromPath("Resources/WindowIcon.png");
        windowIcon->SetImageData(data);

        //Setup button
        auto closeButton = new ssGUI::Button();
        closeButton->SetUserCreated(false);
        closeButton->SetHeapAllocated(true);
        closeButton->SetParent(this);
        closeButton->SetMinSize(glm::vec2(5, 5));
        closeButton->RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);

        //Change button shape to circle
        auto shapeEx = ssGUI::Factory::Create<ssGUI::Extensions::Shape>();
        shapeEx->RemoveGUIObjectShape(0);
        int circleId = shapeEx->AddAdditionalCircle(glm::vec2(), closeButton->GetSize(), glm::u8vec4(255, 127, 127, 255), false);
        closeButton->AddExtension(shapeEx);

        //Add outline to button
        auto closeButtonOutline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        closeButtonOutline->SetOutlineThickness(4);
        closeButtonOutline->SetOutlineColor(glm::u8vec4(255, 127, 127, 255));
        closeButton->SetButtonColor(glm::u8vec4(255, 127, 127, 255));
        closeButton->AddExtension(closeButtonOutline);

        //Setup button event
        auto buttonEvent = closeButton->GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME);
        buttonEvent->RemoveEventListener(0);
        buttonEvent->AddEventListener
        (
            [circleId](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                auto closeButtonObj = static_cast<ssGUI::Button*>(src);
                auto shape = static_cast<ssGUI::Extensions::Shape*>(src->GetExtension(ssGUI::Extensions::Shape::EXTENSION_NAME));
                int amount = 60;
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
                        static_cast<ssGUI::Window*>(src->GetParent())->Close();
                        break;
                    case ssGUI::Enums::ButtonState::DISABLED:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), 
                            closeButtonObj->GetButtonColor() + glm::u8vec4(0, -amount, -amount, 0), false);
                        break;
                }
                
            }
        );

        //Update button's shape size when button's size is changed
        auto shapeEvent = ssGUI::Factory::Create<ssGUI::EventCallbacks::SizeChangedEventCallback>();
        shapeEvent->AddEventListener
        (
            [circleId](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                auto shape = static_cast<ssGUI::Extensions::Shape*>(src->GetExtension(ssGUI::Extensions::Shape::EXTENSION_NAME));
                shape->SetAdditionalCircle(circleId, glm::vec2(), src->GetSize(), glm::u8vec4(255, 127, 127, 255), false);
            }
        );
        closeButton->AddEventCallback(shapeEvent);
        CloseButton = CurrentObjectsReferences.AddObjectReference(closeButton);

        //Add rounded corners to window
        auto rc = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
        rc->ClearTargetShapes();
        rc->AddTargetVertex(0);
        rc->AddTargetVertex(1);
        rc->AddTargetVertex(2);
        rc->AddTargetVertex(3);
        rc->AddTargetVertex(4);
        rc->AddTargetVertex(5);
        AddExtension(rc);

        //Make window dockable
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Dockable>());
        
        //Add outline to window
        auto windowOutline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        windowOutline->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        AddExtension(windowOutline);

        //Add shadow to window
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>());
        RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);
        SetTitlebarHeight(26);

        //Clean up sub-components when this is deleted
        ssGUI::EventCallbacks::OnObjectDestroyEventCallback* callback = nullptr;
        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME))
        {
            callback = static_cast<ssGUI::EventCallbacks::OnObjectDestroyEventCallback*>
                (GetEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME));
        }
        else
        {
            callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::OnObjectDestroyEventCallback>();
            AddEventCallback(callback);
        }
        
        callback->AddEventListener(
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
            {
                auto standardWindow = static_cast<ssGUI::StandardWindow*>(container);
                auto windowTitleObj = standardWindow->GetWindowTitleObject();
                auto windowIconObj = standardWindow->GetWindowIconObject();
                auto closeButtonObj = standardWindow->GetCloseButtonObject();

                if(windowTitleObj != nullptr && windowTitleObj->GetParent() != container && !windowTitleObj->Internal_IsDeleted())
                    windowTitleObj->Delete();
                
                if(windowIconObj != nullptr && windowIconObj->GetParent() != container && !windowIconObj->Internal_IsDeleted())
                    windowIconObj->Delete();

                if(closeButtonObj != nullptr && closeButtonObj->GetParent() != container && !closeButtonObj->Internal_IsDeleted())
                    closeButtonObj->Delete();
            });

        UpdateTitleText();
        UpdateIconImage();
        UpdateCloseButton();

        FUNC_DEBUG_EXIT();
    }
    =================================================================
    */
    class StandardWindow : public Window
    {
        private:
            StandardWindow& operator=(StandardWindow const& other) = default;
        
        protected:
            int HorizontalPadding;
            int VerticalPadding;
            bool AdaptiveTitleColor;
            glm::ivec4 TitleColorDifference;
            bool AdaptiveTitleContrast;
            bool AutoFontSize;
            float FontSizeMultiplier;
            
            ssGUIObjectIndex WindowTitle;
            ssGUIObjectIndex WindowIcon;
            ssGUIObjectIndex CloseButton;

            StandardWindow(StandardWindow const& other);

            virtual void UpdateTitleText();
            virtual void UpdateIconImage();
            virtual void UpdateCloseButton();

        public:
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

            //function: SetAutoFontSizeMultiplier
            //Gets the font size multiplier to have a finer adjustment over it
            virtual float GetAutoFontSizeMultiplier() const;

            /*function: SetWindowIconObject
            Sets the window icon image object.
            You can use <GUIObject::SetUserCreated> and <GUIObject::SetHeapAllocated> to allow 
            Standard Window to manage the lifetime of the text object instead.*/
            virtual void SetWindowIconObject(ssGUI::Image* image);

            //function: GetWindowIconObject
            //Returns the pointer to the window icon image object. Nullptr if it doesn't exist
            virtual ssGUI::Image* GetWindowIconObject() const;

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
            virtual StandardWindow* Clone(bool cloneChildren) override;
    };
}


#endif
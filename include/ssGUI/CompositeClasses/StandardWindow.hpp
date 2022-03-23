#ifndef SSGUI_STANDARD_WINDOW
#define SSGUI_STANDARD_WINDOW

#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/BaseClasses/Window.hpp"
#include "ssGUI/ssGUITags.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class Text;
    class Button;
    class Image;

    /*class: StandardWindow
    A standard window that contains window icon, title and close button. With addtional visual and funcational extensions added.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
            int HorizontalPadding;
            int VerticalPadding;

            ssGUIObjectIndex WindowTitle;
            ssGUIObjectIndex WindowIcon;
            ssGUIObjectIndex CloseButton;
    =================================================================
    ============================== C++ ==============================
    StandardWindow::StandardWindow() : HorizontalPadding(5), VerticalPadding(5), WindowTitle(-1), WindowIcon(-1), CloseButton(-1)
    {        
        WindowTitle = CurrentObjectsReferences.AddObjectReference(new ssGUI::Text());
        CurrentObjectsReferences.GetObjectReference(WindowTitle)->SetUserCreated(false);
        CurrentObjectsReferences.GetObjectReference(WindowTitle)->SetParent(this);
        CurrentObjectsReferences.GetObjectReference(WindowTitle)->SetMinSize(glm::vec2(5, 5));

        // WindowIcon = CurrentObjectsReferences.AddObjectReference(new ssGUI::Image());
        // CurrentObjectsReferences.GetObjectReference(WindowIcon)->SetUserCreated(false);
        // CurrentObjectsReferences.GetObjectReference(WindowIcon)->SetParent(this);
        // CurrentObjectsReferences.GetObjectReference(WindowIcon)->SetMinSize(glm::vec2(5, 5));

        // CloseButton = CurrentObjectsReferences.AddObjectReference(new ssGUI::Button());
        // CurrentObjectsReferences.GetObjectReference(CloseButton)->SetUserCreated(false);
        // CurrentObjectsReferences.GetObjectReference(CloseButton)->SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));
        // CurrentObjectsReferences.GetObjectReference(CloseButton)->SetParent(this);
        // CurrentObjectsReferences.GetObjectReference(CloseButton)->SetMinSize(glm::vec2(5, 5));

        auto rc = new ssGUI::Extensions::RoundedCorners();
        rc->ClearTargetShapes();
        rc->AddTargetVertex(0);
        rc->AddTargetVertex(1);
        rc->AddTargetVertex(2);
        rc->AddTargetVertex(3);
        rc->AddTargetVertex(4);
        rc->AddTargetVertex(5);

        AddExtension(rc);

        RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);
        SetTitlebarHeight(25);

        ssGUI::EventCallbacks::OnObjectDestroyEventCallback* callback = new ssGUI::EventCallbacks::OnObjectDestroyEventCallback();
        callback->AddEventListener(
            [](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
            {
                auto standardWindow = static_cast<ssGUI::StandardWindow*>(container);
                auto windowTitleObj = standardWindow->GetWindowTitleObject();
                auto windowIconObj = standardWindow->GetWindowIconObject();
                auto closeButtonObj = standardWindow->GetCloseButtonObject();

                if(windowTitleObj != nullptr && windowTitleObj->GetParent() == container && !windowTitleObj->Internal_IsDeleted())
                    windowTitleObj->Delete();
                
                if(windowIconObj != nullptr && windowIconObj->GetParent() == container && !windowIconObj->Internal_IsDeleted())
                    windowIconObj->Delete();

                if(closeButtonObj != nullptr && closeButtonObj->GetParent() == container && !closeButtonObj->Internal_IsDeleted())
                    closeButtonObj->Delete();

            });

        UpdateTitleText();
        UpdateIconImage();
        UpdateCloseButton();
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

            //function: SetTitlebar
            //See <Window::SetTitlebar>
            virtual void SetTitlebar(bool set) override;

            //function: SetClosable
            //See <Window::SetClosable>
            virtual void SetClosable(bool closable) override;

            //function: GetType
            //See <GUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Delete
            //See <GUIObject::Delete>
            virtual void Delete() override;

            //function: Internal_Draw
            //virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            //virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Clone
            virtual GUIObject* Clone(bool cloneChildren) override;
    };
}


#endif
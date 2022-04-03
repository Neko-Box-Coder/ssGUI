#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"

#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Image.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/Outline.hpp"


namespace ssGUI
{
    StandardWindow::StandardWindow(StandardWindow const& other) : Window(other)
    {
        HorizontalPadding = other.GetHorizontalPadding();
        VerticalPadding = other.GetVerticalPadding();
        WindowTitle = other.WindowTitle;
        WindowIcon = other.WindowIcon;
        CloseButton = other.CloseButton;
    }

    void StandardWindow::UpdateTitleText()
    {
        auto windowTitleObj = CurrentObjectsReferences.GetObjectReference(WindowTitle);
        if(windowTitleObj == nullptr)
            return;
        
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;

        if(!windowTitleObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            windowTitleObj->AddExtension(new ssGUI::Extensions::AdvancedPosition());
        
        if(!windowTitleObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            windowTitleObj->AddExtension(new ssGUI::Extensions::AdvancedSize());

        ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(windowTitleObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        as = static_cast<ssGUI::Extensions::AdvancedSize*>(windowTitleObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalUsePercentage(true);
        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetHorizontalPercentage(0);
        ap->SetVerticalUsePercentage(false);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
        ap->SetVerticalPixel(-GetTitlebarHeight() + GetVerticalPadding());

        as->SetHorizontalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalUsePercentage(false);
        int textHeight = GetTitlebarHeight() - GetVerticalPadding() * 2;
        as->SetVerticalPixel(textHeight);

        static_cast<ssGUI::Text*>(windowTitleObj)->SetFontSize(textHeight);
        static_cast<ssGUI::Text*>(windowTitleObj)->SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
        static_cast<ssGUI::Text*>(windowTitleObj)->SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);

        if(!windowTitleObj->HasTag(ssGUI::Tags::OVERLAY))
            windowTitleObj->AddTag(ssGUI::Tags::OVERLAY);
    }

    void StandardWindow::UpdateIconImage()
    {
        auto windowIconObj = CurrentObjectsReferences.GetObjectReference(WindowIcon);
        
        if(windowIconObj == nullptr)
            return;
        
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!windowIconObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            windowIconObj->AddExtension(new ssGUI::Extensions::AdvancedPosition());
        
        if(!windowIconObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            windowIconObj->AddExtension(new ssGUI::Extensions::AdvancedSize());

        ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(windowIconObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        as = static_cast<ssGUI::Extensions::AdvancedSize*>(windowIconObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalUsePercentage(false);
        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
        ap->SetHorizontalPixel(GetHorizontalPadding());
        ap->SetVerticalUsePercentage(false);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
        ap->SetVerticalPixel(-GetTitlebarHeight() + GetVerticalPadding());
        
        int iconHeight = GetTitlebarHeight() - GetVerticalPadding() * 2;
        as->SetHorizontalUsePercentage(false);
        as->SetHorizontalPixel(iconHeight);
        as->SetVerticalUsePercentage(false);
        as->SetVerticalPixel(iconHeight);

        if(!windowIconObj->HasTag(ssGUI::Tags::OVERLAY))
            windowIconObj->AddTag(ssGUI::Tags::OVERLAY);
    }

    void StandardWindow::UpdateCloseButton()
    {
        auto closeButtonObj = CurrentObjectsReferences.GetObjectReference(CloseButton);
        
        if(closeButtonObj == nullptr)
            return;
        
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            closeButtonObj->AddExtension(new ssGUI::Extensions::AdvancedPosition());
        
        if(!closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            closeButtonObj->AddExtension(new ssGUI::Extensions::AdvancedSize());

        ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        as = static_cast<ssGUI::Extensions::AdvancedSize*>(closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalUsePercentage(false);
        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
        ap->SetHorizontalPixel(GetHorizontalPadding());
        ap->SetVerticalUsePercentage(false);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
        ap->SetVerticalPixel(-GetTitlebarHeight() + GetVerticalPadding());
        
        int buttonHeight = GetTitlebarHeight() - GetVerticalPadding() * 2;
        as->SetHorizontalUsePercentage(false);
        as->SetHorizontalPixel(buttonHeight);
        as->SetVerticalUsePercentage(false);
        as->SetVerticalPixel(buttonHeight);

        if(!closeButtonObj->HasTag(ssGUI::Tags::OVERLAY))
            closeButtonObj->AddTag(ssGUI::Tags::OVERLAY);
    }

    StandardWindow::StandardWindow() : HorizontalPadding(5), VerticalPadding(5), WindowTitle(-1), WindowIcon(-1), CloseButton(-1)
    {        
        SetMinSize(glm::vec2(50, 50));
        
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
        AddExtension(new ssGUI::Extensions::Dockable());
        AddExtension(new ssGUI::Extensions::Outline());
        RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);
        SetTitlebarHeight(25);

        ssGUI::EventCallbacks::OnObjectDestroyEventCallback* callback = nullptr;
        if(IsEventCallbackExist(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME))
        {
            callback = static_cast<ssGUI::EventCallbacks::OnObjectDestroyEventCallback*>
                (GetEventCallback(ssGUI::EventCallbacks::OnObjectDestroyEventCallback::EVENT_NAME));
        }
        else
        {
            callback = new ssGUI::EventCallbacks::OnObjectDestroyEventCallback();
            AddEventCallback(callback);
        }
        
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

    StandardWindow::~StandardWindow()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        

        // if(!WindowTitle->IsUserCreated())
        //     WindowTitle->Delete(true);

        // if(!WindowIcon->IsUserCreated())
        //     WindowIcon->Delete(true);

        // if(!CloseButton->IsUserCreated())
        //     CloseButton->Delete(true);
    }

    void StandardWindow::SetWindowTitleObject(ssGUI::Text* text)
    {
        std::wstring oldTitle = L"";
        auto oldTitleObj = static_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(WindowTitle));
        if(oldTitleObj != nullptr)
        {
            oldTitle = oldTitleObj->GetText();
            if(!oldTitleObj->IsUserCreated())
                oldTitleObj->Delete();
            else
                oldTitleObj->SetParent(nullptr);
        }
        
        if(text == nullptr)
        {
            WindowTitle = -1;
            return;
        }

        text->SetParent(this);

        ssGUIObjectIndex newTextIndex = CurrentObjectsReferences.GetObjectIndex(text);

        if(newTextIndex != -1)
            WindowTitle = newTextIndex;
        else
            WindowTitle = CurrentObjectsReferences.AddObjectReference(text);

        text->SetText(oldTitle);

        UpdateTitleText();
    }

    ssGUI::Text* StandardWindow::GetWindowTitleObject() const
    {
        return static_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(WindowTitle));
    }

    void StandardWindow::SetWindowIconObject(ssGUI::Image* image)
    {
        auto oldIcon = CurrentObjectsReferences.GetObjectReference(WindowIcon);
        
        if(oldIcon != nullptr)
        {
            if(!oldIcon->IsUserCreated())
                oldIcon->Delete();
            else
                oldIcon->SetParent(nullptr);
        }
        
        if(image == nullptr)
        {
            WindowIcon = -1;
            return;
        }
        
        image->SetParent(this);

        ssGUIObjectIndex newIconIndex = CurrentObjectsReferences.GetObjectIndex(image);

        if(newIconIndex != -1)
            WindowIcon = newIconIndex;
        else
            WindowIcon = CurrentObjectsReferences.AddObjectReference(image);

        UpdateIconImage();
    }
    
    ssGUI::Image* StandardWindow::GetWindowIconObject() const
    {
        return static_cast<ssGUI::Image*>(CurrentObjectsReferences.GetObjectReference(WindowIcon));
    }
    
    void StandardWindow::SetCloseButtonObject(ssGUI::Button* button)
    {
        auto oldButton = CurrentObjectsReferences.GetObjectReference(CloseButton);
        if(oldButton != nullptr)
        {
            if(!oldButton->IsUserCreated())
                oldButton->Delete();
            else
                oldButton->SetParent(nullptr);
        }
        
        if(button == nullptr)
        {
            CloseButton = -1;
            return;
        }
        
        button->SetParent(this);

        ssGUIObjectIndex newButtonIndex = CurrentObjectsReferences.GetObjectIndex(button);

        if(newButtonIndex != -1)
            CloseButton = newButtonIndex;
        else
            CloseButton = CurrentObjectsReferences.AddObjectReference(button);

        UpdateCloseButton();
    }
    
    ssGUI::Button* StandardWindow::GetCloseButtonObject() const
    {
        return static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(CloseButton));
    }

    void StandardWindow::SetHorizontalPadding(int padding)
    {
        HorizontalPadding = padding;
        UpdateTitleText();
        UpdateIconImage();
        UpdateCloseButton();
    }

    int StandardWindow::GetHorizontalPadding() const
    {
        return HorizontalPadding;
    }

    void StandardWindow::SetVerticalPadding(int padding)
    {
        VerticalPadding = padding;
        UpdateTitleText();
        UpdateIconImage();
        UpdateCloseButton();
    }

    int StandardWindow::GetVerticalPadding() const
    {
        return VerticalPadding;
    }

    void StandardWindow::SetTitlebar(bool set)
    {
        auto windowTitleObj = CurrentObjectsReferences.GetObjectReference(WindowTitle);
        auto windowIconObj = CurrentObjectsReferences.GetObjectReference(WindowIcon);
        auto closeButtonObj = CurrentObjectsReferences.GetObjectReference(CloseButton);

        if(set)
        {
            if(windowTitleObj != nullptr)
                windowTitleObj->SetVisible(true);
            
            if(windowIconObj != nullptr)
                windowIconObj->SetVisible(true);
            
            if(closeButtonObj != nullptr)
            {
                if(IsClosable())
                    closeButtonObj->SetVisible(true);
                else
                    closeButtonObj->SetVisible(false);
            }
            
        }
        else
        {
            if(windowTitleObj != nullptr)
                windowTitleObj->SetVisible(false);
            
            if(windowIconObj != nullptr)
                windowIconObj->SetVisible(false);
            
            if(closeButtonObj != nullptr)
                closeButtonObj->SetVisible(false);
        }
        
        Window::SetTitlebar(set);
    }

    //function: SetClosable
    void StandardWindow::SetClosable(bool closable)
    {
        auto closeButtonObj = CurrentObjectsReferences.GetObjectReference(CloseButton);

        if(closeButtonObj == nullptr)
            return;

        if(closable)
        {
            if(HasTitlebar())
                closeButtonObj->SetVisible(true);
            else
                closeButtonObj->SetVisible(false);
        }
        else
            closeButtonObj->SetVisible(false);

        Window::SetClosable(closable);
    }
    

    //function: GetType
    ssGUI::Enums::GUIObjectType StandardWindow::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WINDOW | ssGUI::Enums::GUIObjectType::STANDARD_WINDOW;
    }
    
    /*
    void Window::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {        
        //If it is not visible, don't even update/draw it
        if(!IsVisible())
            return;
        
        for(auto extension : Extensions)
            extension.second->Internal_Update(true, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
        
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(mainWindow);
        glm::ivec2 mouseDelta = currentMousePos - MouseDownPosition;
        
        //Resize
        //On mouse down
        if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::LEFT) &&
            !globalInputStatus.MouseInputBlocked)
        {
            OnMouseDownUpdate(currentMousePos, globalInputStatus);
        }
        //When the user is resizing or dragging the window
        else if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::LEFT) && (ResizingLeft || ResizingRight || ResizingTop || ResizingBot || Dragging))
        {
            OnMouseDragOrResizeUpdate(globalInputStatus, mouseDelta, inputInterface);
        }
        //Otherwise show resize cursor if necessary 
        else
        {
            BlockMouseInputAndUpdateCursor(globalInputStatus, currentMousePos, inputInterface);
        }

        for(auto extension : Extensions)
            extension.second->Internal_Update(false, inputInterface, globalInputStatus, windowInputStatus, mainWindow);
    }*/

    void StandardWindow::Delete()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        ssGUI::Window::Delete();
    }

    //function: Clone
    GUIObject* StandardWindow::Clone(bool cloneChildren)
    {
        FUNC_DEBUG_ENTRY();
        StandardWindow* temp = new StandardWindow(*this);
        CloneExtensionsAndEventCallbacks(temp);

        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                FUNC_DEBUG_EXIT();
                return nullptr;
            }
        }

        FUNC_DEBUG_EXIT();
        return temp;
    }

}
#include "ssGUI/CompositeClasses/StandardWindow.hpp"

#include "ssGUI/BaseClasses/Text.hpp"
#include "ssGUI/BaseClasses/Image.hpp"
#include "ssGUI/BaseClasses/Button.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"



namespace ssGUI
{
    StandardWindow::StandardWindow(StandardWindow const& other)
    {
        HorizontalPadding = other.GetHorizontalPadding();
        VerticalPadding = other.GetVerticalPadding();
        WindowTitle = nullptr;
        WindowIcon = nullptr;
        CloseButton = nullptr;
    }

    void StandardWindow::UpdateTitleText()
    {
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!WindowTitle->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            WindowTitle->AddExtension(new ssGUI::Extensions::AdvancedPosition());
        
        if(!WindowTitle->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            WindowTitle->AddExtension(new ssGUI::Extensions::AdvancedSize());

        ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(WindowTitle->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        as = static_cast<ssGUI::Extensions::AdvancedSize*>(WindowTitle->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

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

        WindowTitle->SetFontSize(textHeight);
        WindowTitle->SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
        WindowTitle->SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
    }

    void StandardWindow::UpdateIconImage()
    {
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!WindowIcon->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            WindowIcon->AddExtension(new ssGUI::Extensions::AdvancedPosition());
        
        if(!WindowIcon->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            WindowIcon->AddExtension(new ssGUI::Extensions::AdvancedSize());

        ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(WindowIcon->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        as = static_cast<ssGUI::Extensions::AdvancedSize*>(WindowIcon->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

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
    }

    void StandardWindow::UpdateCloseButton()
    {
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!CloseButton->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            CloseButton->AddExtension(new ssGUI::Extensions::AdvancedPosition());
        
        if(!CloseButton->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            CloseButton->AddExtension(new ssGUI::Extensions::AdvancedSize());

        ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(CloseButton->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        as = static_cast<ssGUI::Extensions::AdvancedSize*>(CloseButton->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

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
    }

    StandardWindow::StandardWindow() : HorizontalPadding(5), VerticalPadding(5), WindowTitle(nullptr), WindowIcon(nullptr), CloseButton(nullptr)
    {
        ssGUI::Font* font = new ssGUI::Font();
        font->GetBackendFontInterface()->LoadFromPath("NotoSans-Regular.ttf");
        
        WindowTitle = new ssGUI::Text();
        WindowTitle->SetUserCreated(false);
        WindowTitle->SetFont(font);

        //Remove the font and the text object is destroyed as the font is generated
        ssGUI::EventCallbacks::OnObjectDestroyEventCallback* callback = new ssGUI::EventCallbacks::OnObjectDestroyEventCallback();
        callback->AddEventListener([font](ssGUI::GUIObject* obj){delete font;});

        WindowIcon = new ssGUI::Image();
        WindowIcon->SetUserCreated(false);

        CloseButton = new ssGUI::Button();
        CloseButton->SetUserCreated(false);
        CloseButton->SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));

        UpdateTitleText();
        UpdateIconImage();
        UpdateCloseButton();
    }

    StandardWindow::~StandardWindow()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        
        if(!WindowTitle->IsUserCreated())
            WindowTitle->Delete(true);

        if(!WindowIcon->IsUserCreated())
            WindowIcon->Delete(true);

        if(!CloseButton->IsUserCreated())
            CloseButton->Delete(true);
    }

    void StandardWindow::SetWindowTitleObject(ssGUI::Text* text)
    {
        std::wstring oldTitle = WindowTitle->GetText();
        
        if(!WindowTitle->IsUserCreated())
            WindowTitle->Delete(true);
        else
            WindowTitle->SetParent(nullptr);
        
        if(text->HasTag(ssGUI::Tags::OVERLAY))
            text->AddTag(ssGUI::Tags::OVERLAY);

        WindowTitle = text;
        WindowTitle->SetText(oldTitle);
    }

    ssGUI::Text* StandardWindow::GetWindowTitleObject() const
    {
        return WindowTitle;
    }

    void StandardWindow::SetWindowIconObject(ssGUI::Image* image)
    {
        if(!WindowIcon->IsUserCreated())
            WindowIcon->Delete(true);
        else
            WindowIcon->SetParent(nullptr);
        
        if(image->HasTag(ssGUI::Tags::OVERLAY))
            image->AddTag(ssGUI::Tags::OVERLAY);

        WindowIcon = image;
    }
    
    ssGUI::Image* StandardWindow::GetWindowIconObject() const
    {
        return WindowIcon;
    }
    
    void StandardWindow::SetCloseButtonObject(ssGUI::Button* button)
    {
        if(!CloseButton->IsUserCreated())
            CloseButton->Delete(true);
        else
            CloseButton->SetParent(nullptr);
        
        if(button->HasTag(ssGUI::Tags::OVERLAY))
            button->AddTag(ssGUI::Tags::OVERLAY);

        CloseButton = button;
    }
    
    ssGUI::Button* StandardWindow::GetCloseButtonObject() const
    {
        return CloseButton;
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
        if(set)
        {
            WindowTitle->SetVisible(true);
            WindowIcon->SetVisible(true);
            if(IsClosable())
                CloseButton->SetVisible(true);
            else
                CloseButton->SetVisible(false);
        }
        else
        {
            WindowTitle->SetVisible(false);
            WindowIcon->SetVisible(false);
            CloseButton->SetVisible(false);
        }
        
        Window::SetTitlebar(set);
    }

    //function: SetClosable
    void StandardWindow::SetClosable(bool closable)
    {
        if(closable)
        {
            if(HasTitlebar())
                CloseButton->SetVisible(true);
            else
                CloseButton->SetVisible(false);
        }
        else
            CloseButton->SetVisible(false);

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

    //function: Clone
    GUIObject* StandardWindow::Clone(bool cloneChildren)
    {
        StandardWindow* temp = new StandardWindow(*this);

        // for(auto extension : Extensions)
        // {
        //     if(!temp->IsExtensionExist(extension.second->GetExtensionName()))
        //         temp->AddExtension(extension.second->Clone(this));
        // }

        // for(auto eventCallback : EventCallbacks)
        // {
        //     std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();

        //     if(!temp->IsEventCallbackExist(eventCallback.second->GetEventCallbackName()))
        //         temp->AddEventCallback(eventCallback.second->Clone(this, originalObjs, tempVec));
        // }

        return temp;
    }

}
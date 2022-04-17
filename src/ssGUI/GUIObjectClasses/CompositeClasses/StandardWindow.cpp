#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"

#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/Image.hpp"
#include "ssGUI/GUIObjectClasses/Button.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"

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
            windowTitleObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());
        
        if(!windowTitleObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            windowTitleObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

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

        windowIconObj->SetBackgroundColor(glm::u8vec4(255, 255, 255, 0));
        static_cast<ssGUI::Text*>(windowIconObj)->SetBlockInput(false);

        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!windowIconObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            windowIconObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());
        
        if(!windowIconObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            windowIconObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

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
        
        auto imageData = static_cast<ssGUI::Image*>(windowIconObj)->GetImageData();

        if(imageData == nullptr || !imageData->IsValid())
            windowIconObj->SetVisible(false);
        else
            windowIconObj->SetVisible(true);
    }

    void StandardWindow::UpdateCloseButton()
    {
        auto closeButtonObj = static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(CloseButton));
        
        if(closeButtonObj == nullptr)
            return;
        
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            closeButtonObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());
        
        if(!closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            closeButtonObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

        ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        as = static_cast<ssGUI::Extensions::AdvancedSize*>(closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalUsePercentage(false);
        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
        ap->SetHorizontalPixel(GetHorizontalPadding());
        ap->SetVerticalUsePercentage(false);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
        ap->SetVerticalPixel(-GetTitlebarHeight() + GetVerticalPadding() + 2);
        
        int buttonHeight = GetTitlebarHeight() - GetVerticalPadding() * 2 - 4;
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
        
        auto windowTitle = new ssGUI::Text();
        windowTitle->SetUserCreated(false);
        windowTitle->SetHeapAllocated(true);
        windowTitle->SetParent(this);
        windowTitle->SetMinSize(glm::vec2(5, 5));
        windowTitle->SetText("Window");
        WindowTitle = CurrentObjectsReferences.AddObjectReference(windowTitle);

        // auto windowIcon = new ssGUI::Image();
        // windowIcon->SetUserCreated(false);
        // windowIcon->SetHeapAllocated(true);
        // windowIcon->SetParent(this);
        // windowIcon->SetMinSize(glm::vec2(5, 5));
        // WindowIcon = CurrentObjectsReferences.AddObjectReference(windowIcon);

        auto closeButton = new ssGUI::Button();
        closeButton->SetUserCreated(false);
        closeButton->SetHeapAllocated(true);
        closeButton->SetParent(this);
        closeButton->SetMinSize(glm::vec2(5, 5));
        closeButton->RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);
        auto shapeEx = ssGUI::Factory::Create<ssGUI::Extensions::Shape>();
        shapeEx->RemoveGUIObjectShape(0);
        int circleId = shapeEx->AddAdditionalCircle(glm::vec2(), closeButton->GetSize(), glm::u8vec4(255, 127, 127, 255), false);
        closeButton->AddExtension(shapeEx);

        auto closeButtonOutline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        closeButtonOutline->SetOutlineThickness(2);
        closeButtonOutline->SetOutlineColor(glm::u8vec4(255, 127, 127, 255));
        // closeButtonOutline->ClearTargetShapes();
        // closeButtonOutline->SetSimpleOutline(false);
        // closeButtonOutline->AddTargetShape
        closeButton->AddExtension(closeButtonOutline);

        auto buttonEvent = closeButton->GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME);
        buttonEvent->RemoveEventListener(0);
        buttonEvent->AddEventListener
        (
            [circleId](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                auto closeButtonObj = static_cast<ssGUI::Button*>(src);
                auto shape = static_cast<ssGUI::Extensions::Shape*>(src->GetExtension(ssGUI::Extensions::Shape::EXTENSION_NAME));
                switch(closeButtonObj->GetButtonState())
                {
                    case ssGUI::Enums::ButtonState::NORMAL:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), glm::u8vec4(255, 127, 127, 255), false);
                        break;
                    case ssGUI::Enums::ButtonState::HOVER:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), glm::u8vec4(255, 167, 167, 255), false);
                        break;
                    case ssGUI::Enums::ButtonState::ON_CLICK:
                        break;
                    case ssGUI::Enums::ButtonState::CLICKING:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), glm::u8vec4(255, 207, 207, 255), false);
                        break;
                    case ssGUI::Enums::ButtonState::CLICKED:
                        static_cast<ssGUI::Window*>(src->GetParent())->Close();
                        break;
                    case ssGUI::Enums::ButtonState::DISABLED:
                        shape->SetAdditionalCircle(circleId, glm::vec2(), closeButtonObj->GetSize(), glm::u8vec4(255, 107, 107, 255), false);
                        break;
                }
                
            }
        );
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

        auto rc = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
        rc->ClearTargetShapes();
        rc->AddTargetVertex(0);
        rc->AddTargetVertex(1);
        rc->AddTargetVertex(2);
        rc->AddTargetVertex(3);
        rc->AddTargetVertex(4);
        rc->AddTargetVertex(5);

        AddExtension(rc);
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Dockable>());
        auto windowOutline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
        windowOutline->SetOutlineColor(glm::u8vec4(0, 0, 0, 127));
        AddExtension(windowOutline);
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>());
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

    void StandardWindow::Delete()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
        ssGUI::Window::Delete();
    }

    //function: Clone
    StandardWindow* StandardWindow::Clone(bool cloneChildren)
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
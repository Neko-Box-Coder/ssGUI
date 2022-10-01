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
#include "ssGUI/EventCallbacks/OnObjectDestroyEventCallback.hpp"

namespace ssGUI
{
    StandardWindow::StandardWindow(StandardWindow const& other) : Window(other)
    {
        HorizontalPadding = other.GetHorizontalPadding();
        VerticalPadding = other.GetVerticalPadding();
        AdaptiveTitleColor = other.IsAdaptiveTitleColor();
        TitleColorDifference = other.GetAdaptiveTitleColorDifference();
        AdaptiveTitleContrast = other.IsAdaptiveTitleContrast();
        AutoFontSize = other.IsAutoFontSize();
        FontSizeMultiplier = other.GetAutoFontSizeMultiplier();

        WindowTitle = other.WindowTitle;
        WindowIcon = other.WindowIcon;
        CloseButton = other.CloseButton;
    }

    void StandardWindow::UpdateTitleText()
    {
        ssLOG_FUNC_ENTRY();
        auto windowTitleObj = dynamic_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(WindowTitle));
        if(windowTitleObj == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;

        if(!windowTitleObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            windowTitleObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());
        
        if(!windowTitleObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            windowTitleObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

        ap = windowTitleObj->GetAnyExtension<ssGUI::Extensions::AdvancedPosition>();
        as = windowTitleObj->GetAnyExtension<ssGUI::Extensions::AdvancedSize>();

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetHorizontalPercentage(0);
        ap->SetHorizontalPixel(0);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
        ap->SetVerticalPixel(-GetTitlebarHeight() + GetVerticalPadding());
        ap->SetVerticalPercentage(0);

        as->SetHorizontalPercentage(1);
        as->SetHorizontalPixel(0);
        int textHeight = GetTitlebarHeight() - GetVerticalPadding() * 2;
        as->SetVerticalPixel(textHeight);
        as->SetVerticalPercentage(0);

        if(AutoFontSize)
        {
            windowTitleObj->SetNewCharacterFontSize(textHeight * FontSizeMultiplier);
            windowTitleObj->ApplyNewCharacterSettingsToText();
        }
        windowTitleObj->SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
        windowTitleObj->SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);

        if(!windowTitleObj->HasTag(ssGUI::Tags::OVERLAY))
            windowTitleObj->AddTag(ssGUI::Tags::OVERLAY);
        
        ssLOG_FUNC_EXIT();
    }

    void StandardWindow::UpdateIconImage()
    {
        ssLOG_FUNC_ENTRY();
        auto windowIconObj = CurrentObjectsReferences.GetObjectReference(WindowIcon);
        
        if(windowIconObj == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

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

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
        ap->SetHorizontalPixel(GetHorizontalPadding());
        ap->SetHorizontalPercentage(0);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
        ap->SetVerticalPixel(-GetTitlebarHeight() + GetVerticalPadding());
        ap->SetVerticalPercentage(0);
        
        int iconHeight = GetTitlebarHeight() - GetVerticalPadding() * 2;
        as->SetHorizontalPixel(iconHeight);
        as->SetHorizontalPercentage(0);
        as->SetVerticalPixel(iconHeight);
        as->SetVerticalPercentage(0);

        if(!windowIconObj->HasTag(ssGUI::Tags::OVERLAY))
            windowIconObj->AddTag(ssGUI::Tags::OVERLAY);
        
        auto imageData = static_cast<ssGUI::Image*>(windowIconObj)->GetImageData();

        if(imageData == nullptr || !imageData->IsValid())
            windowIconObj->SetEnabled(false);
        else
            windowIconObj->SetEnabled(true);
        
        ssLOG_FUNC_EXIT();
    }

    void StandardWindow::UpdateCloseButton()
    {
        ssLOG_FUNC_ENTRY();
        auto closeButtonObj = static_cast<ssGUI::Button*>(CurrentObjectsReferences.GetObjectReference(CloseButton));
        
        if(closeButtonObj == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        ssGUI::Extensions::AdvancedPosition* ap;
        ssGUI::Extensions::AdvancedSize* as;
        
        if(!closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME))
            closeButtonObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());
        
        if(!closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME))
            closeButtonObj->AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>());

        ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        as = static_cast<ssGUI::Extensions::AdvancedSize*>(closeButtonObj->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
        ap->SetHorizontalPixel(GetHorizontalPadding());
        ap->SetHorizontalPercentage(0);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
        ap->SetVerticalPixel(-GetTitlebarHeight() + GetVerticalPadding() + 2);
        ap->SetVerticalPercentage(0);

        int buttonHeight = GetTitlebarHeight() - GetVerticalPadding() * 2 - 4;
        as->SetHorizontalPixel(buttonHeight);
        as->SetHorizontalPercentage(0);
        as->SetVerticalPixel(buttonHeight);
        as->SetVerticalPercentage(0);

        if(!closeButtonObj->HasTag(ssGUI::Tags::OVERLAY))
            closeButtonObj->AddTag(ssGUI::Tags::OVERLAY);
        
        ssLOG_FUNC_EXIT();
    }

    const std::string StandardWindow::ListenerKey = "Standard Window";

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
        windowTitle->SetParent(this);
        windowTitle->SetMinSize(glm::vec2(5, 5));
        windowTitle->SetNewCharacterColor(glm::u8vec4(255, 255, 255, 255));
        windowTitle->SetText("Window");
        WindowTitle = CurrentObjectsReferences.AddObjectReference(windowTitle);
        SetAdaptiveTitleColor(true);    //Setting it here so that eventcallback is added
        SetAdaptiveTitleColorDifference(glm::ivec4(150, 150, 150, 0));

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
        buttonEvent->RemoveEventListener(ssGUI::Button::ListenerKey, closeButton);
        buttonEvent->AddEventListener
        (
            ListenerKey, this,
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
            ListenerKey, this,
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
        auto shadowEx = ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>();
        shadowEx->SetBlurRadius(20);
        shadowEx->SetSizeOffset(glm::vec2(10, 10));
        AddExtension(shadowEx);
        RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);

        UpdateTitleText();
        UpdateIconImage();
        UpdateCloseButton();

        ssLOG_FUNC_EXIT();
    }

    StandardWindow::~StandardWindow()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();

        //If the object deallocation is not handled by ssGUIManager
        if(!Internal_IsDeleted())
            Internal_ManualDeletion(std::vector<ssGUI::ssGUIObjectIndex>{WindowTitle, WindowIcon, CloseButton});
    }

    void StandardWindow::SetWindowTitleObject(ssGUI::Text* text)
    {
        std::wstring oldTitle = L"";
        auto oldTitleObj = dynamic_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(WindowTitle));
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

        SetAdaptiveTitleColor(IsAdaptiveTitleColor());      //resetting it here so that eventcallback is added
        UpdateTitleText();
    }

    ssGUI::Text* StandardWindow::GetWindowTitleObject() const
    {
        return static_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(WindowTitle));
    }

    void StandardWindow::SetAutoFontSize(bool autoSize)
    {
        AutoFontSize = autoSize;
    }

    bool StandardWindow::IsAutoFontSize() const
    {
        return AutoFontSize;
    }

    void StandardWindow::SetAutoFontSizeMultiplier(float multiplier)
    {
        FontSizeMultiplier = multiplier;
    }

    float StandardWindow::GetAutoFontSizeMultiplier() const
    {
        return FontSizeMultiplier;
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

    void StandardWindow::SetAdaptiveTitleColor(bool adaptive)
    {
        if(CurrentObjectsReferences.GetObjectReference(WindowTitle) == nullptr)
            return;

        AdaptiveTitleColor = adaptive;
        auto titleObj = dynamic_cast<ssGUI::Text*>(CurrentObjectsReferences.GetObjectReference(WindowTitle));
        SetAdaptiveTitleColorDifference(titleObj->GetNewCharacterColor() - GetTitlebarColor());

        SetTitlebarColor(GetTitlebarColor());   //Setting the titlebar color to trigger the event callback
    }

    bool StandardWindow::IsAdaptiveTitleColor() const
    {
        return AdaptiveTitleColor;
    }

    void StandardWindow::SetAdaptiveTitleContrast(bool contrast)
    {
        AdaptiveTitleContrast = contrast;

        if(CurrentObjectsReferences.GetObjectReference(WindowTitle) == nullptr)
            return;

        SetTitlebarColor(GetTitlebarColor());   //Setting the titlebar color to trigger the event callback
    }

    bool StandardWindow::IsAdaptiveTitleContrast() const
    {
        return AdaptiveTitleContrast;
    }

    void StandardWindow::SetAdaptiveTitleColorDifference(glm::ivec4 difference)
    {
        TitleColorDifference = difference;
    }

    glm::ivec4 StandardWindow::GetAdaptiveTitleColorDifference() const
    {
        return TitleColorDifference;
    }

    void StandardWindow::SetTitlebarColor(glm::u8vec4 color)
    {
        Window::SetTitlebarColor(color);
                    
        if(GetWindowTitleObject() == nullptr)
            return;

        if(!IsAdaptiveTitleColor())
        {
            GetWindowTitleObject()->RedrawObject();
            return;    
        }
        
        glm::ivec4 titleResult;
        if(IsAdaptiveTitleContrast())
        {
            float averageTitlebarColor = (GetTitlebarColor().r + GetTitlebarColor().g + GetTitlebarColor().b)/3;
            float averageTitleDiffColor = (GetAdaptiveTitleColorDifference().r + GetAdaptiveTitleColorDifference().g + GetAdaptiveTitleColorDifference().b)/3;
            int contrastFactor = averageTitlebarColor + averageTitleDiffColor > 255 ? -1 : 1; 
            titleResult = (glm::ivec4)GetTitlebarColor() + GetAdaptiveTitleColorDifference() * contrastFactor;
        }
        else
            titleResult = (glm::ivec4)GetTitlebarColor() + GetAdaptiveTitleColorDifference();

        titleResult.r = titleResult.r < 0 ? 0 : titleResult.r;
        titleResult.r = titleResult.r > 255 ? 255 : titleResult.r;
        titleResult.g = titleResult.g < 0 ? 0 : titleResult.g;
        titleResult.g = titleResult.g > 255 ? 255 : titleResult.g;
        titleResult.b = titleResult.b > 255 ? 255 : titleResult.b;
        titleResult.b = titleResult.b < 0 ? 0 : titleResult.b;
        titleResult.a = titleResult.a < 0 ? 0 : titleResult.a;
        titleResult.a = titleResult.a > 255 ? 255 : titleResult.a;

        GetWindowTitleObject()->SetNewCharacterColor((glm::u8vec4)titleResult);
        GetWindowTitleObject()->ApplyNewCharacterSettingsToText();
    }

    void StandardWindow::SetTitlebar(bool set)
    {
        auto windowTitleObj = CurrentObjectsReferences.GetObjectReference(WindowTitle);
        auto windowIconObj = CurrentObjectsReferences.GetObjectReference(WindowIcon);
        auto closeButtonObj = CurrentObjectsReferences.GetObjectReference(CloseButton);

        if(set)
        {
            if(windowTitleObj != nullptr)
                windowTitleObj->SetEnabled(true);
            
            if(windowIconObj != nullptr)
                windowIconObj->SetEnabled(true);
            
            if(closeButtonObj != nullptr)
            {
                if(IsClosable())
                    closeButtonObj->SetEnabled(true);
                else
                    closeButtonObj->SetEnabled(false);
            }
        }
        else
        {
            if(windowTitleObj != nullptr)
                windowTitleObj->SetEnabled(false);
            
            if(windowIconObj != nullptr)
                windowIconObj->SetEnabled(false);
            
            if(closeButtonObj != nullptr)
                closeButtonObj->SetEnabled(false);
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
                closeButtonObj->SetEnabled(true);
            else
                closeButtonObj->SetEnabled(false);
        }
        else
            closeButtonObj->SetEnabled(false);

        Window::SetClosable(closable);
    }

    //function: GetType
    ssGUI::Enums::GUIObjectType StandardWindow::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WINDOW | ssGUI::Enums::GUIObjectType::STANDARD_WINDOW;
    }

    //function: Clone
    StandardWindow* StandardWindow::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        StandardWindow* temp = new StandardWindow(*this);
        CloneExtensionsAndEventCallbacks(temp);

        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                ssLOG_FUNC_EXIT();
                return nullptr;
            }
        }

        ssLOG_FUNC_EXIT();
        return temp;
    }

}
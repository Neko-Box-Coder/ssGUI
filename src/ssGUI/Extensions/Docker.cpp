#include "ssGUI/Extensions/Docker.hpp"


#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"


namespace ssGUI::Extensions
{
    Docker::Docker(Docker const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        ChildrenDockerUseThisSettings = other.IsChildrenDockerUseThisSettings();

        FloatingDockerColor = other.GetFloatingDockerColor();                
        UseTriggerPercentage = other.IsUseTriggerPercentage();
        TriggerHorizontalPercentage = other.GetTriggerHorizontalPercentage();
        TriggerVerticalPercentage = other.GetTriggerVerticalPercentage();
        TriggerHorizontalPixel = other.GetTriggerHorizontalPixel();
        TriggerVerticalPixel = other.GetTriggerVerticalPixel();
        TriggerAreaColor = other.GetTriggerAreaColor();
        DockPreviewColor = other.GetDockPreviewColor();

        DockPreivew = nullptr;
        DockTrigger = nullptr;
    }
    
    ssGUI::Extensions::Docker* Docker::DefaultGeneratedDockerSettings = nullptr;
    ssGUI::Extensions::Layout* Docker::DefaultGeneratedLayoutSettings = nullptr;
    const std::string Docker::EXTENSION_NAME = "Docker";

    void Docker::CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color)
    {
        FUNC_DEBUG_ENTRY();
        
        //If widget is not present, create it
        if((*widget) == nullptr)
        {            
            (*widget) = new ssGUI::Widget();
            (*widget)->SetUserCreated(false);
            (*widget)->SetHeapAllocated(true);
            static_cast<ssGUI::Widget*>((*widget))->SetInteractable(false);
            static_cast<ssGUI::Widget*>((*widget))->SetBlockInput(false);

            ssGUI::Extensions::AdvancedPosition* ap = new ssGUI::Extensions::AdvancedPosition();
            ssGUI::Extensions::AdvancedSize* as = new ssGUI::Extensions::AdvancedSize(); 

            (*widget)->AddExtension(ap);
            (*widget)->AddExtension(as);
        }

        (*widget)->AddTag(ssGUI::Tags::OVERLAY);
        (*widget)->SetParent(Container);
        (*widget)->SetBackgroundColour(color);
        
        FUNC_DEBUG_EXIT();
    }

    void Docker::DrawPreview()
    {
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockPreivew, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivew->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivew->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(1);

        FUNC_DEBUG_EXIT();
    }

    void Docker::DiscardPreview()
    {
        if(DockPreivew != nullptr)
        {
            DockPreivew->Delete();
            DockPreivew = nullptr;
        }
    }

    void Docker::DrawTriggerArea()
    {
        FUNC_DEBUG_ENTRY();
        
        CreateWidgetIfNotPresent(&DockTrigger, GetTriggerAreaColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTrigger->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTrigger->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        as->SetHorizontalUsePercentage(UseTriggerPercentage);
        as->SetVerticalUsePercentage(UseTriggerPercentage);

        if(UseTriggerPercentage)
        {
            ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            as->SetHorizontalPercentage(GetTriggerHorizontalPercentage());
            as->SetVerticalPercentage(GetTriggerVerticalPercentage());
        }
        else
        {
            ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            as->SetHorizontalPixel(GetTriggerHorizontalPixel());
            as->SetVerticalPixel(GetTriggerVerticalPixel());
        }

        FUNC_DEBUG_EXIT();
    }

    void Docker::DiscardTriggerArea()
    {
        if(DockTrigger != nullptr)
        {
            DockTrigger->Delete();
            DockTrigger = nullptr;
        }
    }


    Docker::Docker() : Container(nullptr), Enabled(true), ChildrenDockerUseThisSettings(true), FloatingDockerColor(glm::u8vec4(127, 127, 127, 255)), UseTriggerPercentage(true),
                        TriggerHorizontalPercentage(0.5), TriggerVerticalPercentage(0.5), TriggerHorizontalPixel(15), TriggerVerticalPixel(15),
                        TriggerAreaColor(glm::u8vec4(87, 207, 255, 127)), DockPreviewColor(glm::u8vec4(255, 255, 255, 127)), DockPreivew(nullptr),
                        DockTrigger(nullptr)
    {}

    Docker::~Docker()
    {}
    
    void Docker::SetDefaultGeneratedDockerSettings(ssGUI::Extensions::Docker* defaultDocker)
    {
        DefaultGeneratedDockerSettings = defaultDocker;
    }

    ssGUI::Extensions::Docker* Docker::GetDefaultGeneratedDockerSettings()
    {
        return DefaultGeneratedDockerSettings;
    }

    void Docker::SetDefaultGeneratedLayoutSettings(ssGUI::Extensions::Layout* defaultLayout)
    {
        DefaultGeneratedLayoutSettings = defaultLayout;
    }

    ssGUI::Extensions::Layout* Docker::GetDefaultGeneratedLayoutSettings()
    {
        return DefaultGeneratedLayoutSettings;
    }

    void Docker::SetFloatingDockerColor(glm::u8vec4 color)
    {
        FloatingDockerColor = color;
    }

    glm::u8vec4 Docker::GetFloatingDockerColor() const
    {
        return FloatingDockerColor;
    }

    void Docker::SetChildrenDockerUseThisSettings(bool use)
    {
        ChildrenDockerUseThisSettings = use;
    }

    bool Docker::IsChildrenDockerUseThisSettings() const
    {
        return ChildrenDockerUseThisSettings;
    }

    void Docker::SetUseTriggerPercentage(bool use)
    {
        UseTriggerPercentage = use;
    }

    bool Docker::IsUseTriggerPercentage() const
    {
        return UseTriggerPercentage;
    }

    void Docker::SetTriggerHorizontalPercentage(float percent)
    {
        TriggerHorizontalPercentage = percent;
    }

    float Docker::GetTriggerHorizontalPercentage() const
    {
        return TriggerHorizontalPercentage;
    }

    void Docker::SetTriggerVerticalPercentage(float percent)
    {
        TriggerVerticalPercentage = percent;
    }

    float Docker::GetTriggerVerticalPercentage() const
    {
        return TriggerVerticalPercentage;
    }

    void Docker::SetTriggerHorizontalPixel(int pixel)
    {
        TriggerHorizontalPixel = pixel;
    }

    int Docker::GetTriggerHorizontalPixel() const
    {
        return TriggerHorizontalPixel;
    }

    void Docker::SetTriggerVerticalPixel(int pixel)
    {
        TriggerVerticalPixel = pixel;
    }

    int Docker::GetTriggerVerticalPixel() const
    {
        return TriggerVerticalPixel;
    }

    void Docker::SetTriggerAreaColor(glm::u8vec4 color)
    {
        TriggerAreaColor = color;
    }

    glm::u8vec4 Docker::GetTriggerAreaColor() const
    {
        return TriggerAreaColor;
    }

    void Docker::SetDockPreviewColor(glm::u8vec4 color)
    {
        DockPreviewColor = color;
    }

    glm::u8vec4 Docker::GetDockPreviewColor() const
    {
        return DockPreviewColor;
    }

    void Docker::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Docker::IsEnabled() const
    {
        return Enabled;
    }

    //Extension methods
    void Docker::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();
        
        if(!IsPreUpdate || Container == nullptr || !Enabled)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        //Check if Layout extension exists
        if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        int baseChildCount = 0;
        if(DockPreivew != nullptr)
            baseChildCount++;
        if(DockTrigger != nullptr)
            baseChildCount++;

        //If there are no other children, tell layout to exclude trigger and preview
        if(Container->GetChildrenCount() > baseChildCount)
        {
            //static_cast<ssGUI::Extensions::Layout*>(Container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetEnabled(true);
            DiscardPreview();
            DiscardTriggerArea();
            FUNC_DEBUG_EXIT();
            return;
        }

        //If global dock mode is true, check the cursor against the trigger area
        if(Dockable::GlobalDockMode && !globalInputStatus.DockingBlocked)
        {
            glm::ivec2 containerPos = Container->GetGlobalPosition();
            glm::ivec2 containerSize = Container->GetSize();
            int titleBarOffset = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW ? 
                                    static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() : 0;
            containerSize.y -= titleBarOffset;
            
            glm::ivec2 triggerSize = IsUseTriggerPercentage() ? glm::ivec2(containerSize.x * GetTriggerHorizontalPercentage(), containerSize.y * GetTriggerVerticalPercentage()) : 
                                        glm::ivec2(GetTriggerHorizontalPixel(), GetTriggerVerticalPixel());

            bool previewDrawn = false;

            //Center
            if(inputInterface->GetCurrentMousePosition(mainWindow).x >= containerPos.x + (containerSize.x - triggerSize.x) * 0.5 && 
                inputInterface->GetCurrentMousePosition(mainWindow).x <= containerPos.x + (containerSize.x + triggerSize.x) * 0.5 &&
                inputInterface->GetCurrentMousePosition(mainWindow).y >= containerPos.y + titleBarOffset + (containerSize.y - triggerSize.y) * 0.5 && 
                inputInterface->GetCurrentMousePosition(mainWindow).y <= containerPos.y + titleBarOffset + (containerSize.y + triggerSize.y) * 0.5)
            {
                DrawPreview();
                previewDrawn = true;
                Dockable::TargetDockSide = Dockable::DockSide::CENTER;
            }
            else
            {                
                DiscardPreview();
            }
            
            //Check if the cursor is inside the window
            if(inputInterface->GetCurrentMousePosition(mainWindow).x >= containerPos.x && inputInterface->GetCurrentMousePosition(mainWindow).x <= containerPos.x + containerSize.x &&
                inputInterface->GetCurrentMousePosition(mainWindow).y >= containerPos.y + titleBarOffset && inputInterface->GetCurrentMousePosition(mainWindow).y <= containerPos.y + containerSize.y)
            {
                if(!previewDrawn)
                {
                    Dockable::TargetDockSide = Dockable::DockSide::NONE;
                    DrawTriggerArea();
                }
                else
                    DiscardTriggerArea();
                    
                globalInputStatus.DockingBlocked = true;
                windowInputStatus.DockingBlocked = true;
                Dockable::TargetDockObject = Container;
            }
            else
            {
                    Dockable::TargetDockSide = Dockable::DockSide::NONE;
                
                DiscardTriggerArea();
            }
        }
        else
        {
            DiscardPreview();
            DiscardTriggerArea();
        }

        FUNC_DEBUG_EXIT();
    }

    void Docker::Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {
    }

    std::string Docker::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void Docker::BindToObject(ssGUI::GUIObject* bindObj)
    {
        FUNC_DEBUG_ENTRY();
        
        Container = bindObj;
        DEBUG_LINE();
        ssGUI::GUIObject* containerParent = Container->GetParent();
        DEBUG_LINE();

        //Check if we are using the parent's docker & layout settings or not
        if(containerParent != nullptr && containerParent->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME)
            && static_cast<ssGUI::Extensions::Docker*>(containerParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsChildrenDockerUseThisSettings()
            && static_cast<ssGUI::Extensions::Docker*>(containerParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsEnabled())
        {            
        DEBUG_LINE();
            
            ssGUI::Extensions::Docker* parentDocker = static_cast<ssGUI::Extensions::Docker*>(containerParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME));
            Copy(parentDocker);
        DEBUG_LINE();

            if(containerParent->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
            {
        DEBUG_LINE();
                
                ssGUI::Extensions::Layout* parentLayout = static_cast<ssGUI::Extensions::Layout*>(containerParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));
                if(Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                {
        DEBUG_LINE();
                    static_cast<ssGUI::Extensions::Layout*>(Container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->Copy(parentLayout);
                }
                else
                {
        DEBUG_LINE("Container: "<<Container);
        DEBUG_LINE("Container parent: "<<containerParent);
                    parentLayout->Clone(Container);
                }
        DEBUG_LINE();
            
            }
            else if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
            {
        DEBUG_LINE();

                Container->AddExtension(new ssGUI::Extensions::Layout());
            }
        }
        //Otherwise we generate layout extension if there isn't one
        else
        {
        DEBUG_LINE();
            
            //Check if there's a layout extension for container
            if(!Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
                Container->AddExtension(new ssGUI::Extensions::Layout());
        }

        FUNC_DEBUG_EXIT();
    }

    void Docker::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::Docker* docker = static_cast<ssGUI::Extensions::Docker*>(extension);
        
        Enabled = docker->IsEnabled();
        ChildrenDockerUseThisSettings = docker->IsChildrenDockerUseThisSettings();

        FloatingDockerColor = docker->GetFloatingDockerColor();                
        UseTriggerPercentage = docker->IsUseTriggerPercentage();
        TriggerHorizontalPercentage = docker->GetTriggerHorizontalPercentage();
        TriggerVerticalPercentage = docker->GetTriggerVerticalPercentage();
        TriggerHorizontalPixel = docker->GetTriggerHorizontalPixel();
        TriggerVerticalPixel = docker->GetTriggerVerticalPixel();
        TriggerAreaColor = docker->GetTriggerAreaColor();
        DockPreviewColor = docker->GetDockPreviewColor();
    }

    ObjectsReferences* Docker::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Extension* Docker::Clone(ssGUI::GUIObject* newContainer)
    {
        Docker* temp = new Docker(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}
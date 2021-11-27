#include "ssGUI/Extensions/Dockable.hpp"


#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Docker.hpp"

namespace ssGUI::Extensions
{
    bool Dockable::GlobalDockMode = false;
    ssGUI::MainWindow* Dockable::MainWindowUnderDocking = nullptr;
    ssGUI::GUIObject* Dockable::TargetDockObject = nullptr;
    Dockable::DockSide Dockable::TargetDockSide = Dockable::DockSide::NONE;
    
    Dockable::Dockable(Dockable const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        TopLevelParent = other.GetTopLevelParent();
        UseTriggerPercentage = other.IsUseTriggerPercentage();
        TriggerPixel = other.GetTriggerPixel();
        TriggerAreaColor = other.GetTriggerAreaColor();
        DockPreviewColor = other.GetDockPreviewColor();
        OriginalParent = nullptr;
        ContainerIsDocking = false;
        DockPreivewTop = nullptr;
        DockPreivewRight = nullptr;
        DockPreivewBottom = nullptr;
        DockPreivewLeft = nullptr;
        DockTriggerTop = nullptr;
        DockTriggerRight = nullptr;
        DockTriggerBottom = nullptr;
        DockTriggerLeft = nullptr;
    }

    void Dockable::CreateWidgetIfNotPresent(ssGUI::GUIObject** widget, glm::u8vec4 color)
    {
        //If widget is not present, create it
        if((*widget) == nullptr)
        {            
            (*widget) = new ssGUI::Widget();
            (*widget)->SetUserCreated(false);
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
    }

    
    void Dockable::DrawLeftPreview()
    {
        CreateWidgetIfNotPresent(&DockPreivewLeft, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewLeft->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewLeft->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(0.5);
        as->SetVerticalPercentage(1);
    }

    void Dockable::DrawTopPreview()
    {
        CreateWidgetIfNotPresent(&DockPreivewTop, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewTop->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewTop->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(0.5);
    }

    void Dockable::DrawRightPreview()
    {
        CreateWidgetIfNotPresent(&DockPreivewRight, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewRight->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewRight->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(0.5);
        as->SetVerticalPercentage(1);
    }

    void Dockable::DrawBottomPreview()
    {
        CreateWidgetIfNotPresent(&DockPreivewBottom, GetDockPreviewColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* ap = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockPreivewBottom->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* as = static_cast<ssGUI::Extensions::AdvancedSize*>(DockPreivewBottom->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
        ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);

        as->SetHorizontalUsePercentage(true);
        as->SetVerticalUsePercentage(true);
        as->SetHorizontalPercentage(1);
        as->SetVerticalPercentage(0.5);
    }

    void Dockable::DiscardLeftPreview()
    {
        if(DockPreivewLeft != nullptr)
        {
            DockPreivewLeft->Delete(true);
            DockPreivewLeft = nullptr;

        }
    }

    void Dockable::DiscardTopPreview()
    {
        if(DockPreivewTop != nullptr)
        {
            DockPreivewTop->Delete(true);
            DockPreivewTop = nullptr;
        }
    }

    void Dockable::DiscardRightPreview()
    {
        if(DockPreivewRight != nullptr)
        {
            DockPreivewRight->Delete(true);
            DockPreivewRight = nullptr;
        }
    }

    void Dockable::DiscardBottomPreview()
    {
        if(DockPreivewBottom != nullptr)
        {
            DockPreivewBottom->Delete(true);
            DockPreivewBottom = nullptr;
        }
    }


    void Dockable::DrawTriggerAreas()
    {
        //return;
        
        CreateWidgetIfNotPresent(&DockTriggerTop, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerRight, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerBottom, GetTriggerAreaColor());
        CreateWidgetIfNotPresent(&DockTriggerLeft, GetTriggerAreaColor());

        //Set the correct position and size
        ssGUI::Extensions::AdvancedPosition* apTop = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTriggerTop->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* asTop = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTriggerTop->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ssGUI::Extensions::AdvancedPosition* apRight = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTriggerRight->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* asRight = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTriggerRight->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ssGUI::Extensions::AdvancedPosition* apBottom = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTriggerBottom->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* asBottom = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTriggerBottom->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        ssGUI::Extensions::AdvancedPosition* apLeft = static_cast<ssGUI::Extensions::AdvancedPosition*>(DockTriggerLeft->GetExtension(ssGUI::Extensions::AdvancedPosition::EXTENSION_NAME));
        ssGUI::Extensions::AdvancedSize* asLeft = static_cast<ssGUI::Extensions::AdvancedSize*>(DockTriggerLeft->GetExtension(ssGUI::Extensions::AdvancedSize::EXTENSION_NAME));

        asTop->SetHorizontalUsePercentage(UseTriggerPercentage);
        asTop->SetVerticalUsePercentage(UseTriggerPercentage);
        asRight->SetHorizontalUsePercentage(UseTriggerPercentage);
        asRight->SetVerticalUsePercentage(UseTriggerPercentage);
        asBottom->SetHorizontalUsePercentage(UseTriggerPercentage);
        asBottom->SetVerticalUsePercentage(UseTriggerPercentage);
        asLeft->SetHorizontalUsePercentage(UseTriggerPercentage);
        asLeft->SetVerticalUsePercentage(UseTriggerPercentage);

        if(UseTriggerPercentage)
        {
            apTop->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apTop->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
            asTop->SetHorizontalPercentage(1 - GetTriggerPercentage() * 2);
            asTop->SetVerticalPercentage(GetTriggerPercentage());

            apRight->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
            apRight->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asRight->SetHorizontalPercentage(GetTriggerPercentage());
            asRight->SetVerticalPercentage(1 - GetTriggerPercentage() * 2);

            apBottom->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apBottom->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);
            asBottom->SetHorizontalPercentage(1 - GetTriggerPercentage() * 2);
            asBottom->SetVerticalPercentage(GetTriggerPercentage());

            apLeft->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
            apLeft->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asLeft->SetHorizontalPercentage(GetTriggerPercentage());
            asLeft->SetVerticalPercentage(1 - GetTriggerPercentage() * 2);
        }
        else
        {
            apTop->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apTop->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);
            asTop->SetHorizontalPixel(Container->GetSize().x - GetTriggerPixel() * 2);
            asTop->SetVerticalPixel(GetTriggerPixel());

            apRight->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::RIGHT);
            apRight->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asRight->SetHorizontalPixel(GetTriggerPixel());
            asRight->SetVerticalPixel(Container->GetSize().y - GetTriggerPixel() * 2);

            apBottom->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
            apBottom->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::BOTTOM);
            asBottom->SetHorizontalPixel(Container->GetSize().x - GetTriggerPixel() * 2);
            asBottom->SetVerticalPixel(GetTriggerPercentage());

            apLeft->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::LEFT);
            apLeft->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
            asLeft->SetHorizontalPixel(GetTriggerPixel());
            asLeft->SetVerticalPixel(Container->GetSize().y - GetTriggerPixel() * 2);
        }
    }

    //TODO : Add mutex for multi-threading support
    void Dockable::OnWindowDragStarted()
    {        
        ContainerIsDocking = true;
        GlobalDockMode = true;

        //Find the Main Window
        ssGUI::GUIObject* curParent = TopLevelParent == nullptr ? Container->GetParent() : TopLevelParent;
        while (curParent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && curParent != nullptr)
        {
            curParent = curParent->GetParent();
        }
        
        //If this has no parent. Docking is disabled
        if(curParent == nullptr)
        {
            ContainerIsDocking = false;
            GlobalDockMode = false;
            return;
        }

        MainWindowUnderDocking = static_cast<ssGUI::MainWindow*>(curParent);

        Container->AddTag(ssGUI::Tags::FLOATING);

        //Docking mechanism
        //Parent the container to the MainWindow.
        OriginalParent = Container->GetParent();

        if(TopLevelParent == nullptr)
            Container->SetParent(MainWindowUnderDocking);
        else
            Container->SetParent(TopLevelParent);
    }

    void Dockable::OnWindowDragFinished()
    {        
        //Remove the floating tag to allow docking
        Container->RemoveTag(ssGUI::Tags::FLOATING);
        
        //Docking mechanism for dockable window
        if(TargetDockObject != nullptr && TargetDockObject->GetParent() != nullptr && TargetDockSide != DockSide::NONE)
        {
            //If it is just docker, just need to add it as a child
            if(TargetDockObject->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
            {
                Container->SetParent(TargetDockObject);
                goto reset;
            }
            
            //Otherwise dock as normal
            ssGUI::Extensions::Layout* dockLayout = nullptr;

            //Check if the parent of the targetDockObject has the Docker extension
            if(TargetDockObject->GetParent()->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME) && TargetDockObject->GetParent()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
            {
                ssGUI::Extensions::Layout* parentLayout = static_cast<ssGUI::Extensions::Layout*>(TargetDockObject->GetParent()->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));

                if(TargetDockObject->GetParent()->GetChildrenCount() > 1)
                {
                    if(parentLayout->IsHorizontalLayout() && (TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::RIGHT))
                        dockLayout = parentLayout;
                    else if(!parentLayout->IsHorizontalLayout() && (TargetDockSide == DockSide::TOP || TargetDockSide == DockSide::BOTTOM))
                        dockLayout = parentLayout;
                }
                else
                {
                    if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::RIGHT)
                        parentLayout->SetHorizontalLayout(true);
                    else
                        parentLayout->SetHorizontalLayout(false);

                    dockLayout = parentLayout;
                }
            }

            //Create an empty parent and add Layout extension if dock layout isn't found
            if(dockLayout == nullptr)
            {
                ssGUI::Window* newParent = new ssGUI::Window();
                                
                newParent->SetUserCreated(false);
                newParent->SetSize(TargetDockObject->GetSize());
                newParent->SetAnchorType(TargetDockObject->GetAnchorType());
                newParent->SetPosition(TargetDockObject->GetPosition());
                newParent->SetParent(TargetDockObject->GetParent());

                //The docker will automatically create layout extension if not exist
                newParent->AddExtension(new ssGUI::Extensions::Docker());

                //Check if the generated docker does not use parent docker or not. If so, use default docker & layout settings if present
                if(newParent->GetParent()->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) 
                    && !static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsChildrenDockerUseThisSettings()
                    && static_cast<ssGUI::Extensions::Docker*>(newParent->GetParent()->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->IsEnabled())
                {
                    if(ssGUI::Extensions::Docker::GetDefaultGeneratedDockerSettings() != nullptr)
                        newParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME)->Copy(ssGUI::Extensions::Docker::GetDefaultGeneratedDockerSettings());

                    if(ssGUI::Extensions::Docker::GetDefaultGeneratedLayoutSettings() != nullptr)
                        newParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME)->Copy(ssGUI::Extensions::Docker::GetDefaultGeneratedLayoutSettings());
                }


                dockLayout = static_cast<ssGUI::Extensions::Layout*>(newParent->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));
                
                //Then change the Layout orientation to the same as the docking orientation
                if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::RIGHT)
                    dockLayout->SetHorizontalLayout(true);
                else
                    dockLayout->SetHorizontalLayout(false);

                //Floating
                if(TargetDockObject->GetParent()->GetType() == ssGUI::Enums::GUIObjectType::MAIN_WINDOW ||
                    TargetDockObject->GetParent() == TopLevelParent)
                {
                    newParent->SetBackgroundColour(static_cast<ssGUI::Extensions::Docker*>(newParent->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->GetFloatingDockerColor());
                }
                //Non Floating
                else
                {
                    newParent->SetTitlebar(false);
                    newParent->SetBackgroundColour(glm::u8vec4(0, 0, 0, 0));
                    newParent->SetResizeType(ssGUI::Enums::ResizeType::NONE);
                }

                //Restore order
                std::list<ssGUI::GUIObject*>::iterator dockObjectIt = TargetDockObject->GetParent()->FindChild(TargetDockObject);
                std::list<ssGUI::GUIObject*>::iterator endIt = TargetDockObject->GetParent()->GetChildrenEndIterator();
                
                TargetDockObject->GetParent()->ChangeChildOrder(--endIt, dockObjectIt);
                TargetDockObject->SetParent(newParent);
            }

            //This inserts the container to the end
            TargetDockObject->SetSize(glm::ivec2(TargetDockObject->GetSize().x * 0.5, TargetDockObject->GetSize().y * 0.5));
            //Container->SetSize(TargetDockObject->GetSize());
            Container->SetParent(TargetDockObject->GetParent());

            //Insert the Container after/before it
            std::list<ssGUI::GUIObject*>::iterator dockObjectIt = TargetDockObject->GetParent()->FindChild(TargetDockObject);
            std::list<ssGUI::GUIObject*>::iterator endIt = TargetDockObject->GetParent()->GetChildrenEndIterator();
            
            if(!dockLayout->IsReverseOrder())
            {
                //Before
                if(TargetDockSide == DockSide::LEFT || TargetDockSide == DockSide::TOP)
                    TargetDockObject->GetParent()->ChangeChildOrder(--endIt, dockObjectIt);
                //After
                else
                {
                    dockObjectIt++;
                    TargetDockObject->GetParent()->ChangeChildOrder(--endIt, dockObjectIt);
                }
            }
            else
            {
                //Before
                if(TargetDockSide == DockSide::RIGHT || TargetDockSide == DockSide::BOTTOM)
                    TargetDockObject->GetParent()->ChangeChildOrder(--endIt, dockObjectIt);
                //After
                else
                {
                    dockObjectIt++;
                    TargetDockObject->GetParent()->ChangeChildOrder(--endIt, dockObjectIt);
                }
            }
        }
        
        //Delete the original parent if it has less than 1 child and not UserCreated and is a Docker
        if(!OriginalParent->IsUserCreated() && OriginalParent->GetChildrenCount() <= 1 && OriginalParent->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME))
        {
            if(OriginalParent->GetChildrenCount() == 1)
            {
                ssGUI::GUIObject* child = (*(OriginalParent->GetChildrenStartIterator()));

                //Restore order
                if(OriginalParent->GetParent() != nullptr)
                {
                    child->SetPosition(OriginalParent->GetPosition());
                    
                    std::list<ssGUI::GUIObject*>::iterator posObjectIt = OriginalParent->GetParent()->FindChild(OriginalParent);
                    std::list<ssGUI::GUIObject*>::iterator endIt = OriginalParent->GetParent()->GetChildrenEndIterator();
                    child->SetParent(OriginalParent->GetParent());
                    child->GetParent()->ChangeChildOrder(--endIt, posObjectIt);
                    OriginalParent->Delete(true);
                    
                    //Check if the child is a docker and if the parent of originalParent is MainWindow or not. (Floating)
                    //If so, make the child draggable 
                    if(!child->IsUserCreated() && child->IsExtensionExist(ssGUI::Extensions::Docker::EXTENSION_NAME) && OriginalParent->GetParent()->GetType() == ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                    {
                        ssGUI::Window* childDocker = dynamic_cast<ssGUI::Window*>(child);

                        childDocker->SetTitlebar(true);
                        childDocker->SetDraggable(true);
                        childDocker->SetResizeType(ssGUI::Enums::ResizeType::ALL);
                        childDocker->SetBackgroundColour(static_cast<ssGUI::Extensions::Docker*>(child->GetExtension(ssGUI::Extensions::Docker::EXTENSION_NAME))->GetFloatingDockerColor());
                    }
                }
                else
                {
                    child->SetParent(OriginalParent->GetParent());   
                    OriginalParent->Delete(true);
                }
            }
            else
                OriginalParent->Delete(true);
        }

        //Reset docking variables
        reset:
        OriginalParent = nullptr;
        ContainerIsDocking = false;
        GlobalDockMode = false;
        MainWindowUnderDocking = nullptr;
        TargetDockObject = nullptr;
        TargetDockSide = DockSide::NONE;
    }
    
    void Dockable::DiscardTriggerAreas()
    {
        if(DockTriggerTop != nullptr)
        {
            DockTriggerTop->Delete(true);
            DockTriggerTop = nullptr;
        }

        if(DockTriggerRight != nullptr)
        {
            DockTriggerRight->Delete(true);
            DockTriggerRight = nullptr;
        }

        if(DockTriggerBottom != nullptr)
        {
            DockTriggerBottom->Delete(true);
            DockTriggerBottom = nullptr;
        }

        if(DockTriggerLeft != nullptr)
        {
            DockTriggerLeft->Delete(true);
            DockTriggerLeft = nullptr;
        }
    }
    
    const std::string Dockable::EXTENSION_NAME = "Dockable";

    Dockable::Dockable() : Container(nullptr), Enabled(true), TopLevelParent(nullptr), UseTriggerPercentage(true), TriggerPercentage(0.25f), 
                            TriggerPixel(15), TriggerAreaColor(glm::u8vec4(87, 207, 255, 127)), DockPreviewColor(glm::u8vec4(255, 255, 255, 127)), OriginalParent(nullptr),
                            ContainerIsDocking(false), DockPreivewTop(nullptr), DockPreivewRight(nullptr), DockPreivewBottom(nullptr), DockPreivewLeft(nullptr),
                            DockTriggerTop(nullptr), DockTriggerRight(nullptr), DockTriggerBottom(nullptr), DockTriggerLeft(nullptr)
    {}
    
    Dockable::~Dockable()
    {}
    
    bool GlobalDockMode = false;
    ssGUI::MainWindow* MainWindowUnderDocking = nullptr;

    void Dockable::SetTriggerPercentage(float percentage)
    {
        TriggerPercentage = percentage;
    }

    float Dockable::GetTriggerPercentage() const
    {
        return TriggerPercentage;
    }
    
    void Dockable::SetTriggerPixel(int pixel)
    {
        TriggerPixel = pixel;
    }

    int Dockable::GetTriggerPixel() const
    {
        return TriggerPixel;
    }

    void Dockable::SetUseTriggerPercentage(bool usePercentage)
    {
        UseTriggerPercentage = usePercentage;
    }

    bool Dockable::IsUseTriggerPercentage() const
    {
        return UseTriggerPercentage;
    }

    void Dockable::SetTriggerAreaColor(glm::u8vec4 color)
    {
        TriggerAreaColor = color;
    }

    glm::u8vec4 Dockable::GetTriggerAreaColor() const
    {
        return TriggerAreaColor;
    }

    void Dockable::SetDockPreviewColor(glm::u8vec4 color)
    {
        DockPreviewColor = color;
    }

    glm::u8vec4 Dockable::GetDockPreviewColor() const
    {
        return DockPreviewColor;
    }

    void Dockable::SetTopLevelParent(ssGUI::GUIObject* parent)
    {
        TopLevelParent = parent;
    }

    ssGUI::GUIObject* Dockable::GetTopLevelParent() const
    {
        return TopLevelParent;
    }   

    void Dockable::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }

    bool Dockable::IsEnabled() const
    {
        return Enabled;
    }

    //function: Update
    void Dockable::Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        if(!IsPreUpdate || Container == nullptr || !Enabled)
            return;

        //If global dock mode is true, check the cursor against the trigger area
        if(GlobalDockMode && !ContainerIsDocking && !globalInputStatus.DockingBlocked)
        {
            glm::ivec2 containerPos = Container->GetGlobalPosition();
            glm::ivec2 containerSize = Container->GetSize();
            int titleBarOffset = Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && Container->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW ? 
                                    static_cast<ssGUI::Window*>(Container)->GetTitlebarHeight() : 0;
            containerSize.y -= titleBarOffset;
            
            glm::ivec2 triggerSize = IsUseTriggerPercentage() ? glm::ivec2(glm::vec2(containerSize) * GetTriggerPercentage()) : glm::ivec2(GetTriggerPixel(), GetTriggerPixel());

            //Inform layout to exclude triggers and previews visual widgets
            ssGUI::Extensions::Layout* containerLayout = Container->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME) ? 
                                                            static_cast<ssGUI::Extensions::Layout*>(Container->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME)) : nullptr;

            bool previewDrawn = false;

            //Left
            if(inputInterface->GetCurrentMousePosition(mainWindow).x >= containerPos.x && 
                inputInterface->GetCurrentMousePosition(mainWindow).x <= containerPos.x + triggerSize.x &&
                inputInterface->GetCurrentMousePosition(mainWindow).y >= containerPos.y + titleBarOffset + triggerSize.y && 
                inputInterface->GetCurrentMousePosition(mainWindow).y <= containerPos.y + titleBarOffset + containerSize.y - triggerSize.y)
            {
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DrawLeftPreview();
                previewDrawn = true;
                TargetDockSide = DockSide::LEFT;
            }
            //Top
            else if(inputInterface->GetCurrentMousePosition(mainWindow).x >= containerPos.x + triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(mainWindow).x <= containerPos.x + containerSize.x - triggerSize.x &&
                    inputInterface->GetCurrentMousePosition(mainWindow).y >= containerPos.y + titleBarOffset && 
                    inputInterface->GetCurrentMousePosition(mainWindow).y <= containerPos.y + titleBarOffset + triggerSize.y)
            {
                DiscardLeftPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
                DrawTopPreview();
                previewDrawn = true;
                TargetDockSide = DockSide::TOP;
            }
            //Right
            else if(inputInterface->GetCurrentMousePosition(mainWindow).x >= containerPos.x + containerSize.x - triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(mainWindow).x <= containerPos.x + containerSize.x &&
                    inputInterface->GetCurrentMousePosition(mainWindow).y >= containerPos.y + titleBarOffset + triggerSize.y && 
                    inputInterface->GetCurrentMousePosition(mainWindow).y <= containerPos.y + titleBarOffset + containerSize.y - triggerSize.y)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardBottomPreview();
                DrawRightPreview();
                previewDrawn = true;
                TargetDockSide = DockSide::RIGHT;
            }
            //Bottom
            else if(inputInterface->GetCurrentMousePosition(mainWindow).x >= containerPos.x + triggerSize.x && 
                    inputInterface->GetCurrentMousePosition(mainWindow).x <= containerPos.x + containerSize.x - triggerSize.x &&
                    inputInterface->GetCurrentMousePosition(mainWindow).y >= containerPos.y + containerSize.y - triggerSize.y && 
                    inputInterface->GetCurrentMousePosition(mainWindow).y <= containerPos.y + containerSize.y)
            {
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DrawBottomPreview();  
                previewDrawn = true;
                TargetDockSide = DockSide::BOTTOM;
            }
            else
            {                
                DiscardLeftPreview();
                DiscardTopPreview();
                DiscardRightPreview();
                DiscardBottomPreview();
            }
            
            //Check if the cursor is inside the window
            if(inputInterface->GetCurrentMousePosition(mainWindow).x >= containerPos.x && inputInterface->GetCurrentMousePosition(mainWindow).x <= containerPos.x + containerSize.x &&
                inputInterface->GetCurrentMousePosition(mainWindow).y >= containerPos.y + titleBarOffset && inputInterface->GetCurrentMousePosition(mainWindow).y <= containerPos.y + containerSize.y)
            {
                if(!previewDrawn)
                {
                    TargetDockSide = DockSide::NONE;
                    DrawTriggerAreas();
                }
                else                    
                    DiscardTriggerAreas();
                
                    
                globalInputStatus.DockingBlocked = true;
                windowInputStatus.DockingBlocked = true;
                TargetDockObject = Container;
            }
            else
            {
                TargetDockSide = DockSide::NONE;
                DiscardTriggerAreas();
            }
        }
        else
        {   
            DiscardLeftPreview();
            DiscardTopPreview();
            DiscardRightPreview();
            DiscardBottomPreview();
            DiscardTriggerAreas();
        }
    }

    void Dockable::Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset)
    {}
    
    //function: GetExtensionName
    std::string Dockable::GetExtensionName()
    {
        return EXTENSION_NAME;
    }
    
    //function: BindToObject
    void Dockable::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;

        ssGUI::EventCallbacks::EventCallback* event = nullptr;
        if(Container->IsEventCallbackExist(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME))
            event = Container->GetEventCallback(ssGUI::EventCallbacks::WindowDragStateChangedEventCallback::EVENT_NAME);
        else
        {
            event = new ssGUI::EventCallbacks::WindowDragStateChangedEventCallback();
            Container->AddEventCallback(event);
        }

        if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
        {
            event->AddEventListener([&](ssGUI::GUIObject* src)
            {
                //When the current window started being dragged
                if(static_cast<ssGUI::Window*>(src)->GetWindowDragState() == ssGUI::Enums::WindowDragState::STARTED)
                {
                    OnWindowDragStarted();
                }
                //When the current window finished being dragged
                else if(static_cast<ssGUI::Window*>(src)->GetWindowDragState() == ssGUI::Enums::WindowDragState::ENDED)
                {
                    OnWindowDragFinished();
                }
            });
        }
    }

    void Dockable::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::Dockable* dockable = static_cast<ssGUI::Extensions::Dockable*>(extension);

        Enabled = dockable->IsEnabled();
        TopLevelParent = dockable->GetTopLevelParent();
        UseTriggerPercentage = dockable->IsUseTriggerPercentage();
        TriggerPixel = dockable->GetTriggerPixel();
        TriggerAreaColor = dockable->GetTriggerAreaColor();
        DockPreviewColor = dockable->GetDockPreviewColor();   
    }
    
    //function: Clone
    Extension* Dockable::Clone(ssGUI::GUIObject* newContainer)
    {
        Dockable* temp = new Dockable(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}



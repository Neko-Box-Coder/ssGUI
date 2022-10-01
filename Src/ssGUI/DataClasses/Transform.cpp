#include "ssGUI/DataClasses/Transform.hpp"
#include "ssGUI/DataClasses/Hierarchy.hpp"
#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/DataClasses/EventCallbackManager.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"


namespace ssGUI
{
    Transform::Transform(Transform const& other)
    {
        Position = other.GetPosition(); 
        GlobalPosition = other.GlobalPosition;
        Size = other.GetSize();
        MinSize = other.GetMinSize();
        MaxSize = other.GetMaxSize();
        Anchor = other.GetAnchorType();
        CurrentHierarchy = nullptr;
        CurrentRenderer = nullptr;
        CurrentEventCallbackManager = nullptr;
        CurrentObject = nullptr;
    }
    
    Transform::Transform() :    Position(0, 0),
                                GlobalPosition(0, 0),
                                Size(25, 25),
                                MinSize(25, 25),
                                MaxSize(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
                                Anchor(ssGUI::Enums::AnchorType::TOP_LEFT),
                                CurrentHierarchy(nullptr),
                                CurrentRenderer(nullptr),
                                CurrentEventCallbackManager(nullptr),
                                CurrentObject(nullptr)
    {}

    Transform::~Transform()
    {}

    void Transform::SetDependentComponents(ssGUI::Hierarchy* hierarchy, ssGUI::Renderer* renderer, ssGUI::EventCallbackManager* eventCallbackManager, 
        ssGUI::GUIObject* obj)
    {
        CurrentHierarchy = hierarchy;
        CurrentRenderer = renderer;
        CurrentEventCallbackManager = eventCallbackManager;
        CurrentObject = obj;
    }

    void Transform::SyncPosition()
    {
        ssLOG_FUNC_ENTRY();
        
        if(CurrentHierarchy->GetParent() == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        glm::vec2 parentGlobalPositon = CurrentHierarchy->GetParent()->GetParent() == nullptr ? 
                                        glm::vec2() : CurrentHierarchy->GetParent()->GetGlobalPosition();
        glm::vec2 parentSize = CurrentHierarchy->GetParent()->GetSize();
        glm::vec2 anchorPosition = parentGlobalPositon;
        glm::vec2 anchorDirection = glm::vec2(1, 1);
        glm::vec2 positionOffset = GetSize();
        
        //Find anchor position
        switch(Anchor)
        {
            case ssGUI::Enums::AnchorType::TOP_LEFT:
                anchorPosition = parentGlobalPositon;
                positionOffset = glm::vec2(0, 0);
                break;
            case ssGUI::Enums::AnchorType::TOP_RIGHT:
                anchorPosition = parentGlobalPositon + glm::vec2(parentSize.x, 0);
                anchorDirection.x = -1;
                positionOffset = glm::vec2(GetSize().x, 0);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_LEFT:
                anchorPosition = parentGlobalPositon + glm::vec2(0, parentSize.y);
                anchorDirection.y = -1;
                positionOffset = glm::vec2(0, GetSize().y);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_RIGHT:
                anchorPosition = parentGlobalPositon + glm::vec2(parentSize.x, parentSize.y);
                anchorDirection *= -1;
                positionOffset = glm::vec2(GetSize().x, GetSize().y);
                break;
        }

        //TODO : Maybe move this part to window as override
        if(CurrentHierarchy->GetParent()->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
            CurrentHierarchy->GetParent()->GetType() == ssGUI::Enums::GUIObjectType::WINDOW && 
            (Anchor == ssGUI::Enums::AnchorType::TOP_LEFT || Anchor == ssGUI::Enums::AnchorType::TOP_RIGHT))
        {
            anchorPosition.y += dynamic_cast<ssGUI::Window*>(CurrentHierarchy->GetParent())->GetTitlebarHeight();
        }

        //Local Position = (Global position - Anchor Point) * anchorDirection
        Position.x = (GlobalPosition.x + positionOffset.x - anchorPosition.x) * anchorDirection.x;
        Position.y = (GlobalPosition.y + positionOffset.y - anchorPosition.y) * anchorDirection.y;

        ssLOG_FUNC_EXIT();
    }
    
    void Transform::SyncGlobalPosition()
    {
        ssLOG_FUNC_ENTRY();
        
        if(CurrentHierarchy->GetParent() == nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }
        
        glm::vec2 parentGlobalPositon = CurrentHierarchy->GetParent()->GetParent() == nullptr ? 
            glm::vec2() : CurrentHierarchy->GetParent()->GetGlobalPosition();
        glm::vec2 parentSize = CurrentHierarchy->GetParent()->GetSize();
        glm::vec2 anchorPosition = parentGlobalPositon;
        glm::vec2 anchorDirection = glm::vec2(1, 1);
        glm::vec2 positionOffset = GetSize();

        // std::cout<<"parent pos:"<<parentGlobalPositon.x<<", "<<parentGlobalPositon.y<<"\n";
        // std::cout<<"parentSize: "<<parentSize.x<<", "<<parentSize.y<<"\n";
        
        //Find anchor position
        switch(Anchor)
        {
            case ssGUI::Enums::AnchorType::TOP_LEFT:
                anchorPosition = parentGlobalPositon;
                positionOffset = glm::vec2(0, 0);
                break;
            case ssGUI::Enums::AnchorType::TOP_RIGHT:
                anchorPosition = parentGlobalPositon + glm::vec2(parentSize.x, 0);
                anchorDirection.x = -1;
                positionOffset = glm::vec2(GetSize().x, 0);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_LEFT:
                anchorPosition = parentGlobalPositon + glm::vec2(0, parentSize.y);
                anchorDirection.y = -1;
                positionOffset = glm::vec2(0, GetSize().y);
                break;
            case ssGUI::Enums::AnchorType::BOTTOM_RIGHT:
                anchorPosition = parentGlobalPositon + glm::vec2(parentSize.x, parentSize.y);
                anchorDirection *= -1;
                positionOffset = glm::vec2(GetSize().x, GetSize().y);
                break;
        }

        //TODO : Maybe move this part to window as override
        if(CurrentHierarchy->GetParent()->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW && 
            CurrentHierarchy->GetParent()->GetType() == ssGUI::Enums::GUIObjectType::WINDOW &&
            (Anchor == ssGUI::Enums::AnchorType::TOP_LEFT || Anchor == ssGUI::Enums::AnchorType::TOP_RIGHT))
        {
            anchorPosition.y += dynamic_cast<ssGUI::Window*>(CurrentHierarchy->GetParent())->GetTitlebarHeight();
        }

        // std::cout<<"anchorPosition: "<<anchorPosition.x<<", "<<anchorPosition.y<<"\n";
        // std::cout<<"anchorDirection: "<<anchorDirection.x<<", "<<anchorDirection.y<<"\n";
        // std::cout<<"positionOffset: "<<positionOffset.x<<", "<<positionOffset.y<<"\n";

        //Global Position = AnchorPoint + local position * anchorDirection
        GlobalPosition.x = anchorPosition.x + Position.x * anchorDirection.x - positionOffset.x;
        GlobalPosition.y = anchorPosition.y + Position.y * anchorDirection.y - positionOffset.y;

        ssLOG_FUNC_EXIT();
    }

    glm::vec2 Transform::GetPosition() const
    {
        return Position;
    }

    void Transform::SetPosition(glm::vec2 position)
    {
        if(Position != position)
            CurrentRenderer->RedrawObject();
        
        Position = position;
    }

    glm::vec2 Transform::GetGlobalPosition()
    {
        //Update global position
        SyncGlobalPosition();
        
        return GlobalPosition;
    }

    void Transform::SetGlobalPosition(glm::vec2 position)
    {
        if(GlobalPosition != position)
            CurrentRenderer->RedrawObject();

        GlobalPosition = position;

        //Update local position
        SyncPosition();
    }

    glm::vec2 Transform::GetSize() const
    {
        return Size;
    }

    void Transform::SetSize(glm::vec2 size)
    {
        glm::vec2 oriSize = Size;
        
        size.x = size.x > MaxSize.x ? MaxSize.x : size.x;
        size.y = size.y > MaxSize.y ? MaxSize.y : size.y;
        size.x = size.x < MinSize.x ? MinSize.x : size.x;
        size.y = size.y < MinSize.y ? MinSize.y : size.y;

        Size = size;

        if(Size != oriSize)
        {
            if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::EventCallbacks::SizeChangedEventCallback::EVENT_NAME))
                CurrentEventCallbackManager->GetEventCallback(ssGUI::EventCallbacks::SizeChangedEventCallback::EVENT_NAME)->Notify(CurrentObject);
            CurrentRenderer->RedrawObject();
        }
    }

    glm::vec2 Transform::GetMinSize() const
    {
        return MinSize;
    }

    void Transform::SetMinSize(glm::vec2 minSize)
    {
        if(MinSize != minSize)
            CurrentRenderer->RedrawObject();
        
        MinSize = minSize;
        MaxSize.x = MinSize.x > MaxSize.x ? MinSize.x : MaxSize.x;
        MaxSize.y = MinSize.y > MaxSize.y ? MinSize.y : MaxSize.y;
        glm::vec2 size = GetSize(); //MainWindow is not updating the Size component therefore need to use GetSize()

        if(size.x < MinSize.x && Size.y < MinSize.y)
            SetSize(MinSize);
        else if(size.x < MinSize.x)
            SetSize(glm::vec2(MinSize.x, size.y));
        else if(size.y < MinSize.y)        
            SetSize(glm::vec2(size.x, MinSize.y));
            
        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME))
            CurrentEventCallbackManager->GetEventCallback(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME)->Notify(CurrentObject);
    }

    glm::vec2 Transform::GetMaxSize() const
    {
        return MaxSize;
    }

    void Transform::SetMaxSize(glm::vec2 maxSize)
    {
        if(MaxSize != maxSize)
            CurrentRenderer->RedrawObject();
        
        MaxSize = maxSize;
        MinSize.x = MaxSize.x < MinSize.x ? MaxSize.x : MinSize.x;
        MinSize.y = MaxSize.y < MinSize.y ? MaxSize.y : MinSize.y;
        glm::vec2 size = GetSize();

        if(size.x > MaxSize.x && size.y > MaxSize.y)
            SetSize(MaxSize);
        else if(size.x > MaxSize.x)
            SetSize(glm::vec2(MaxSize.x, size.y));
        else if(size.y > MaxSize.y)
            SetSize(glm::vec2(size.x, MaxSize.y));

        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME))
            CurrentEventCallbackManager->GetEventCallback(ssGUI::EventCallbacks::MinMaxSizeChangedEventCallback::EVENT_NAME)->Notify(CurrentObject);
    }

    ssGUI::Enums::AnchorType Transform::GetAnchorType() const
    {
        return Anchor;
    }
    
    void Transform::SetAnchorType(ssGUI::Enums::AnchorType anchorType)
    {
        Anchor = anchorType;
    }
}


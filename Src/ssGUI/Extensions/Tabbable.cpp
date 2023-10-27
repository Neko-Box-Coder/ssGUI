#include "ssGUI/GUIObjectClasses/MainWindow.hpp"
#include "ssGUI/Extensions/Tabbable.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

namespace ssGUI
{

//TODO(NOW): Finish constructors and copy function

namespace Extensions
{
    Tabbable::Tabbable() :    Container(nullptr),
                            Enabled(true),
                            TabAreaObject(-1),
                            CurrentObjectsReferences(),
                            TopLevelParent(-1),
                            Untabbable(false)
    {}

    Tabbable::~Tabbable()
    {}

    Tabbable::Tabbable(Tabbable const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
    }
    
    void Tabbable::ConstructRenderInfo()
    {}

    void Tabbable::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset)
    {}

    #if 0
    void Tabbable::DrawTabPreview(glm::vec2 mousePos)
    {
        //TODO(NOW)
        ssLOG_LINE("DrawTabPreview...");
    }
    #endif

    const std::string Tabbable::EXTENSION_NAME = "Tabbable";
    
    void Tabbable::SetTopLevelParent(ssGUI::GUIObject* topLevelParent)
    {
        if(topLevelParent == nullptr)
        {
            TopLevelParent = -1;
            return;
        }
        
        TopLevelParent = CurrentObjectsReferences.AddObjectReference(topLevelParent);
    }
    
    ssGUI::GUIObject* Tabbable::GetTopLevelParent() const
    {
        if(TopLevelParent < 0)
            return nullptr;
        
        return CurrentObjectsReferences.GetObjectReference(TopLevelParent);
    }

    void Tabbable::SetUntabbable(bool untabbable)
    {
        Untabbable = untabbable;
    }
    
    bool Tabbable::IsUntabbable() const
    {
        return Untabbable;
    }
    
    glm::vec2 Tabbable::GetLastMouseDragBeginPosition() const
    {
        return LastMouseDragBeginPosition;
    }
    
    void Tabbable::SetLastMouseDragBeginPosition(glm::vec2 pos)
    {
        LastMouseDragBeginPosition = pos;
    }
    
    ssGUI::GUIObject* Tabbable::GetTabAreaObject() const
    {
        if(TabAreaObject < 0)
            return nullptr;
        
        return CurrentObjectsReferences.GetObjectReference(TabAreaObject);
    }
    
    void Tabbable::SetTabAreaObject(ssGUI::GUIObject* tabArea)
    {
        if(tabArea == nullptr)
        {
            TabAreaObject = -1;
            return;
        }
        
        TabAreaObject = CurrentObjectsReferences.AddObjectReference(tabArea);
    }
    
    #if 0
    void Tabbable::AddExternalTab(ssGUI::GUIObject* exteranlObject, bool dockAfter)
    {
        //TODO(NOW)
        ssLOG_LINE("AddTab");
        ssLOG_LINE("dockAfter: " << dockAfter);
    }
    #endif
    
    bool Tabbable::IsEnabled() const
    {
        return Enabled;
    }

    void Tabbable::SetEnabled(bool enabled)
    {
        Enabled = enabled;
    }
    
    void Tabbable::Internal_Update(bool isPreUpdate, 
                                            ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();

        #define DISCARD_AND_RETURN() do { return; } while(0)
        
        //This is function is executed twice, one before the Container GUI object update and one after.
        //You can use the isPreUpdate variable to decide when to execute the extension update
        if(isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
            return;
        
        #if 0
        //If we are being dragged, check if we are still being dragged
        if(Dragged)
        {
            if( inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::MouseButton::LEFT) ||
                inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::MouseButton::LEFT))
            {
                currentInputStatus.CurrentDragData.SetDragData(Container);
                inputInterface->SetCursorType(ssGUI::Enums::CursorType::MOVE);
                DISCARD_AND_RETURN();
            }
            
            Dragged = false;
        }

        //If there's no mouse button input, that means we don't need to do anything
        if( !inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::MouseButton::LEFT) &&
            !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::MouseButton::LEFT))
        {
            DISCARD_AND_RETURN();
        }
        #endif

        if(Container->GetType() != ssGUI::Enums::GUIObjectType::WINDOW)
            DISCARD_AND_RETURN();

        {
            auto* window = static_cast<ssGUI::Window*>(Container);
            
            if(window->GetWindowDragState() == ssGUI::Enums::WindowDragState::STARTED)
            {
                auto* mainWindowBackend = static_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface();
                LastMouseDragBeginPosition = inputInterface->GetCurrentMousePosition(mainWindowBackend);
            }
            
            //If the window is being dragged / finished being dragged, set the input status
            if( window->GetWindowDragState() == ssGUI::Enums::WindowDragState::DRAGGING || 
                window->GetWindowDragState() == ssGUI::Enums::WindowDragState::ENDED)
            {
                currentInputStatus.CurrentDragData.SetDragData(Container);
            }
        }

        #if 0
        //If we are start to being dragged to be tabbed somewhere else, register it
        if( lastInputStatus.CurrentDragData.GetDragDataType() == ssGUI::Enums::DragDataType::NONE &&
            inputInterface->IsButtonOrKeyDown(ssGUI::Enums::MouseButton::LEFT))
        {
            currentInputStatus.CurrentDragData.SetDragData(Container);
            inputInterface->SetCursorType(ssGUI::Enums::CursorType::MOVE);
            Dragged = true;
            DISCARD_AND_RETURN();
        }
        #endif



        #if 0
        //See if mouse cursor is inside container
        glm::ivec2 mousePos = inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());
        glm::vec2 containerMaxBound = Container->GetGlobalPosition() + Container->GetSize();
        bool isMouseInContainer =   !(  mousePos.x < Container->GetGlobalPosition().x ||
                                        mousePos.x >= containerMaxBound.x ||
                                        mousePos.y < Container->GetGlobalPosition().y ||
                                        mousePos.y >= containerMaxBound.y);

        //Check if we can block mouse input
        bool canBlockMouse =    currentInputStatus.MouseInputBlockedData.GetBlockDataType() == ssGUI::Enums::BlockDataType::GUI_OBJECT && 
                                currentInputStatus.MouseInputBlockedData.GetBlockData<ssGUI::GUIObject>() == Container;
        
        canBlockMouse |= currentInputStatus.MouseInputBlockedData.GetBlockDataType() == ssGUI::Enums::BlockDataType::NONE;

        //Check if we are being tabbed from external
        if( lastInputStatus.CurrentDragData.GetDragDataType() == ssGUI::Enums::DragDataType::GUI_OBJECT &&
            lastInputStatus.CurrentDragData.GetDragData<ssGUI::GUIObject>() != Container &&
            !lastInputStatus.CurrentDragData.IsIntercepted())
        {
            //If the mouse is inside the container
            if(canBlockMouse && isMouseInContainer)
            {
                //If mouse up, add it to a tab
                if(inputInterface->IsButtonOrKeyUp(ssGUI::Enums::MouseButton::LEFT))
                {
                    lastInputStatus.CurrentDragData.SetInterceptor(Container);
                    
                    //TODO(NOW): Account for vertical tab
                    AddExternalTab( lastInputStatus.CurrentDragData.GetDragData<ssGUI::GUIObject>(), 
                            mousePos.x > Container->GetGlobalPosition().x + Container->GetSize().x / 2.f);
                    
                    DISCARD_AND_RETURN();
                }
                //otherwise, show preview
                else
                {
                    DrawTabPreview(mousePos);
                    return;
                }
            }
            
            DISCARD_AND_RETURN();
        }
        
        DISCARD_AND_RETURN();
        #endif
    }

    void Tabbable::Internal_Draw(  bool isPreRender, 
                                            ssGUI::Backend::BackendDrawingInterface* drawingInterface, 
                                            ssGUI::GUIObject* mainWindow, 
                                            glm::vec2 mainWindowPositionOffset)
    {
    }

    std::string Tabbable::GetExtensionName() const
    {
        return EXTENSION_NAME;
    }

    void Tabbable::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
        
        
        
        
    }

    void Tabbable::Copy(const ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        auto* original = static_cast<const ssGUI::Extensions::Tabbable*>(extension);
        Enabled = original->IsEnabled();
    }

    ObjectsReferences* Tabbable::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    Tabbable* Tabbable::Clone()
    {
        Tabbable* temp = new Tabbable(*this);
        return temp;
    }
}

}
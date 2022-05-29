#include "ssGUI/Extensions/LayoutEnforcer.hpp"
#include "ssGUI/Extensions/Layout.hpp"


#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

//Remember to add the .cpp file to the cmake list if you are using cmake

namespace ssGUI::Extensions
{
    LayoutEnforcer::LayoutEnforcer() : Container(nullptr), Enabled(true), ContainerStartPos(), ContainerStartSize(), ContainerResizeStarted(false),
                                        LastContainerSize()
    {}

    LayoutEnforcer::~LayoutEnforcer()
    {}
    
    LayoutEnforcer::LayoutEnforcer(LayoutEnforcer const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        ContainerStartPos = other.ContainerStartPos;
        ContainerStartSize = other.ContainerStartSize;
        ContainerResizeStarted = other.ContainerResizeStarted;
    }

    void LayoutEnforcer::ConstructRenderInfo()
    {}

    void LayoutEnforcer::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {}
    
    //Defining the extension name
    const std::string LayoutEnforcer::EXTENSION_NAME = "Layout Enforcer";

    void LayoutEnforcer::SetEnabled(bool enabled)
    {
        Enabled = enabled;

        //Redrawing the object if this affects the rendering
        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool LayoutEnforcer::IsEnabled() const
    {
        return Enabled;
    }
        
    //Extension methods
    void LayoutEnforcer::Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        FUNC_DEBUG_ENTRY();

        if(!Enabled || Container == nullptr)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        if(isPreUpdate)
        {
            if(Container->GetType() == ssGUI::Enums::GUIObjectType::WINDOW)
            {
                LastContainerSize = Container->GetSize();
            }
            
            FUNC_DEBUG_EXIT();
            return;
        }

        //Check if parent has layout
        if(Container->GetType() != ssGUI::Enums::GUIObjectType::WINDOW || Container->GetParent() == nullptr || 
            !Container->GetParent()->IsExtensionExist(ssGUI::Extensions::Layout::EXTENSION_NAME))
        {
            Container->RemoveExtension(GetExtensionName());
            FUNC_DEBUG_EXIT();
            return;
        }

        //Check if container is resizing
        auto windowContainer = static_cast<ssGUI::Window*>(Container);
        if(!windowContainer->IsResizing())
        {
            ContainerResizeStarted = false;
            FUNC_DEBUG_EXIT();
            return;
        }

        if(LastContainerSize != Container->GetSize())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        auto layout = static_cast<ssGUI::Extensions::Layout*>(Container->GetParent()->GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));
        
        auto resizeLambda = [&](float targetResizeAmount, float containerResizedAmount, float othersResizedAmount, ssGUI::WindowResizeDragData const & resizeData, bool horizontal)
        {
            float diff = targetResizeAmount - containerResizedAmount - othersResizedAmount;

            //If the resizing is going towards the other direction, reset the resize state for the window
            if(targetResizeAmount == 0 || (diff > 0 && targetResizeAmount < 0) || (diff < 0 && targetResizeAmount > 0))
            {
                ContainerStartPos = Container->GetPosition();
                ContainerStartSize = Container->GetSize();
                auto temp = resizeData;
                temp.MouseDownPosition = inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow));
                temp.OnTransformBeginSize = Container->GetSize();
                windowContainer->SetResizeDragData(temp);
                LastContainerSize = Container->GetSize();
                FUNC_DEBUG_EXIT();
                return;
            }

            float resizedAmount = 0;
            auto resizeBefore = [&](bool resizeFirst)
            {
                float prevDiff = resizeFirst ? diff : resizedAmount;
                //Reduce gui objects sizes before container
                Container->GetParent()->StashChildrenIterator();
                Container->GetParent()->FindChild(Container);
                Container->GetParent()->MoveChildrenIteratorPrevious();
                while(!Container->GetParent()->IsChildrenIteratorEnd() && prevDiff != 0)
                {
                    auto curChild = Container->GetParent()->GetCurrentChild();
                    float sizeBefore = horizontal ? curChild->GetSize().x : curChild->GetSize().y;
                    if(horizontal)
                    {
                        curChild->SetSize(glm::vec2(curChild->GetSize().x + prevDiff, curChild->GetSize().y));
                        prevDiff -= curChild->GetSize().x - sizeBefore;
                        resizedAmount += curChild->GetSize().x - sizeBefore;
                    }
                    else
                    {
                        curChild->SetSize(glm::vec2(curChild->GetSize().x, curChild->GetSize().y + prevDiff));
                        prevDiff -= curChild->GetSize().y - sizeBefore;
                        resizedAmount += curChild->GetSize().y - sizeBefore;
                    }

                    Container->GetParent()->MoveChildrenIteratorPrevious();
                }
                Container->GetParent()->PopChildrenIterator();
            };

            auto resizeAfter = [&](bool resizeFirst)
            {
                //Increase gui objects sizes after container
                float afterDiff = resizeFirst ? diff : resizedAmount;
                Container->GetParent()->StashChildrenIterator();
                Container->GetParent()->FindChild(Container);
                Container->GetParent()->MoveChildrenIteratorNext();
                while(!Container->GetParent()->IsChildrenIteratorEnd() && afterDiff != 0)
                {
                    auto curChild = Container->GetParent()->GetCurrentChild();
                    float sizeBefore = horizontal ? curChild->GetSize().x : curChild->GetSize().y;
                    if(horizontal)
                    {
                        curChild->SetSize(glm::vec2(curChild->GetSize().x - afterDiff, curChild->GetSize().y));
                        afterDiff += curChild->GetSize().x - sizeBefore;
                        resizedAmount -= curChild->GetSize().x - sizeBefore;
                    }
                    else
                    {
                        curChild->SetSize(glm::vec2(curChild->GetSize().x, curChild->GetSize().y - afterDiff));
                        afterDiff += curChild->GetSize().y - sizeBefore;
                        resizedAmount -= curChild->GetSize().x - sizeBefore;
                    }

                    Container->GetParent()->MoveChildrenIteratorNext();
                }
                Container->GetParent()->PopChildrenIterator();
            };

            if(targetResizeAmount < 0)
            {
                resizeBefore(true);
                resizeAfter(false);
            }
            else
            {
                resizeAfter(true);
                resizeBefore(false);
            }
        };

        //Check if container's min size has reached. 
        //If so, decrease the size to previous GUI object depending on the distance the cursor 
        //Increase the same amount of size to next GUI Object 
        if(layout->IsHorizontalLayout() && 
            (Container->GetSize().x <= Container->GetMinSize().x || Container->GetSize().x >= Container->GetMaxSize().x))
        {               
            auto resizeData = windowContainer->GetResizeDragData();
            
            //When container resizing first reached min size
            if(!ContainerResizeStarted)
            {
                ContainerResizeStarted = true;
                ContainerStartPos = Container->GetPosition();
                ContainerStartSize = Container->GetSize();
                FUNC_DEBUG_EXIT();
                return;
            }

            //Resizing left
            if(windowContainer->GetResizeType() == ssGUI::Enums::ResizeType::LEFT)
            {
                //Calculates how much do we want to reduce the size of the gui object
                float targetResizeAmount = resizeData.MouseDownPosition.x - 
                    inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)).x;
                
                float containerResizedAmount = Container->GetSize().x - resizeData.OnTransformBeginSize.x; 
                float othersResizedAmount = (ContainerStartPos.x + ContainerStartSize.x) - (Container->GetPosition().x + Container->GetSize().x);
                resizeLambda(targetResizeAmount, containerResizedAmount, othersResizedAmount, resizeData, true);
            }
            //Resizing right
            else
            {
                //Calculates how much do we want to reduce the size of the gui object
                float targetResizeAmount = inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)).x - 
                    resizeData.MouseDownPosition.x;
                
                float containerResizedAmount = Container->GetSize().x - resizeData.OnTransformBeginSize.x; 
                float othersResizedAmount = Container->GetPosition().x - ContainerStartPos.x;
                resizeLambda(targetResizeAmount, containerResizedAmount, othersResizedAmount, resizeData, true);
            }
        }
        else if(!layout->IsHorizontalLayout() && 
            (Container->GetSize().y <= Container->GetMinSize().y || Container->GetSize().y >= Container->GetMaxSize().y))
        {
            auto resizeData = windowContainer->GetResizeDragData();
            
            //When container resizing first reached min size
            if(!ContainerResizeStarted)
            {
                ContainerResizeStarted = true;
                ContainerStartPos = Container->GetPosition();
                ContainerStartSize = Container->GetSize();
            }
            
            //Resizing top
            if(windowContainer->GetResizeType() == ssGUI::Enums::ResizeType::TOP)
            {
                //Calculates how much do we want to reduce the size of the gui object
                float targetResizeAmount = resizeData.MouseDownPosition.y - 
                    inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)).y;
                
                float containerResizedAmount = Container->GetSize().y - resizeData.OnTransformBeginSize.y; 
                float othersResizedAmount = ContainerStartPos.y - Container->GetPosition().y;

                resizeLambda(targetResizeAmount, containerResizedAmount, othersResizedAmount, resizeData, false);
            }
            //Resizing bottom
            else
            {
                //Calculates how much do we want to reduce the size of the gui object
                float targetResizeAmount = inputInterface->GetCurrentMousePosition(static_cast<ssGUI::MainWindow*>(mainWindow)).y - 
                    resizeData.MouseDownPosition.y;
                
                float containerResizedAmount = Container->GetSize().y - resizeData.OnTransformBeginSize.y; 
                float othersResizedAmount = Container->GetPosition().y - ContainerStartPos.y;

                resizeLambda(targetResizeAmount, containerResizedAmount, othersResizedAmount, resizeData, false);
            }
        }
        else
            ContainerResizeStarted = false;

        FUNC_DEBUG_EXIT();
    }

    void LayoutEnforcer::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {        
        FUNC_DEBUG_ENTRY();
        //Don't need to draw anything
        FUNC_DEBUG_EXIT();
    }

    std::string LayoutEnforcer::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void LayoutEnforcer::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void LayoutEnforcer::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;
        
        ssGUI::Extensions::LayoutEnforcer* layoutEnforcer = static_cast<ssGUI::Extensions::LayoutEnforcer*>(extension);
        
        Enabled = layoutEnforcer->IsEnabled();
        ContainerStartPos = layoutEnforcer->ContainerStartPos;
        ContainerStartSize = layoutEnforcer->ContainerStartSize;
        ContainerResizeStarted = layoutEnforcer->ContainerResizeStarted;
    }

    ObjectsReferences* LayoutEnforcer::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    LayoutEnforcer* LayoutEnforcer::Clone(ssGUI::GUIObject* newContainer)
    {
        LayoutEnforcer* temp = new LayoutEnforcer(*this);
        if(newContainer != nullptr)
            newContainer->AddExtension(temp);
        return temp;
    }
}
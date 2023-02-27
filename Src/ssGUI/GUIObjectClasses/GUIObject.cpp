#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp"    //This is for getting the MainWindow offset
#include "ssGUI/ssGUIManager.hpp"                   //This is for accessing DeletedObjs
#include "ssGUI/GUIObjectClasses/Menu.hpp"          //This is for spawning right click menu
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{    
    std::vector<ssGUI::GUIObject*> GUIObject::ObjsToDelete = std::vector<ssGUI::GUIObject*>();


    GUIObject::GUIObject(GUIObject const& other) : Transform(other), Renderer(other), Hierarchy(other), ExtensionManager(other),
                                                    EventCallbackManager(other)
    {
        LastGlobalPosition = other.LastGlobalPosition;
        CurrentTags = other.CurrentTags;// std::unordered_set<std::string>();
        RightClickMenu = other.RightClickMenu;

        SetupComponents();

        SetParent(other.GetParent()); //Note : Reason of using SetParent is to inform the parent to add this as a child
    }

    void GUIObject::SetupComponents()
    {
        Transform::SetDependentComponents(this, this, this, this);
        Renderer::SetDependentComponents(this, this, this);
        Hierarchy::SetDependentComponents(this, this, this, this);
        ExtensionManager::SetDependentComponents(this, this);
        EventCallbackManager::SetDependentComponents(this, this);
    }

    ssGUI::GUIObject* GUIObject::CloneChildren(ssGUI::GUIObject* originalRoot, ssGUI::GUIObject* clonedRoot)
    {
        ssLOG_FUNC_ENTRY();
        
        //1. First get a list of objects needed to be cloned and create a hashmap of original objects with the index of it
        std::vector<ssGUI::GUIObject*> originalObjsToClone = std::vector<ssGUI::GUIObject*>();
        std::unordered_map<ssGUI::GUIObject*, int> originalObjsIndex = std::unordered_map<ssGUI::GUIObject*, int>();
        std::vector<ssGUI::GUIObject*> clonedObjs = std::vector<ssGUI::GUIObject*>();

        originalObjsToClone.push_back(originalRoot);
        originalObjsIndex[originalRoot] = 0;
        clonedObjs.push_back(clonedRoot);

        int currentIndex = 0;
        do
        {
            ssGUI::GUIObject* curObj = originalObjsToClone[currentIndex];
            curObj->StashChildrenIterator();
            curObj->MoveChildrenIteratorToFirst();
            while (!curObj->IsChildrenIteratorEnd())
            {
                originalObjsToClone.push_back(curObj->GetCurrentChild());
                originalObjsIndex[curObj->GetCurrentChild()] = originalObjsToClone.size() - 1;
                curObj->MoveChildrenIteratorNext();
            }
            curObj->PopChildrenIterator();
            currentIndex++;
        }
        while (currentIndex < originalObjsToClone.size());

        //2. Clone the objects (starting from 1 because root is cloned)
        for(int i = 1; i < originalObjsToClone.size(); i++)
        {
            auto curClonedObj = originalObjsToClone[i]->Clone(false);
            curClonedObj->SetParent(nullptr);
            clonedObjs.push_back(curClonedObj);
        }

        //3. Set ObjectsReferences for both the object itself, extension and event callbacks using the hashmap
        for(int i = 0; i < clonedObjs.size(); i++)
        {
            auto curClonedObj = clonedObjs[i];
            auto curObjOR = clonedObjs[i]->Internal_GetObjectsReferences();
            auto curObjORindices = curObjOR->GetListOfObjectsIndices();

            //Fix the object's objects references
            for(auto orIndex : curObjORindices)
            {
                auto refObj = curObjOR->GetObjectReference(orIndex);

                //if the referencing object is found to be cloned
                if(originalObjsIndex.find(refObj) != originalObjsIndex.end())
                    curObjOR->SetObjectReference(orIndex, clonedObjs[originalObjsIndex[refObj]]);
            }

            //Fix the object's extensions' objects references
            auto extensionsList = curClonedObj->GetListOfExtensions();
            for(auto extension : extensionsList)
            {
                auto curExtensionOR = extension->Internal_GetObjectsReferences();
                if(curExtensionOR == nullptr)
                    continue;
                
                auto curExtensionORindices = curExtensionOR->GetListOfObjectsIndices();
                for(auto orIndex : curExtensionORindices)
                {
                    auto refObj = curExtensionOR->GetObjectReference(orIndex);

                    //if the referencing object is found to be cloned
                    if(originalObjsIndex.find(refObj) != originalObjsIndex.end())
                        curExtensionOR->SetObjectReference(orIndex, clonedObjs[originalObjsIndex[refObj]]);
                }
            }

            //Fix the object's event callbacks' objects references
            auto eventCallbackList = curClonedObj->GetListOfEventCallbacks();
            for(auto eventCallback : eventCallbackList)
            {
                auto curEventCallbackOR = eventCallback->Internal_GetObjectsReferences();
                if(curEventCallbackOR == nullptr)
                    continue;
                
                auto curEventCallbackORindices = curEventCallbackOR->GetListOfObjectsIndices();
                for(auto orIndex : curEventCallbackORindices)
                {
                    auto refObj = curEventCallbackOR->GetObjectReference(orIndex);

                    //if the referencing object is found to be cloned
                    if(originalObjsIndex.find(refObj) != originalObjsIndex.end())
                        curEventCallbackOR->SetObjectReference(orIndex, clonedObjs[originalObjsIndex[refObj]]);
                }
            }
        }

        //4. Set the correct parent to the cloned objects (Don't need to set root's parent as it is already set when it was cloned)
        for(int i = 1; i < clonedObjs.size(); i++)
        {
            auto oriParent = originalObjsToClone[i]->GetParent();
            if(originalObjsIndex.find(oriParent) == originalObjsIndex.end())
            {
                ssGUI_ERROR(ssGUI_GUI_OBJECT_TAG, "Unable to clone, original parent can't be found: "<<oriParent);
                ssLOG_EXIT_PROGRAM();
                return nullptr;
            }
            
            clonedObjs[i]->SetParent(clonedObjs[originalObjsIndex.at(oriParent)]);
        }

        ssLOG_FUNC_EXIT();
        return clonedRoot;
    }

    void GUIObject::CloneExtensionsAndEventCallbacks(ssGUI::GUIObject* clonedObj)
    {
        ssLOG_FUNC_ENTRY();
        for(auto extension : Extensions)
        {
            if(!clonedObj->IsExtensionExist(extension.second->GetExtensionName()))
                extension.second->Clone(clonedObj);
            else
                clonedObj->GetExtension(extension.first)->Copy(extension.second);
        }

        //Copy draw and update order
        for(int i = 0; i < ExtensionsDrawOrder.size(); i++)
            clonedObj->ChangeExtensionDrawOrder(ExtensionsDrawOrder[i], i);
        
        for(int i = 0; i < ExtensionsUpdateOrder.size(); i++)
            clonedObj->ChangeExtensionUpdateOrder(ExtensionsUpdateOrder[i], i);

        for(auto eventCallback : EventCallbacks)
        {
            std::vector<ssGUI::GUIObject*> tempVec = std::vector<ssGUI::GUIObject*>();
            
            if(!clonedObj->IsEventCallbackExist(eventCallback.second->GetEventCallbackName()))
                eventCallback.second->Clone(clonedObj, true);
        }

        ssLOG_FUNC_EXIT();
    }

    void GUIObject::CheckRightClickMenu(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
        ssGUI::GUIObject* mainWindow)
    {
        if(inputStatus.MouseInputBlockedObject != nullptr || RightClickMenu == nullptr)
            return;
        
        //Mouse Input blocking
        glm::ivec2 currentMousePos = inputInterface->GetCurrentMousePosition(dynamic_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface());

        bool mouseInWindowBoundX = false;
        bool mouseInWindowBoundY = false;
        
        if(currentMousePos.x >= GetGlobalPosition().x && currentMousePos.x <= GetGlobalPosition().x + GetSize().x)
            mouseInWindowBoundX = true;

        if(currentMousePos.y >= GetGlobalPosition().y && currentMousePos.y <= GetGlobalPosition().y + GetSize().y)
            mouseInWindowBoundY = true;
        
        //Input blocking
        if(mouseInWindowBoundX && mouseInWindowBoundY && (inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::RIGHT) && !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::RIGHT)))
        {
            inputStatus.MouseInputBlockedObject = this;
            RightClickMenu->SetMenuTarget(this);
            RightClickMenu->SpawnMenu(currentMousePos);
        }
    }

    void GUIObject::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
        ssGUI::GUIObject* mainWindow)
    {
    }

    GUIObject::GUIObject() :    LastGlobalPosition(),
                                CurrentTags(),
                                RightClickMenu(nullptr)
    {
        SetupComponents();
    }

    GUIObject::~GUIObject()
    {
        ssLOG_FUNC_ENTRY();
        if(!ObjectDelete)
        {
            NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
            
            CurrentObjectsReferences.CleanUp();
        }

        //Only clean up extensions and event callbacks when the GUI Object is destroyed
        for(auto it : Extensions)
            ssGUI::Factory::Dispose(it.second);
        
        for(auto it : EventCallbacks)
            ssGUI::Factory::Dispose(it.second);
        
        ssLOG_FUNC_EXIT();
    }

    ssGUI::Enums::GUIObjectType GUIObject::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::BASE_OBJECT;
    }

    void GUIObject::AddTag(std::string tag)
    {
        if(CurrentTags.find(tag) == CurrentTags.end())
            CurrentTags.insert(tag);
    }

    void GUIObject::RemoveTag(std::string tag)
    {
        if(CurrentTags.find(tag) != CurrentTags.end())
            CurrentTags.erase(tag);
    }

    bool GUIObject::HasTag(std::string tag) const
    {
        return CurrentTags.find(tag) != CurrentTags.end();
    }

    void GUIObject::RegisterRightClickMenu(ssGUI::Menu* menu)
    {
        RightClickMenu = menu;
    }

    void GUIObject::ClearRightClickMenu()
    {
        RightClickMenu = nullptr;
    }

    void GUIObject::Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        ssLOG_FUNC_ENTRY();

        if(!IsEnabled())
        {
            Redraw = false;
            ssLOG_FUNC_EXIT();
            return;
        }

        if(Redraw)
        {
            DisableRedrawObjectRequest();
            
            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(true, drawingInterface, mainWindow, mainWindowPositionOffset);

            UpdateGUIObjectVertexAndShapeIndex();

            ConstructRenderInfo(drawingInterface, mainWindow, mainWindowPositionOffset);

            for(auto extension : ExtensionsDrawOrder)
                Extensions.at(extension)->Internal_Draw(false, drawingInterface, mainWindow, mainWindowPositionOffset);

            EnableRedrawObjectRequest();

            drawingInterface->DrawEntities(DrawingVerticies, DrawingUVs, DrawingColours, DrawingCounts, DrawingProperties);
            CacheRendering();
            DrawingVerticies.clear();
            DrawingUVs.clear();
            DrawingColours.clear();
            DrawingCounts.clear();
            DrawingProperties.clear();
            Redraw = false;
        }
        else
            drawingInterface->DrawEntities(LastDrawingVerticies, LastDrawingUVs, LastDrawingColours, LastDrawingCounts, LastDrawingProperties);

        ssLOG_FUNC_EXIT();
    }

    void GUIObject::Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow)
    {
        ssLOG_FUNC_ENTRY();
        
        //If it is not enabled, don't even update/draw it
        if(!IsEnabled())
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(true, inputInterface, inputStatus, mainWindow);
        }

        CheckRightClickMenu(inputInterface, inputStatus, mainWindow);
        MainLogic(inputInterface, inputStatus, mainWindow);

        for(auto extension : ExtensionsUpdateOrder)
        {
            //Guard against extension being deleted by other extensions
            if(!IsExtensionExist(extension))
                continue;

            Extensions.at(extension)->Internal_Update(false, inputInterface, inputStatus, mainWindow);
        }
        
        //Check position different for redraw
        if(GetGlobalPosition() != LastGlobalPosition)
            RedrawObject();

        LastGlobalPosition = GetGlobalPosition();

        ssLOG_FUNC_EXIT();
    }

    GUIObject* GUIObject::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        GUIObject* temp = new GUIObject(*this);
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
    
    void GUIObject::InitiateDefaultResources(){}
}

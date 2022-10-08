#include "ssGUI/ssGUIManager.hpp"
#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Dropdown.hpp"
#include "ssGUI/ssGUITags.hpp"

//Debug
#include "ssGUI/Extensions/Layout.hpp"
#include <thread>
#include <chrono>



namespace ssGUI
{
    void ssGUIManager::Internal_Update()
    {
        ssLOG_FUNC_ENTRY();

        for(auto mainWindow : MainWindowPList)
        {
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                ssLOG_LINE("Invalid object type added to gui manager");
                continue;
            }

            if(!mainWindow->IsEnabled())
                continue;

            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);

            currentMainWindowP->Internal_Draw();
            currentMainWindowP->ClearBackBuffer();
        }

        while (!MainWindowPList.empty())
        {
            uint64_t startFrameTime = BackendInput->GetElapsedTime();
            
            PollInputs();

            //Clear up any main windows that are closed
            CheckMainWindowExistence();

            //Set cursor default to normal. Save last cursor so don't need to load cursor every frame
            ssGUI::Enums::CursorType lastCursor = BackendInput->GetCursorType();
            std::string lastCustomCursorName = BackendInput->GetCurrentCustomCursorName();
            BackendInput->SetCursorType(ssGUI::Enums::CursorType::NORMAL);

            //Dispatch Update event
            ssLOG_FUNC_ENTRY("ssGUIManagerPreUpdateEvent");
            for(int i = 0; i < PreGUIUpdateEventListeners.size(); i++)
            {                
                if(PreGUIUpdateEventListenersValid[i])
                    PreGUIUpdateEventListeners[i]();
            }
            ssLOG_FUNC_EXIT("ssGUIManagerPreUpdateEvent");

            #if DEBUG_STATE 
                ssLOG_LINE("Update");
            #endif

            UpdateObjects();

            //Clear up any main windows that are deleted
            CheckMainWindowExistence();

            //Clean up deleted objects
            if(!ssGUI::GUIObject::ObjsToDelete.empty())
            {
                for(int i = 0; i < ssGUI::GUIObject::ObjsToDelete.size(); i++)
                {
                    if(ssGUI::GUIObject::ObjsToDelete[i]->Internal_IsDeleted())
                    {
                        if(ssGUI::GUIObject::ObjsToDelete[i]->IsHeapAllocated())
                            delete ssGUI::GUIObject::ObjsToDelete[i];
                    }
                }
                ssGUI::GUIObject::ObjsToDelete = std::vector<ssGUI::GUIObject*>();
            }

            //Dispatch Update event
            ssLOG_FUNC_ENTRY("ssGUIManagerPostUpdateEvent");
            for(int i = 0; i < PostGUIUpdateEventListeners.size(); i++)
            {                
                if(PostGUIUpdateEventListenersValid[i])
                    PostGUIUpdateEventListeners[i]();
            }
            ssLOG_FUNC_EXIT("ssGUIManagerPostUpdateEvent");

            #if DEBUG_STATE 
                ssLOG_LINE("Render");
            #endif

            //Dispatch Custom Rendering event
            if(IsCustomRendering)
            {
                ssLOG_FUNC_ENTRY("ssGUIManagerCustomRenderingEvent");
                for(int i = 0; i < OnCustomRenderEventListeners.size(); i++)
                {
                    if(OnCustomRenderEventListenersValid[i])
                        OnCustomRenderEventListeners[i](MainWindowPList);
                }
                ssLOG_FUNC_EXIT("ssGUIManagerCustomRenderingEvent");
            }
            else
                Render();
            
            #if DEBUG_STATE 
                ssLOG_LINE("Post Render");
            #endif

            if(lastCursor != BackendInput->GetCursorType() || 
                (lastCursor == ssGUI::Enums::CursorType::CUSTOM && lastCustomCursorName != BackendInput->GetCurrentCustomCursorName()))
            {
                UpdateCursor();   
            }

            #if SLOW_UPDATE
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            #endif

            if(TargetFrameInterval > 0)
            {
                if(FrameTimes.size() < 100)
                    FrameTimes.push_back(BackendInput->GetElapsedTime() - startFrameTime);
                else
                {
                    FrameTimes[FrameTimeIndex++] = BackendInput->GetElapsedTime() - startFrameTime;
                    FrameTimeIndex %= 100;
                }
                
                float averageFrameTime = 0;

                for(int i = 0; i < FrameTimes.size(); i++)
                    averageFrameTime += FrameTimes[i];
                
                averageFrameTime /= FrameTimes.size();
                // ssLOG_LINE("averageFrameTime: "<<averageFrameTime);
                // ssLOG_LINE("TargetFrameInterval - averageFrameTime: "<<TargetFrameInterval - averageFrameTime);
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(TargetFrameInterval - averageFrameTime)));
            }

            #if REFRESH_CONSOLE
                Clear();
            #endif
        }
        ssLOG_FUNC_EXIT();
    }
    
    void ssGUIManager::PollInputs()
    {
        BackendInput->UpdateInput();
    }
    
    void ssGUIManager::CheckMainWindowExistence()
    {
        ssLOG_FUNC_ENTRY();
        
        std::vector<ssGUI::GUIObject*> windowsToRemoved;

        for(auto windowP : MainWindowPList)
        {
            if(static_cast<ssGUI::MainWindow*>(windowP)->IsClosed() || windowP->Internal_IsDeleted())
                windowsToRemoved.push_back(windowP);
        }
        
        for(int i = 0; i < windowsToRemoved.size(); i++)
        {
            RemoveGUIObject(windowsToRemoved[i]);
        }

        ssLOG_FUNC_EXIT();
    }

    void ssGUIManager::Render()
    {        
        ssLOG_FUNC_ENTRY();
        
        if(MainWindowPList.empty())
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        for(auto mainWindow : MainWindowPList)
        {
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                ssLOG_LINE("Invalid object type added to gui manager");
                continue;
            }

            if(!mainWindow->IsEnabled())
                continue;

            //TODO: Change from list to vector
            std::list<std::pair<ssGUI::GUIObject*, bool>> objToRender;      //Object to render, has overlay parent flag
            std::list<ssGUI::GUIObject*> renderQueue;
            std::vector<ssGUI::GUIObject*> overlayQueue;
            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);

            currentMainWindowP->Internal_Draw();

            //Populate the render queue first
            currentMainWindowP->StashChildrenIterator();
            currentMainWindowP->MoveChildrenIteratorToFirst();
            while (!currentMainWindowP->IsChildrenIteratorEnd())
            {
                objToRender.push_back(std::pair(currentMainWindowP->GetCurrentChild(), false));
                currentMainWindowP->MoveChildrenIteratorNext();
            }
            currentMainWindowP->PopChildrenIterator();

            while (!objToRender.empty())
            {
                ssGUI::GUIObject* currentObjP = objToRender.front().first;
                bool isParentOverlay = objToRender.front().second;
                objToRender.pop_front();

                bool overlay = isParentOverlay || currentObjP->HasTag(ssGUI::Tags::OVERLAY);
                if(overlay)
                    overlayQueue.push_back(currentObjP);
                else
                    renderQueue.push_back(currentObjP);

                //Add children to draw queue
                if(currentObjP->GetChildrenCount() > 0)
                {
                    currentObjP->StashChildrenIterator();
                    //Add children from back to front so that the drawing queue order is front to back
                    currentObjP->MoveChildrenIteratorToLast();
                    while (!currentObjP->IsChildrenIteratorEnd())
                    {
                        objToRender.push_front(std::pair(currentObjP->GetCurrentChild(), overlay));
                        currentObjP->MoveChildrenIteratorPrevious();
                    }
                    currentObjP->PopChildrenIterator();
                }
            }

            //Check if render is needed
            bool renderNeeded = false;

            if(!IsForceRendering())
            {
                if(!currentMainWindowP->IsRedrawNeeded())
                {
                    for(auto it = renderQueue.begin(); it != renderQueue.end(); it++)
                    {
                        if((*it)->IsRedrawNeeded())
                        {
                            renderNeeded = true;
                            break;
                        }
                    }

                    for(auto it = overlayQueue.begin(); it != overlayQueue.end(); it++)
                    {
                        if((*it)->IsRedrawNeeded())
                        {
                            renderNeeded = true;
                            break;
                        }
                    }
                }
                else
                    renderNeeded = true;
            }
            else
                renderNeeded = true;

            //Render if needed
            if(renderNeeded)
            {
                #if USE_DEBUG
                    ssLOG_LINE("Rendering");
                #endif

                while(!renderQueue.empty())
                {
                    ssGUI::GUIObject* currentObjP = renderQueue.front();

                    //Remove current gui object from render queue
                    renderQueue.pop_front();

                    if(currentObjP->Internal_IsDeleted())
                        continue;

                    if(IsForceRendering())
                        currentObjP->RedrawObject();

                    (currentObjP)->Internal_Draw(   currentMainWindowP->GetBackendDrawingInterface(), 
                                                    dynamic_cast<ssGUI::GUIObject*>(currentMainWindowP), 
                                                    currentMainWindowP->GetPositionOffset());
                }

                for(int i = 0; i < overlayQueue.size(); i++)
                {
                    overlayQueue[i]->Internal_Draw(  currentMainWindowP->GetBackendDrawingInterface(), 
                                                    dynamic_cast<ssGUI::GUIObject*>(currentMainWindowP), 
                                                    currentMainWindowP->GetPositionOffset());
                }

                //Dispatch Post Rendering Update event
                ssLOG_FUNC_ENTRY("ssGUIManagerPostRenderingUpdateEvent");
                for(int i = 0; i < PostGUIRenderingUpdateEventListeners.size(); i++)
                {
                    if(PostGUIRenderingUpdateEventListenersValid[i])
                        PostGUIRenderingUpdateEventListeners[i]();
                }
                ssLOG_FUNC_EXIT("ssGUIManagerPostRenderingUpdateEvent");

                //Draw everything that is displayed on the mainWindow back buffer
                currentMainWindowP->Render();
            }
            else
            {
                #if USE_DEBUG
                    ssLOG_LINE("Caching");
                #endif

                currentMainWindowP->ClearBackBuffer();
            }
        }
        ssLOG_FUNC_EXIT();
    }

    void ssGUIManager::UpdateObjects()
    {
        ssLOG_FUNC_ENTRY();
        
        if(MainWindowPList.empty())
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        std::stack<ssGUI::GUIObject*> objToUpdate;
        std::stack<bool> childrenEvaluated;
        std::queue<ssGUI::GUIObject*> updateQueue;

        //For now, update main window in order as it doesn't matter
        for(auto mainWindow : MainWindowPList)
        {            
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                ssLOG_LINE("Invalid object type added to gui manager");
                continue;
            }
            if(mainWindow->Internal_IsDeleted() || static_cast<ssGUI::Window*>(mainWindow)->IsClosed())
                continue;

            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);
            
            objToUpdate.push(mainWindow);
            childrenEvaluated.push(false);

            ssGUI::GUIObject* parentWindowP = nullptr;
            ssGUI::InputStatus inputStatus;

            while (!objToUpdate.empty())
            {
                //Update object if there's no children or the children are already evaluated
                if(objToUpdate.top()->GetChildrenCount() == 0 || childrenEvaluated.top() == true)
                {                                        
                    updateQueue.push(objToUpdate.top());
                    objToUpdate.pop();
                    childrenEvaluated.pop();
                }
                //Otherwise add all the children
                else
                {                    
                    childrenEvaluated.top() = true;

                    ssGUI::GUIObject* currentObjP = objToUpdate.top();
                    currentObjP->StashChildrenIterator();
                    currentObjP->MoveChildrenIteratorToFirst();
                    while (!currentObjP->IsChildrenIteratorEnd())
                    {
                        objToUpdate.push(currentObjP->GetCurrentChild());
                        childrenEvaluated.push(false);
                        currentObjP->MoveChildrenIteratorNext();
                    }
                    currentObjP->PopChildrenIterator();
                }
            }

            while (!updateQueue.empty())
            {
                if(!updateQueue.front()->Internal_IsDeleted())
                {
                    updateQueue.front()->Internal_Update(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(BackendInput), inputStatus, mainWindow);
                    
                    #if USE_DEBUG
                    ssLOG_LINE("object "<<updateQueue.front()<<" checking validity");
                    updateQueue.front()->Internal_GetObjectsReferences()->CheckObjectsReferencesValidity();
                    #endif
                }
                updateQueue.pop();
            }
        }

        ssLOG_FUNC_EXIT();
    }

    void ssGUIManager::UpdateCursor()
    {
        //Update the cursor
        BackendInput->UpdateCursor();
    }

    ssGUI::GUIObject* ssGUIManager::FindParentWindowP(ssGUI::GUIObject& obj)
    {        
        ssGUI::GUIObject* currentParentP = obj.GetParent();

        while (currentParentP != nullptr && currentParentP->GetType() != ssGUI::Enums::GUIObjectType::WINDOW)
            currentParentP = currentParentP->GetParent();
        
        if(currentParentP != nullptr && currentParentP->GetType() != ssGUI::Enums::GUIObjectType::WINDOW)
            return nullptr;

        return currentParentP;
    }

    ssGUI::ssGUIManager* ssGUIManager::CurrentInstanceP = nullptr;

    ssGUIManager::ssGUIManager() :  BackendInput(), MainWindowPList(), PreGUIUpdateEventListeners(), 
                                    PreGUIUpdateEventListenersValid(), PreGUIUpdateEventListenersNextFreeIndices(),
                                    PostGUIUpdateEventListeners(), PostGUIUpdateEventListenersValid(), 
                                    PostGUIUpdateEventListenersNextFreeIndices(),
                                    PostGUIRenderingUpdateEventListeners(), PostGUIRenderingUpdateEventListenersValid(), 
                                    PostGUIRenderingUpdateEventListenersNextFreeIndices(), OnCustomRenderEventListeners(),
                                    OnCustomRenderEventListenersValid(), OnCustomRenderEventListenersNextFreeIndices(), 
                                    IsCustomRendering(false), ForceRendering(false), TargetFrameInterval(20),
                                    FrameTimeIndex(0), FrameTimes()
    {
        BackendInput = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
        CurrentInstanceP = this;
    }

    ssGUIManager::~ssGUIManager()
    {
        delete BackendInput;
    }
    
    void ssGUIManager::AddGUIObject(ssGUI::GUIObject* obj)
    {
        if(obj->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            return;
        
        MainWindowPList.push_back(obj);

        UpdateCursor();
    }

    void ssGUIManager::UseCustomRendering(bool customRendering)
    {
        IsCustomRendering = customRendering;
    }
    
    void ssGUIManager::RemoveGUIObject(ssGUI::GUIObject* obj)
    {
        MainWindowPList.remove(obj);
    }

    int ssGUIManager::GetGUIObjectCount()
    {
        return MainWindowPList.size();
    }

    void ssGUIManager::StartRunning()
    {
        ssLOG_FUNC_ENTRY();
        Internal_Update();
        CleanUpDefaultSystemGUIObjects();
        ssLOG_FUNC_EXIT();
    }

    ssGUI::Backend::BackendSystemInputInterface* ssGUIManager::GetBackendInputInterface()
    {
        return BackendInput;
    }

    ssGUI::ssGUIManager* ssGUIManager::GetInstance()
    {
        return CurrentInstanceP;
    }

    int ssGUIManager::AddPreGUIUpdateEventListener(std::function<void()> event)
    {
        int addedIndex = -1;

        if(PreGUIUpdateEventListenersNextFreeIndices.empty())
        {
            PreGUIUpdateEventListeners.push_back(event);
            PreGUIUpdateEventListenersValid.push_back(true);
            addedIndex = PreGUIUpdateEventListeners.size() - 1;
        }
        else
        {
            addedIndex = PreGUIUpdateEventListenersNextFreeIndices.front();
            PreGUIUpdateEventListeners[PreGUIUpdateEventListenersNextFreeIndices.front()] = event;
            PreGUIUpdateEventListenersValid[PreGUIUpdateEventListenersNextFreeIndices.front()] = true;
            PreGUIUpdateEventListenersNextFreeIndices.pop();
        }

        return addedIndex;
    }

    void ssGUIManager::RemovePreGUIUpdateEventListener(int index)
    {
        PreGUIUpdateEventListenersValid[index] = false;
        PreGUIUpdateEventListenersNextFreeIndices.push(index);
    }

    int ssGUIManager::AddPostGUIUpdateEventListener(std::function<void()> event)
    {
        int addedIndex = -1;

        if(PostGUIUpdateEventListenersNextFreeIndices.empty())
        {
            PostGUIUpdateEventListeners.push_back(event);
            PostGUIUpdateEventListenersValid.push_back(true);
            addedIndex = PostGUIUpdateEventListeners.size() - 1;
        }
        else
        {
            addedIndex = PostGUIUpdateEventListenersNextFreeIndices.front();
            PostGUIUpdateEventListeners[PostGUIUpdateEventListenersNextFreeIndices.front()] = event;
            PostGUIUpdateEventListenersValid[PostGUIUpdateEventListenersNextFreeIndices.front()] = true;
            PostGUIUpdateEventListenersNextFreeIndices.pop();
        }

        return addedIndex;
    }

    void ssGUIManager::RemovePostGUIUpdateEventListener(int index)
    {
        PostGUIUpdateEventListenersValid[index] = false;
        PostGUIUpdateEventListenersNextFreeIndices.push(index);
    }

    int ssGUIManager::AddPostGUIRenderingUpdateEventListener(std::function<void()> event)
    {
        int addedIndex = -1;

        if(PostGUIRenderingUpdateEventListenersNextFreeIndices.empty())
        {
            PostGUIRenderingUpdateEventListeners.push_back(event);
            PostGUIRenderingUpdateEventListenersValid.push_back(true);
            addedIndex = PostGUIRenderingUpdateEventListeners.size() - 1;
        }
        else
        {
            addedIndex = PostGUIRenderingUpdateEventListenersNextFreeIndices.front();
            PostGUIRenderingUpdateEventListeners[PostGUIRenderingUpdateEventListenersNextFreeIndices.front()] = event;
            PostGUIRenderingUpdateEventListenersValid[PostGUIRenderingUpdateEventListenersNextFreeIndices.front()] = true;
            PostGUIRenderingUpdateEventListenersNextFreeIndices.pop();
        }

        return addedIndex;
    }

    void ssGUIManager::RemovePostGUIRenderingUpdateEventListener(int index)
    {
        PostGUIRenderingUpdateEventListenersValid[index] = false;
        PostGUIRenderingUpdateEventListenersNextFreeIndices.push(index);
    }

    int ssGUIManager::AddOnCustomRenderEventListener(std::function<void( std::list<ssGUI::GUIObject*>& ) > event)
    {
        int addedIndex = -1;

        if(OnCustomRenderEventListenersNextFreeIndices.empty())
        {
            OnCustomRenderEventListeners.push_back(event);
            OnCustomRenderEventListenersValid.push_back(true);
            addedIndex = OnCustomRenderEventListeners.size() - 1;
        }
        else
        {
            addedIndex = OnCustomRenderEventListenersNextFreeIndices.front();
            OnCustomRenderEventListeners[OnCustomRenderEventListenersNextFreeIndices.front()] = event;
            OnCustomRenderEventListenersValid[OnCustomRenderEventListenersNextFreeIndices.front()] = true;
            OnCustomRenderEventListenersNextFreeIndices.pop();
        }

        return addedIndex;
    }

    void ssGUIManager::RemoveOnCustomRenderEventListener(int index)
    {
        OnCustomRenderEventListenersValid[index] = false;
        OnCustomRenderEventListenersNextFreeIndices.push(index);
    }

    void ssGUIManager::SetForceRendering(bool force)
    {
        ForceRendering = force;
    }

    bool ssGUIManager::IsForceRendering()
    {
        return ForceRendering;
    }

    void ssGUIManager::SetTargetFramerate(float framerate)
    {
        if(framerate <= 0)
            TargetFrameInterval = -1;
        else
            TargetFrameInterval = 1000 / framerate; 
    }

    float ssGUIManager::GetTargetFramerate()
    {
        return 1000 / TargetFrameInterval;
    }

    void ssGUIManager::CleanUpDefaultSystemGUIObjects()
    {
        Text::CleanUpAllDefaultFonts();
        Dropdown::CleanUpAllDefaultDropdownImage();
    }
}
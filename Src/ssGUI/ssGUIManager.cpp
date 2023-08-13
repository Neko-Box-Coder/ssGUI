#include "ssGUI/ssGUIManager.hpp"
#include "ssGUI/GUIObjectClasses/Text.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Dropdown.hpp"
#include "ssGUI/ssGUITags.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "ssGUI/Backend/BackendFactory.hpp"

//Debug
#include "ssGUI/Extensions/Layout.hpp"
#include <thread>
#include <chrono>



namespace ssGUI
{
    void ssGUIManager::InitializeMainWindows()
    {
        for(auto mainWindow : MainWindowPList)
        {
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                ssGUI_WARNING(ssGUI_MANAGER_TAG, "Invalid object type added to gui manager");
                continue;
            }

            if(!mainWindow->IsEnabled())
                continue;

            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);

            currentMainWindowP->ClearBackBuffer();
            currentMainWindowP->Internal_Draw();
            currentMainWindowP->ClearBackBuffer();
        }
    }

    void ssGUIManager::Internal_Update()
    {
        ssGUI_LOG_FUNC();

        InitializeMainWindows();

        float threadSleepMultiplier = 1;
        while (IsRunningNeededAsync())
        {
            uint64_t startFrameTime = BackendInput->GetElapsedTime();
            
            StepFrame();

            #if SSGUI_SLOW_UPDATE
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
                
                float averageFrameTime = 1;

                for(int i = 0; i < FrameTimes.size(); i++)
                    averageFrameTime += FrameTimes[i];
                
                averageFrameTime /= FrameTimes.size();
                //ssGUI_DEBUG(ssGUI_MANAGER_TAG, "averageFrameTime: "<<averageFrameTime);
                //ssGUI_DEBUG(ssGUI_MANAGER_TAG, "TargetFrameInterval - averageFrameTime: "<<TargetFrameInterval - averageFrameTime);
                //ssGUI_DEBUG(ssGUI_MANAGER_TAG, "threadSleepMultiplier: "<<threadSleepMultiplier);

                //There's no guarantee that the thread will sleep for how long we ask for (especially on Windows *cough* *cough*)
                //so we need to adjust the sleep time depending on how long will have actually slept
                uint32_t preSleepTime = BackendInput->GetElapsedTime();
                int64_t sleepDuration = static_cast<int64_t>((TargetFrameInterval - averageFrameTime) * threadSleepMultiplier);
                if(sleepDuration < 0)
                    sleepDuration = 0;
                //ssGUI_DEBUG(ssGUI_MANAGER_TAG, "sleepDuration: "<<sleepDuration<<", "<<"TargetFrameInterval: "<<TargetFrameInterval<<", averageFrameTime: "<<averageFrameTime);
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
                uint32_t actualSleptDuration = BackendInput->GetElapsedTime() - preSleepTime;
                
                #ifndef FLOAT_EQ
                #define FLOAT_EQ(a, b) (a + 0.0001f > b && a - 0.0001f < b)
                #endif

                if(FLOAT_EQ(actualSleptDuration, 0))
                    threadSleepMultiplier = 1.f;
                else
                    threadSleepMultiplier = (double)sleepDuration / (double)actualSleptDuration;
            }

            #if SSGUI_REFRESH_CONSOLE
                Clear();
            #endif
        }
    }
    
    bool ssGUIManager::CheckMainWindowExistence()
    {
        ssGUI_LOG_FUNC();
        
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

        return !MainWindowPList.empty();
    }

    void ssGUIManager::Render()
    {        
        ssGUI_LOG_FUNC();
        
        if(MainWindowPList.empty())
            return;

        for(auto mainWindow : MainWindowPList)
        {
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                ssGUI_WARNING(ssGUI_MANAGER_TAG, "Invalid object type added to gui manager");
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
                objToRender.push_back(std::pair<ssGUI::GUIObject*, bool>(currentMainWindowP->GetCurrentChild(), false));
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
                        objToRender.push_front(std::pair<ssGUI::GUIObject*, bool>(currentObjP->GetCurrentChild(), overlay));
                        currentObjP->MoveChildrenIteratorPrevious();
                    }
                    currentObjP->PopChildrenIterator();
                }
            }

            //Check if render is needed
            bool renderNeeded = false;

            if(!IsRedrawEveryFrame())
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
                #ifdef SSGUI_LOG_MANAGER_STATE
                    ssGUI_DEBUG(ssGUI_MANAGER_TAG, "Rendering");
                #endif

                while(!renderQueue.empty())
                {
                    ssGUI::GUIObject* currentObjP = renderQueue.front();

                    //Remove current gui object from render queue
                    renderQueue.pop_front();

                    if(currentObjP->Internal_IsDeleted())
                        continue;

                    if(IsRedrawEveryFrame())
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

                {
                    //Dispatch Post Rendering Update event
                    ssGUI_LOG_FUNC("ssGUIManagerPostRenderingUpdateEvent");
                    for(int i = 0; i < PostGUIRenderEventListeners.size(); i++)
                    {
                        if(PostGUIRenderEventListenersValid[i])
                            PostGUIRenderEventListeners[i](currentMainWindowP);
                    }
                }

                //Draw everything that is displayed on the mainWindow back buffer
                currentMainWindowP->Render();
            }
            else
            {
                #ifdef SSGUI_LOG_MANAGER_STATE
                    ssGUI_DEBUG(ssGUI_MANAGER_TAG, "Caching");
                #endif
                currentMainWindowP->ClearBackBuffer();
            }
        }
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

    ssGUIManager::ssGUIManager() :  BackendInput(), 
                                    MainWindowPList(), 
                                    PreGUIUpdateEventListeners(), 
                                    PreGUIUpdateEventListenersValid(), 
                                    PreGUIUpdateEventListenersNextFreeIndices(),
                                    PostGUIUpdateEventListeners(), 
                                    PostGUIUpdateEventListenersValid(), 
                                    PostGUIUpdateEventListenersNextFreeIndices(),
                                    PostGUIRenderEventListeners(), 
                                    PostGUIRenderEventListenersValid(), 
                                    PostGUIRenderEventListenersNextFreeIndices(), 
                                    OnCustomRenderEventListeners(),
                                    OnCustomRenderEventListenersValid(), 
                                    OnCustomRenderEventListenersNextFreeIndices(), 
                                    IsCustomRendering(false), 
                                    ForceRendering(false), 
                                    LastCursor(ssGUI::Enums::CursorType::NORMAL),
                                    LastCursorName(),
                                    TargetFrameInterval(10),
                                    FrameTimeIndex(0), 
                                    FrameTimes()
    {
        BackendInput = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
        CurrentInstanceP = this;
    }

    ssGUIManager::~ssGUIManager()
    {
        delete BackendInput;
    }
    
    void ssGUIManager::AddRootGUIObject(ssGUI::GUIObject* obj)
    {
        if(obj->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            return;
        
        MainWindowPList.push_back(obj);

        BackendInput->UpdateCursor();
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
        ssGUI_LOG_FUNC();
        Internal_Update();
    }
    
    void ssGUIManager::StartRunningAsync()
    {
        InitializeMainWindows();
    }
    
    bool ssGUIManager::IsRunningNeededAsync()
    {
        //If MainWindow is closed before this function is called, this will updpate it
        //which will prevent crash from PollInputs().
        CheckMainWindowExistence();

        if(MainWindowPList.empty())
            return false;
        
        return true;
    }
    
    void ssGUIManager::PollInputsAsync()
    {
        BackendInput->UpdateInput();
        
        //Set cursor default to normal. Save last cursor so don't need to load cursor every frame
        LastCursor = BackendInput->GetCursorType();
        LastCursorName = BackendInput->GetCurrentCustomCursorName();
        BackendInput->SetCursorType(ssGUI::Enums::CursorType::NORMAL);
    }

    void ssGUIManager::InvokePreGUIObjectsUpdateEventAsync()
    {
        ssGUI_LOG_FUNC();
        for(int i = 0; i < PreGUIUpdateEventListeners.size(); i++)
        {                
            if(PreGUIUpdateEventListenersValid[i])
                PreGUIUpdateEventListeners[i]();
        }
    }
    
    void ssGUIManager::UpdateObjectsAsync()
    {
        ssGUI_LOG_FUNC();
        
        if(MainWindowPList.empty())
            return;

        std::stack<ssGUI::GUIObject*> objToUpdate;
        std::stack<bool> childrenEvaluated;
        std::queue<ssGUI::GUIObject*> updateQueue;

        //For now, update main window in order as it doesn't matter
        for(auto mainWindow : MainWindowPList)
        {            
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                ssGUI_WARNING(ssGUI_MANAGER_TAG, "Invalid object type added to gui manager");
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
                    
                    //ssGUI_DEBUG(ssGUI_MANAGER_TAG, "object "<<updateQueue.front()<<" checking validity");
                    //updateQueue.front()->Internal_GetObjectsReferences()->CheckObjectsReferencesValidity();
                }
                updateQueue.pop();
            }
        }
    }
    
    void ssGUIManager::CleanUpDeletedObjectsAsync()
    {
        CheckMainWindowExistence();
        
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
    }
    
    void ssGUIManager::InvokePostGUIObjectsUpdateEventAsync()
    {
        ssGUI_LOG_FUNC();
    
        if(!MainWindowPList.empty())
        {
            //Dispatch Update event
            for(int i = 0; i < PostGUIUpdateEventListeners.size(); i++)
            {                
                if(PostGUIUpdateEventListenersValid[i])
                    PostGUIUpdateEventListeners[i]();
            }
        }
    }
    
    void ssGUIManager::RenderObjectsAsync()
    {
        //Dispatch Custom Rendering event
        if(IsCustomRendering)
        {
            ssGUI_LOG_FUNC("ssGUIManagerCustomRenderingEvent");
            for(int i = 0; i < OnCustomRenderEventListeners.size(); i++)
            {
                if(OnCustomRenderEventListenersValid[i])
                    OnCustomRenderEventListeners[i](MainWindowPList);
            }
        }
        else
            Render();
    }
    
    void ssGUIManager::UpdateCursorAsync()
    {
        //Update the cursor if it is different from last frame
        if(LastCursor != BackendInput->GetCursorType() || 
            (LastCursor == ssGUI::Enums::CursorType::CUSTOM && LastCursorName != BackendInput->GetCurrentCustomCursorName()))
        {
            BackendInput->UpdateCursor();
        }
    }
    
    bool ssGUIManager::StepFrame()
    {
        if(!IsRunningNeededAsync())
            return false;
    
        {
            PollInputsAsync();

            //Dispatch Update event
            InvokePreGUIObjectsUpdateEventAsync();

            #ifdef SSGUI_LOG_MANAGER_STATE
                ssGUI_DEBUG(ssGUI_MANAGER_TAG, "Update");
            #endif

            UpdateObjectsAsync();

            CleanUpDeletedObjectsAsync();

            InvokePostGUIObjectsUpdateEventAsync();

            CleanUpDeletedObjectsAsync();

            #ifdef SSGUI_LOG_MANAGER_STATE
                ssGUI_DEBUG(ssGUI_MANAGER_TAG, "Render");
            #endif

            RenderObjectsAsync();        

            #ifdef SSGUI_LOG_MANAGER_STATE
                ssGUI_DEBUG(ssGUI_MANAGER_TAG, "Post Render");
            #endif

            UpdateCursorAsync();

            #if SSGUI_SLOW_UPDATE
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            #endif

            #if SSGUI_REFRESH_CONSOLE
                Clear();
            #endif
        }
        
        return true;
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
    
    int ssGUIManager::AddPreGUIRenderEventListener(std::function<void()> event)
    {
        return AddPostGUIUpdateEventListener(event);
    }

    void ssGUIManager::RemovePostGUIUpdateEventListener(int index)
    {
        if(index < 0 || index >= PostGUIUpdateEventListeners.size())
            return;
        
        PostGUIUpdateEventListenersValid[index] = false;
        PostGUIUpdateEventListenersNextFreeIndices.push(index);
    }
    
    void ssGUIManager::RemovePreGUIRenderEventListener(int index)
    {
        RemovePostGUIUpdateEventListener(index);    
    }

    int ssGUIManager::AddPostGUIRenderEventListener(std::function<void(ssGUI::MainWindow*)> event)
    {
        int addedIndex = -1;

        if(PostGUIRenderEventListenersNextFreeIndices.empty())
        {
            PostGUIRenderEventListeners.push_back(event);
            PostGUIRenderEventListenersValid.push_back(true);
            addedIndex = PostGUIRenderEventListeners.size() - 1;
        }
        else
        {
            addedIndex = PostGUIRenderEventListenersNextFreeIndices.front();
            PostGUIRenderEventListeners[PostGUIRenderEventListenersNextFreeIndices.front()] = event;
            PostGUIRenderEventListenersValid[PostGUIRenderEventListenersNextFreeIndices.front()] = true;
            PostGUIRenderEventListenersNextFreeIndices.pop();
        }

        return addedIndex;
    }

    void ssGUIManager::RemovePostGUIRenderEventListener(int index)
    {
        PostGUIRenderEventListenersValid[index] = false;
        PostGUIRenderEventListenersNextFreeIndices.push(index);
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

    void ssGUIManager::SetRedrawEveryFrame(bool redraw)
    {
        ForceRendering = redraw;
    }

    bool ssGUIManager::IsRedrawEveryFrame()
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
    
    bool ssGUIManager::IsButtonOrKeyDown(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return BackendInput->IsButtonOrKeyPressExistCurrentFrame(input) && !BackendInput->IsButtonOrKeyPressExistLastFrame(input);
    }
    
    bool ssGUIManager::IsButtonOrKeyHeld(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return BackendInput->IsButtonOrKeyPressExistCurrentFrame(input) && BackendInput->IsButtonOrKeyPressExistLastFrame(input);
    }
    
    bool ssGUIManager::IsButtonOrKeyUp(ssGUI::Enums::GenericButtonAndKeyInput input) const
    {
        return !BackendInput->IsButtonOrKeyPressExistCurrentFrame(input) && BackendInput->IsButtonOrKeyPressExistLastFrame(input);
    }
    
    glm::ivec2 ssGUIManager::GetMousePosition(ssGUI::MainWindow* mainWindow) const
    {
        return BackendInput->GetCurrentMousePosition(mainWindow == nullptr ? nullptr : mainWindow->GetBackendWindowInterface());   
    }
    
    glm::ivec2 ssGUIManager::GetMousePositionDelta(ssGUI::MainWindow* mainWindow) const
    {
        return  BackendInput->GetCurrentMousePosition(mainWindow == nullptr ? nullptr : mainWindow->GetBackendWindowInterface()) - 
                BackendInput->GetLastMousePosition(mainWindow == nullptr ? nullptr : mainWindow->GetBackendWindowInterface());
    }
    
    glm::vec2 ssGUIManager::GetMouseScrollDelta() const
    {
        return BackendInput->GetCurrentMouseScrollDelta();
    }
    
    void ssGUIManager::GetTextInput(std::wstring& textInput) const
    {
        textInput = BackendInput->GetTextInput();
    }
    
    void ssGUIManager::GetTextInput(std::string& textInput) const
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        textInput = converter.to_bytes(BackendInput->GetTextInput());
    }
    
    void ssGUIManager::SetCursorType(ssGUI::Enums::CursorType cursorType)
    {
        BackendInput->SetCursorType(cursorType);    
    }
    
    ssGUI::Enums::CursorType ssGUIManager::GetCursorType() const
    {
        return BackendInput->GetCursorType();
    }
    
    void ssGUIManager::CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot)
    {
        BackendInput->CreateCustomCursor(customCursor->GetBackendImageInterface(), cursorName, cursorSize, hotspot);
    }
    
    void ssGUIManager::SetCurrentCustomCursor(std::string cursorName)
    {
        BackendInput->SetCurrentCustomCursor(cursorName);
    }
    
    void ssGUIManager::GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot)
    {
        BackendInput->GetCurrentCustomCursor(*customCursor.GetBackendImageInterface(), hotspot);
    }
    
    void ssGUIManager::GetCurrentCustomCursorName(std::string& name)
    {
        name = BackendInput->GetCurrentCustomCursorName();
    }
    
    void ssGUIManager::GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot)
    {
        BackendInput->GetCustomCursor(*customCursor.GetBackendImageInterface(), cursorName, hotspot);
    }
    
    bool ssGUIManager::HasCustomCursor(const std::string& cursorName)
    {
        return BackendInput->HasCustomCursor(cursorName);
    }
    
    bool ssGUIManager::ClearClipboard()
    {
        return BackendInput->ClearClipboard();
    }
    
    bool ssGUIManager::ClipbaordHasText()
    {
        return BackendInput->ClipbaordHasText();
    }
    
    bool ssGUIManager::ClipbaordHasImage()
    {
        return BackendInput->ClipbaordHasImage();
    }
    
    bool ssGUIManager::SetClipboardImage(const ssGUI::ImageData& imgData)
    {
        return BackendInput->SetClipboardImage(*imgData.GetBackendImageInterface());
    }
    
    bool ssGUIManager::SetClipboardText(const std::wstring& str)
    {
        return BackendInput->SetClipboardText(str);
    }
    
    bool ssGUIManager::SetClipboardText(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return BackendInput->SetClipboardText(converter.from_bytes(str));
    }
    
    bool ssGUIManager::GetClipboardImage(ssGUI::ImageData& imgData)
    {
        return BackendInput->GetClipboardImage(*imgData.GetBackendImageInterface());
    }
    
    bool ssGUIManager::GetClipboardText(std::wstring& str)
    {
        return BackendInput->GetClipboardText(str);
    }
    
    bool ssGUIManager::GetClipboardText(std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring clipboardText;
        bool result = BackendInput->GetClipboardText(clipboardText);
        str = converter.to_bytes(clipboardText);
        return result;
    }
    
    uint64_t ssGUIManager::GetElapsedTimeInMillisecond() const
    {
        return BackendInput->GetElapsedTime();
    }
    
    void ssGUIManager::PrintGUIObjectTree() const
    {
        using depth = int;
        using lastSpaces = std::string;

        std::list<std::tuple<ssGUI::GUIObject*, depth, lastSpaces>> childrenToPrint;
        
        for(auto it = MainWindowPList.begin(); it != MainWindowPList.end(); --it)
            childrenToPrint.push_back(std::make_tuple(*it, 0, ""));

        while(!childrenToPrint.empty()) 
        {
            ssGUI::GUIObject* curObj = std::get<0>(childrenToPrint.front());
            int curDepth = std::get<1>(childrenToPrint.front());
            std::string lastSpaces = std::get<2>(childrenToPrint.front());
            childrenToPrint.pop_front();
            
            bool hasNextWithSameDepth = childrenToPrint.empty() ? false : (std::get<1>(childrenToPrint.front()) == curDepth);
            
            ssLOG_LINE(lastSpaces << "|   ");
            ssLOG_LINE(lastSpaces << "|---" << "GUI Object: " << curObj);
            ssLOG_LINE(lastSpaces << (hasNextWithSameDepth ? "|   " : "    " ) << "Type: " << ssGUI::Enums::GUIObjectTypeToString(curObj->GetType())); 
            
            //Add the children to the list
            std::vector<ssGUI::GUIObject*> curChildren = curObj->GetListOfChildren();
            
            std::string nextSpaces = lastSpaces;
            if(hasNextWithSameDepth)
                nextSpaces += "|   ";
            else
                nextSpaces += "    ";
            
            for(int i = curChildren.size() - 1; i >= 0; --i)
                childrenToPrint.push_front(std::make_tuple(curChildren.at(i), curDepth + 1, nextSpaces)); 
        }
    }
    
    void ssGUIManager::Clear()
    {
        #if defined (_WIN32)
            system("cls");
            //clrscr(); // including header file : conio.h
        #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
            system("clear");
            //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
        #elif defined (__APPLE__)
            system("clear");
        #endif
    }
    
}
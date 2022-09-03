#include "ssGUI/ssGUIManager.hpp"


//Debug
#include "ssGUI/Extensions/Layout.hpp"
#include <thread>
#include <chrono>



namespace ssGUI
{
    void ssGUIManager::Internal_Update()
    {        
        //Render to allow Main Window to clear the window with background color first
        //Render();
        for(auto mainWindow : MainWindowPList)
        {
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                DEBUG_LINE("Invalid object type added to gui manager");
                continue;
            }

            if(!mainWindow->IsVisible())
                continue;

            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);

            currentMainWindowP->Internal_Draw();
            currentMainWindowP->ClearBackBuffer();
        }

        while (!MainWindowPList.empty())
        {
            PollInputs();

            //Clear up any main windows that are closed
            CheckMainWindowExistence();

            //Set cursor default to normal. Save last cursor so don't need to load cursor every frame
            ssGUI::Enums::CursorType lastCursor = BackendInput->GetCursorType();
            std::string lastCustomCursorName = BackendInput->GetCurrentCustomCursorName();
            BackendInput->SetCursorType(ssGUI::Enums::CursorType::NORMAL);

            //Dispatch Update event
            FUNC_DEBUG_ENTRY("ssGUIManagerPreUpdateEvent");
            for(int i = 0; i < PreGUIUpdateEventListeners.size(); i++)
            {                
                if(PreGUIUpdateEventListenersValid[i])
                    PreGUIUpdateEventListeners[i]();
            }
            FUNC_DEBUG_EXIT("ssGUIManagerPreUpdateEvent");

            #if DEBUG_STATE 
                DEBUG_LINE("Update");
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
            FUNC_DEBUG_ENTRY("ssGUIManagerPostUpdateEvent");
            for(int i = 0; i < PostGUIUpdateEventListeners.size(); i++)
            {                
                if(PostGUIUpdateEventListenersValid[i])
                    PostGUIUpdateEventListeners[i]();
            }
            FUNC_DEBUG_EXIT("ssGUIManagerPostUpdateEvent");

            #if DEBUG_STATE 
                DEBUG_LINE("Render");
            #endif

            // //Dispatch Update event
            // FUNC_DEBUG_ENTRY("ssGUIManagerCustomUpdateEvent");
            // for(int i = 0; i < OnUpdateEventListeners.size(); i++)
            // {                
            //     if(OnUpdateEventListenersValid[i])
            //         OnUpdateEventListeners[i]();
            // }
            // FUNC_DEBUG_EXIT("ssGUIManagerCustomUpdateEvent");

            //Dispatch Custom Rendering event
            if(IsCustomRendering)
            {
                FUNC_DEBUG_ENTRY("ssGUIManagerCustomRenderingEvent");
                for(int i = 0; i < OnCustomRenderEventListeners.size(); i++)
                {
                    if(OnCustomRenderEventListenersValid[i])
                        OnCustomRenderEventListeners[i](MainWindowPList);
                }
                FUNC_DEBUG_EXIT("ssGUIManagerCustomRenderingEvent");
            }
            else
                Render();
            
            #if DEBUG_STATE 
                DEBUG_LINE("Post Render");
            #endif

            if(lastCursor != BackendInput->GetCursorType() || 
                (lastCursor == ssGUI::Enums::CursorType::CUSTOM && lastCustomCursorName != BackendInput->GetCurrentCustomCursorName()))
            {
                UpdateCursor();   
            }

            /* Don't know that this code is for. Seems like UpdateCursor is already got this sorted
            for(auto obj : MainWindowPList)
            {
                static_cast<ssGUI::Backend::BackendMainWindowSFML&>(dynamic_cast<ssGUI::MainWindow*>(obj)->
                GetBackendWindowInterface()).GetSFWindow().setMouseCursorVisible(true);
            }
            */
            #if SLOW_UPDATE
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            #endif

            #if REFRESH_CONSOLE
                Clear();
            #endif
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
    
    void ssGUIManager::PollInputs()
    {
        BackendInput->UpdateInput();
    }
    
    void ssGUIManager::CheckMainWindowExistence()
    {
        FUNC_DEBUG_ENTRY();
        
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

        FUNC_DEBUG_EXIT();
    }

    void ssGUIManager::Render()
    {        
        FUNC_DEBUG_ENTRY();
        
        if(MainWindowPList.empty())
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        for(auto mainWindow : MainWindowPList)
        {
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                DEBUG_LINE("Invalid object type added to gui manager");
                continue;
            }

            if(!mainWindow->IsVisible())
                continue;

            std::list<ssGUI::GUIObject*> objToRender;
            std::list<ssGUI::GUIObject*> renderQueue;
            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);

            currentMainWindowP->Internal_Draw();

            //Populate the render queue first
            currentMainWindowP->StashChildrenIterator();
            currentMainWindowP->MoveChildrenIteratorToFirst();
            while (!currentMainWindowP->IsChildrenIteratorEnd())
            {
                objToRender.push_back(currentMainWindowP->GetCurrentChild());
                currentMainWindowP->MoveChildrenIteratorNext();
            }
            currentMainWindowP->PopChildrenIterator();

            while (!objToRender.empty())
            {
                ssGUI::GUIObject* currentObjP = objToRender.front();
                objToRender.pop_front();
                renderQueue.push_back(currentObjP);

                //Add children to draw queue
                if(currentObjP->GetChildrenCount() > 0)
                {
                    currentObjP->StashChildrenIterator();
                    //Add children from back to front so that the drawing queue order is front to back
                    currentObjP->MoveChildrenIteratorToLast();
                    while (!currentObjP->IsChildrenIteratorEnd())
                    {
                        objToRender.push_front(currentObjP->GetCurrentChild());
                        currentObjP->MoveChildrenIteratorPrevious();
                    }
                    currentObjP->PopChildrenIterator();
                }
            }

            //Check if render is needed
            bool renderNeeded = false;

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
            }
            else
                renderNeeded = true;

            //Render if needed
            if(renderNeeded)
            {
                #if USE_DEBUG
                    DEBUG_LINE("Rendering");
                #endif

                while(!renderQueue.empty())
                {
                    ssGUI::GUIObject* currentObjP = renderQueue.front();

                    //Remove current gui object from render queue
                    renderQueue.pop_front();

                    if(currentObjP->Internal_IsDeleted())
                        continue;

                    (currentObjP)->Internal_Draw(   currentMainWindowP->GetBackendDrawingInterface(), 
                                                    dynamic_cast<ssGUI::GUIObject*>(currentMainWindowP), 
                                                    currentMainWindowP->GetPositionOffset());
                }

                //Dispatch Post Rendering Update event
                FUNC_DEBUG_ENTRY("ssGUIManagerPostRenderingUpdateEvent");
                for(int i = 0; i < PostGUIRenderingUpdateEventListeners.size(); i++)
                {
                    if(PostGUIRenderingUpdateEventListenersValid[i])
                        PostGUIRenderingUpdateEventListeners[i]();
                }
                FUNC_DEBUG_EXIT("ssGUIManagerPostRenderingUpdateEvent");

                //Draw everything that is displayed on the mainWindow back buffer
                currentMainWindowP->Render();
            }
            else
            {
                #if USE_DEBUG
                    DEBUG_LINE("Caching");
                #endif

                currentMainWindowP->ClearBackBuffer();
            }
        }
        FUNC_DEBUG_EXIT();
    }

    void ssGUIManager::UpdateObjects()
    {
        FUNC_DEBUG_ENTRY();
        
        if(MainWindowPList.empty())
        {
            FUNC_DEBUG_EXIT();
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
                DEBUG_LINE("Invalid object type added to gui manager");
                continue;
            }
            if(mainWindow->Internal_IsDeleted() || static_cast<ssGUI::Window*>(mainWindow)->IsClosed())
                continue;

            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);

            //Populate the update stack first
            /*
            for(std::list<ssGUI::GUIObject*>::iterator childIt = currentMainWindowP->GetChildrenStartIterator();
                childIt != currentMainWindowP->GetChildrenEndIterator(); childIt++)
            {
                objToUpdate.push(*childIt);
                childrenEvaluated.push(false);
            }
            */
            
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
                    DEBUG_LINE("object "<<updateQueue.front()<<" checking validity");
                    updateQueue.front()->Internal_GetObjectsReferences()->CheckObjectsReferencesValidity();
                    #endif
                }
                updateQueue.pop();
            }
        }

        FUNC_DEBUG_EXIT();
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

    // void ssGUIManager::AssginParentToChildren(ssGUI::GUIObject& targetObj, ssGUI::GUIObject* newParentP)
    // {        
    //     targetObj.MoveChildrenIteratorToFirst();
    //     while (targetObj.IsChildrenIteratorEnd())
    //     {
    //         targetObj.GetCurrentChild()->SetParent(newParentP);
    //         targetObj.MoveChildrenIteratorNext();
    //     }
    // }

    ssGUI::ssGUIManager* ssGUIManager::CurrentInstanceP = nullptr;

    ssGUIManager::ssGUIManager() :  BackendInput(), MainWindowPList(), PreGUIUpdateEventListeners(), 
                                    PreGUIUpdateEventListenersValid(), PreGUIUpdateEventListenersNextFreeIndices(),
                                    PostGUIUpdateEventListeners(), PostGUIUpdateEventListenersValid(), 
                                    PostGUIUpdateEventListenersNextFreeIndices(),
                                    PostGUIRenderingUpdateEventListeners(), PostGUIRenderingUpdateEventListenersValid(), 
                                    PostGUIRenderingUpdateEventListenersNextFreeIndices(), OnCustomRenderEventListeners(),
                                    OnCustomRenderEventListenersValid(), OnCustomRenderEventListenersNextFreeIndices(), 
                                    IsCustomRendering(false)
    {
        BackendInput = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
        CurrentInstanceP = this;
    }

    ssGUIManager::~ssGUIManager()
    {
        delete BackendInput;

        //TODO : Remove all non user created objects
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

    /* void ssGUIManager::ChangeGUIObjectOrder(ssGUI::GUIObject& obj, int order)
    {
        if(order >= GUIObjectPList.size() || order < 0)
            return;
        
        int currentOrder = GetGUIObjectOrder(obj);
        if(currentOrder < 0)
            return;

        ssGUI::GUIObject* curObjP = GUIObjectPList[currentOrder];
        GUIObjectPList.erase(GUIObjectPList.begin() + currentOrder);
        GUIObjectPList.insert(GUIObjectPList.begin() + order, curObjP);
    }

    int ssGUIManager::GetGUIObjectOrder(ssGUI::GUIObject& obj)
    {
        std::vector<ssGUI::GUIObject*>::iterator it;
        it = std::find_if(GUIObjectPList.begin(), GUIObjectPList.end(), 
                            [&obj](ssGUI::GUIObject* curObjP){return &obj == curObjP;});

        if(it == GUIObjectPList.end())
            return -1;
        else
            return it - GUIObjectPList.begin();
    } */

    int ssGUIManager::GetGUIObjectCount()
    {
        return MainWindowPList.size();
    }

    void ssGUIManager::StartRunning()
    {
        Internal_Update();
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
}
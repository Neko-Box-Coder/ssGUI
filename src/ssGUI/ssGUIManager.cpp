#include "ssGUI/ssGUIManager.hpp"


//Debug
#include "ssGUI/Extensions/Layout.hpp"



namespace ssGUI
{
    void ssGUIManager::Internal_Update()
    {        
        //Render to allow Main Window to clear the window with background color
        Render();
        
        while (!MainWindowPList.empty())
        {
            PollInputs();

            //Set cursor default to normal. Save last cursor so don't need to load cursor every frame
            ssGUI::Enums::CursorType lastCursor = BackendInput->GetCursorType();
            BackendInput->SetCursorType(ssGUI::Enums::CursorType::NORMAL);

            #if DEBUG_STATE 
                std::cout<<"Update\n";
            #endif

            UpdateObjects();

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

            #if DEBUG_STATE 
                std::cout<<"\nRender\n";
            #endif

            //Clear up any main windows that are closed
            CheckMainWindowExistence();

            //Dispatch Update event
            FUNC_DEBUG_ENTRY("ssGUIManagerCustomUpdateEvent");
            for(int i = 0; i < OnUpdateEventListeners.size(); i++)
            {                
                if(OnUpdateEventListenersValid[i])
                    OnUpdateEventListeners[i]();
            }
            FUNC_DEBUG_EXIT("ssGUIManagerCustomUpdateEvent");

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
                std::cout<<"\nPost Render\n";
            #endif

            //Dispatch Post Rendering Update event
            FUNC_DEBUG_ENTRY("ssGUIManagerPostRenderingUpdateEvent");
            for(int i = 0; i < OnPostRenderingUpdateEventListeners.size(); i++)
            {
                if(OnPostRenderingUpdateEventListenersValid[i])
                    OnPostRenderingUpdateEventListeners[i]();
            }
            FUNC_DEBUG_EXIT("ssGUIManagerPostRenderingUpdateEvent");

            if(lastCursor != BackendInput->GetCursorType())
                UpdateCursor();

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
        }
    }
    
    void ssGUIManager::PollInputs()
    {
        BackendInput->UpdateInput();
    }
    
    void ssGUIManager::CheckMainWindowExistence()
    {
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
    }

    void ssGUIManager::GUIObjectsEventCallback()
    {
        //TODO: Not sure if this function is needed
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
            std::list<ssGUI::GUIObject*> objToRender;
            std::list<ssGUI::GUIObject*> renderQueue;

            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                DEBUG_LINE("Invalid object type added to gui manager");
                continue;
            }

            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);

            currentMainWindowP->Internal_Draw();

            //Populate the render queue first
            currentMainWindowP->MoveChildrenIteratorToFirst();
            while (!currentMainWindowP->IsChildrenIteratorEnd())
            {
                objToRender.push_back(currentMainWindowP->GetCurrentChild());
                currentMainWindowP->MoveChildrenIteratorNext();
            }

            while (!objToRender.empty())
            {
                ssGUI::GUIObject* currentObjP = objToRender.front();
                objToRender.pop_front();

                //Internal_Draw the gui object only when it is visible
                if(currentObjP->IsVisible())
                {                    
                    renderQueue.push_back(currentObjP);

                    //Add children to draw queue
                    if(currentObjP->GetChildrenCount() > 0)
                    {
                        //Add children from back to front so that the drawing queue order is front to back
                        currentObjP->MoveChildrenIteratorToLast();
                        while (!currentObjP->IsChildrenIteratorEnd())
                        {
                            objToRender.push_front(currentObjP->GetCurrentChild());
                            currentObjP->MoveChildrenIteratorPrevious();
                        }
                    }
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
            ssGUI::InputStatus globalInputStatus;
            ssGUI::InputStatus windowInputStatus;

            while (!objToUpdate.empty())
            {
                //Update object if there's no children or the children are already evaluated
                if(objToUpdate.top()->GetChildrenCount() == 0 || childrenEvaluated.top() == true)
                {                    
                    //Check if parent window is different, if so reset block window input
                    ssGUI::GUIObject* currentParentP = FindParentWindowP(*objToUpdate.top());
                    if(currentParentP != parentWindowP)
                    {
                        parentWindowP = currentParentP;
                        windowInputStatus.KeyInputBlocked = false;
                        windowInputStatus.MouseInputBlocked = false;
                        windowInputStatus.DockingBlocked = false;
                    }
                    
                    //objToUpdate.top()->Internal_Update(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(BackendInput), globalInputStatus, windowInputStatus, mainWindow);
                    updateQueue.push(objToUpdate.top());
                    objToUpdate.pop();
                    childrenEvaluated.pop();
                }
                //Otherwise add all the children
                else
                {                    
                    childrenEvaluated.top() = true;

                    ssGUI::GUIObject* currentObjP = objToUpdate.top();
                    currentObjP->MoveChildrenIteratorToFirst();
                    while (!currentObjP->IsChildrenIteratorEnd())
                    {
                        objToUpdate.push(currentObjP->GetCurrentChild());
                        childrenEvaluated.push(false);
                        currentObjP->MoveChildrenIteratorNext();
                    }
                }
            }

            while (!updateQueue.empty())
            {
                if(!updateQueue.front()->Internal_IsDeleted())
                {
                    updateQueue.front()->Internal_Update(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(BackendInput), globalInputStatus, windowInputStatus, mainWindow);
                    
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

    void ssGUIManager::AssginParentToChildren(ssGUI::GUIObject& targetObj, ssGUI::GUIObject* newParentP)
    {        
        targetObj.MoveChildrenIteratorToFirst();
        while (targetObj.IsChildrenIteratorEnd())
        {
            targetObj.GetCurrentChild()->SetParent(newParentP);
            targetObj.MoveChildrenIteratorNext();
        }
    }

    ssGUI::ssGUIManager* ssGUIManager::CurrentInstanceP = nullptr;

    ssGUIManager::ssGUIManager() :  BackendInput(), MainWindowPList(), OnUpdateEventListeners(), 
                                    OnUpdateEventListenersValid(), OnUpdateEventListenersNextFreeIndices(),
                                    OnPostRenderingUpdateEventListeners(), OnPostRenderingUpdateEventListenersValid(), 
                                    OnPostRenderingUpdateEventListenersNextFreeIndices(), OnCustomRenderEventListeners(),
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

    int ssGUIManager::AddOnUpdateEventListener(std::function<void()> event)
    {
        int addedIndex = -1;

        if(OnUpdateEventListenersNextFreeIndices.empty())
        {
            OnUpdateEventListeners.push_back(event);
            OnUpdateEventListenersValid.push_back(true);
            addedIndex = OnUpdateEventListeners.size() - 1;
        }
        else
        {
            addedIndex = OnUpdateEventListenersNextFreeIndices.front();
            OnUpdateEventListeners[OnUpdateEventListenersNextFreeIndices.front()] = event;
            OnUpdateEventListenersValid[OnUpdateEventListenersNextFreeIndices.front()] = true;
            OnUpdateEventListenersNextFreeIndices.pop();
        }

        return addedIndex;
    }

    void ssGUIManager::RemoveOnUpdateEventListener(int index)
    {
        OnUpdateEventListenersValid[index] = false;
        OnUpdateEventListenersNextFreeIndices.push(index);
    }

    int ssGUIManager::AddOnPostRenderingUpdateEventListener(std::function<void()> event)
    {
        int addedIndex = -1;

        if(OnPostRenderingUpdateEventListenersNextFreeIndices.empty())
        {
            OnPostRenderingUpdateEventListeners.push_back(event);
            OnPostRenderingUpdateEventListenersValid.push_back(true);
            addedIndex = OnPostRenderingUpdateEventListeners.size() - 1;
        }
        else
        {
            addedIndex = OnPostRenderingUpdateEventListenersNextFreeIndices.front();
            OnPostRenderingUpdateEventListeners[OnPostRenderingUpdateEventListenersNextFreeIndices.front()] = event;
            OnPostRenderingUpdateEventListenersValid[OnPostRenderingUpdateEventListenersNextFreeIndices.front()] = true;
            OnPostRenderingUpdateEventListenersNextFreeIndices.pop();
        }

        return addedIndex;
    }

    void ssGUIManager::RemoveOnPostRenderingUpdateEventListener(int index)
    {
        OnPostRenderingUpdateEventListenersValid[index] = false;
        OnPostRenderingUpdateEventListenersNextFreeIndices.push(index);
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
#include "ssGUI/ssGUIManager.hpp"


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

            #if USE_DEBUG 
                std::cout<<"Update\n";
            #endif

            UpdateObjects();

            #if USE_DEBUG 
                std::cout<<"\nRender\n";
            #endif

            //Clear up any main windows that are closed
            CheckMainWindowExistence();

            //Dispatch Update event
            for(int i = 0; i < OnUpdateEventListeners.size(); i++)
            {                
                if(OnUpdateEventListenersValid[i])
                    OnUpdateEventListeners[i]();
            }

            //Dispatch Custom Rendering event
            if(IsCustomRendering)
            {
                for(int i = 0; i < OnCustomRenderEventListeners.size(); i++)
                {
                    if(OnCustomRenderEventListenersValid[i])
                        OnCustomRenderEventListeners[i](MainWindowPList);
                }
            }
            else
                Render();
            
            #if USE_DEBUG 
                std::cout<<"\nPost Render\n";
            #endif

            //Dispatch Post Rendering Update event
            for(int i = 0; i < OnPostRenderingUpdateEventListeners.size(); i++)
            {
                if(OnPostRenderingUpdateEventListenersValid[i])
                    OnPostRenderingUpdateEventListeners[i]();
            }

            if(lastCursor != BackendInput->GetCursorType())
                UpdateCursor();

            /* Don't know that this code is for. Seems like UpdateCursor is already got this sorted
            for(auto obj : MainWindowPList)
            {
                static_cast<ssGUI::Backend::BackendMainWindowSFML&>(dynamic_cast<ssGUI::MainWindow*>(obj)->
                GetBackendWindowInterface()).GetSFWindow().setMouseCursorVisible(true);
            }
            */

            #if REFRESH_CONSOLE
                Clear();
            #endif

            #if SLOW_UPDATE
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
            if(static_cast<ssGUI::MainWindow*>(windowP)->IsClosed())
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
        if(MainWindowPList.empty())
            return;

        std::list<ssGUI::GUIObject*> objToRender;

        for(auto mainWindow : MainWindowPList)
        {
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                std::cout << "Invalid object type added to gui manager\n";
                continue;
            }

            ssGUI::MainWindow* currentMainWindowP = dynamic_cast<ssGUI::MainWindow*>(mainWindow);

            currentMainWindowP->Draw();

            //Populate the render queue first
            for(std::list<ssGUI::GUIObject*>::iterator childIt = currentMainWindowP->GetChildrenStartIterator();
                childIt != currentMainWindowP->GetChildrenEndIterator(); childIt++)
            {
                objToRender.push_back(*childIt);
            }

            while (!objToRender.empty())
            {
                ssGUI::GUIObject* currentObjP = objToRender.front();
                //Remove current gui object from draw queue
                objToRender.pop_front();

                //Draw the gui object only when it is visible
                if(currentObjP->IsVisible())
                {
                    (currentObjP)->Draw(    currentMainWindowP->GetBackendDrawingInterface(), 
                                            dynamic_cast<ssGUI::GUIObject*>(currentMainWindowP), 
                                            currentMainWindowP->GetPositionOffset());

                    //Add children to draw queue
                    if(currentObjP->GetChildrenCount() > 0)
                    {
                        //Add children from back to front so that the drawing queue order is front to back
                        std::list<ssGUI::GUIObject*>::iterator childIt = currentObjP->GetChildrenEndIterator();
                        childIt--;
                        std::list<ssGUI::GUIObject*>::iterator childTerminateIt = currentObjP->GetChildrenStartIterator();
                        childTerminateIt--;

                        for(;childIt != childTerminateIt; childIt--)
                        {
                            objToRender.push_front(*childIt);
                        }
                    }                
                }
            }

            //Draw everything that is displayed on the mainWindow buffer
            currentMainWindowP->Render();
        }
    }

    void ssGUIManager::UpdateObjects()
    {
        if(MainWindowPList.empty())
            return;

        std::stack<ssGUI::GUIObject*> objToUpdate;
        std::stack<bool> childrenEvaluated;

        //For now, update main window in order as it doesn't matter
        for(auto mainWindow : MainWindowPList)
        {            
            if(mainWindow->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
            {
                std::cout << "Invalid object type added to gui manager\n";
                continue;
            }
            if(static_cast<ssGUI::Window*>(mainWindow)->IsClosed())
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
                    }
                    
                    objToUpdate.top()->Internal_Update(static_cast<ssGUI::Backend::BackendSystemInputInterface*>(BackendInput), globalInputStatus, windowInputStatus, mainWindow);

                    objToUpdate.pop();
                    childrenEvaluated.pop();
                }
                //Otherwise add all the children
                else
                {                    
                    childrenEvaluated.top() = true;

                    ssGUI::GUIObject* currentObjP = objToUpdate.top();
                    for(std::list<ssGUI::GUIObject*>::iterator childIt = currentObjP->GetChildrenStartIterator();
                        childIt != currentObjP->GetChildrenEndIterator(); childIt++)
                    {
                        objToUpdate.push(*childIt);
                        childrenEvaluated.push(false);
                    }
                }
            }
        }
    }

    void ssGUIManager::UpdateCursor()
    {
        //Update the cursor
        BackendInput->UpdateCursor();
    }

    ssGUI::GUIObject* ssGUIManager::FindParentWindowP(ssGUI::GUIObject& obj)
    {        
        ssGUI::GUIObject* currentParentP = obj.GetParentP();

        while (currentParentP != nullptr && currentParentP->GetType() != ssGUI::Enums::GUIObjectType::WINDOW)
            currentParentP = currentParentP->GetParentP();
        
        if(currentParentP != nullptr && currentParentP->GetType() != ssGUI::Enums::GUIObjectType::WINDOW)
            return nullptr;

        return currentParentP;
    }

    void ssGUIManager::AssginParentToChildren(ssGUI::GUIObject& targetObj, ssGUI::GUIObject* newParentP)
    {
        for(auto it = targetObj.GetChildrenStartIterator(); it != targetObj.GetChildrenEndIterator(); it++)
            (*it)->SetParentP(newParentP);
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
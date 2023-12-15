#include "ssGUI/Backend/BackendManager.hpp"


namespace ssGUI
{

namespace Backend
{
    int BackendManager::MainWindowCount = 0;
    std::vector<BackendDrawingInterface*> BackendManager::BackendDrawingInterfaces;
    std::vector<BackendMainWindowInterface*> BackendManager::BackendMainWindowInterfaces;
    BackendSystemInputInterface* BackendManager::CurrentBackendSystemInputInterface = nullptr;
    
    void BackendManager::AddDrawingInterface(BackendDrawingInterface* backendDrawing)
    {
        BackendDrawingInterfaces.push_back(backendDrawing);
    }

    void BackendManager::RemoveDrawingInterface(BackendDrawingInterface* backendDrawing)
    {
        for(int i = 0; i < BackendDrawingInterfaces.size(); i++)
        {
            if(BackendDrawingInterfaces.at(i) == backendDrawing)
            {
                BackendDrawingInterfaces.erase(BackendDrawingInterfaces.begin() + i);
            }
        }
    }

    BackendDrawingInterface* BackendManager::GetDrawingInterface(int index)
    {
        return BackendDrawingInterfaces.at(index);
    }

    int BackendManager::GetDrawingInterfaceIndex(BackendDrawingInterface* backendDrawing)
    {
        for(int i = 0; i < BackendDrawingInterfaces.size(); i++)
        {
            if(BackendDrawingInterfaces.at(i) == backendDrawing)
                return i;
        }

        return -1;
    }

    void BackendManager::AddInputInterface(BackendSystemInputInterface* backendInput)
    {
        CurrentBackendSystemInputInterface = backendInput;
    }

    void BackendManager::RemoveInputInterface(BackendSystemInputInterface* backendInput)
    {
        CurrentBackendSystemInputInterface = nullptr;
    }

    BackendSystemInputInterface* BackendManager::GetInputInterface()
    {
        return CurrentBackendSystemInputInterface;
    }

    void BackendManager::AddMainWindowInterface(BackendMainWindowInterface* backendMainWindow)
    {
        BackendMainWindowInterfaces.push_back(backendMainWindow);
    }

    void BackendManager::RemoveMainWindowInterface(BackendMainWindowInterface* backendMainWindow)
    {
        for(int i = 0; i < BackendMainWindowInterfaces.size(); i++)
        {
            if(BackendMainWindowInterfaces.at(i) == backendMainWindow)
            {
                BackendMainWindowInterfaces.erase(BackendMainWindowInterfaces.begin() + i);
            }
        }
    }

    BackendMainWindowInterface* BackendManager::GetMainWindowInterface(int index)
    {
        assert(index >= 0 && index < BackendMainWindowInterfaces.size());
        return BackendMainWindowInterfaces.at(index);
    }

    int BackendManager::GetMainWindowInterfaceIndex(BackendMainWindowInterface* backendMainWindow)
    {
        for(int i = 0; i < BackendMainWindowInterfaces.size(); i++)
        {
            if(BackendMainWindowInterfaces.at(i) == backendMainWindow)
                return i;
        }

        return -1; 
    }

    int BackendManager::GetMainWindowCount()
    {
        return BackendMainWindowInterfaces.size();
    }
}

}
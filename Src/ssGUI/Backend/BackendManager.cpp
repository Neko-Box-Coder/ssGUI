#include "ssGUI/Backend/BackendManager.hpp"


namespace ssGUI
{

namespace Backend
{
    int BackendManager::MainWindowCount = 0;
    std::vector<ssGUI::Backend::BackendDrawingInterface*> BackendManager::BackendDrawingInterfaces;
    std::vector<ssGUI::Backend::BackendMainWindowInterface*> BackendManager::BackendMainWindowInterfaces;
    ssGUI::Backend::BackendSystemInputInterface* BackendManager::BackendSystemInputInterface = nullptr;
    
    void BackendManager::AddDrawingInterface(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        BackendDrawingInterfaces.push_back(backendDrawing);
    }

    void BackendManager::RemoveDrawingInterface(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        for(int i = 0; i < BackendDrawingInterfaces.size(); i++)
        {
            if(BackendDrawingInterfaces.at(i) == backendDrawing)
            {
                BackendDrawingInterfaces.erase(BackendDrawingInterfaces.begin() + i);
            }
        }
    }

    ssGUI::Backend::BackendDrawingInterface* BackendManager::GetDrawingInterface(int index)
    {
        return BackendDrawingInterfaces.at(index);
    }

    int BackendManager::GetDrawingInterfaceIndex(ssGUI::Backend::BackendDrawingInterface* backendDrawing)
    {
        for(int i = 0; i < BackendDrawingInterfaces.size(); i++)
        {
            if(BackendDrawingInterfaces.at(i) == backendDrawing)
                return i;
        }

        return -1;
    }

    void BackendManager::AddInputInterface(ssGUI::Backend::BackendSystemInputInterface* backendInput)
    {
        BackendSystemInputInterface = backendInput;
    }

    void BackendManager::RemoveInputInterface(ssGUI::Backend::BackendSystemInputInterface* backendInput)
    {
        BackendSystemInputInterface = nullptr;
    }

    ssGUI::Backend::BackendSystemInputInterface* BackendManager::GetInputInterface()
    {
        return BackendSystemInputInterface;
    }

    void BackendManager::AddMainWindowInterface(ssGUI::Backend::BackendMainWindowInterface* backendMainWindow)
    {
        BackendMainWindowInterfaces.push_back(backendMainWindow);
    }

    void BackendManager::RemoveMainWindowInterface(ssGUI::Backend::BackendMainWindowInterface* backendMainWindow)
    {
        for(int i = 0; i < BackendMainWindowInterfaces.size(); i++)
        {
            if(BackendMainWindowInterfaces.at(i) == backendMainWindow)
            {
                BackendMainWindowInterfaces.erase(BackendMainWindowInterfaces.begin() + i);
            }
        }
    }

    ssGUI::Backend::BackendMainWindowInterface* BackendManager::GetMainWindowInterface(int index)
    {
        assert(index >= 0 && index < BackendMainWindowInterfaces.size());
        return BackendMainWindowInterfaces.at(index);
    }

    int BackendManager::GetMainWindowInterfaceIndex(ssGUI::Backend::BackendMainWindowInterface* backendMainWindow)
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
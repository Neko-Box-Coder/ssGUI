#include "ssGUI/Backend/BackendManager.hpp"


namespace ssGUI
{

namespace Backend
{
    int BackendManager::MainWindowCount = 0;
    std::vector<DrawingInterface*> BackendManager::DrawingInterfaces;
    std::vector<MainWindowInterface*> BackendManager::MainWindowInterfaces;
    SystemInputInterface* BackendManager::CurrentSystemInputInterface = nullptr;
    
    void BackendManager::AddDrawingInterface(DrawingInterface* backendDrawing)
    {
        DrawingInterfaces.push_back(backendDrawing);
    }

    void BackendManager::RemoveDrawingInterface(DrawingInterface* backendDrawing)
    {
        for(int i = 0; i < DrawingInterfaces.size(); i++)
        {
            if(DrawingInterfaces.at(i) == backendDrawing)
            {
                DrawingInterfaces.erase(DrawingInterfaces.begin() + i);
            }
        }
    }

    DrawingInterface* BackendManager::GetDrawingInterface(int index)
    {
        return DrawingInterfaces.at(index);
    }

    int BackendManager::GetDrawingInterfaceIndex(DrawingInterface* backendDrawing)
    {
        for(int i = 0; i < DrawingInterfaces.size(); i++)
        {
            if(DrawingInterfaces.at(i) == backendDrawing)
                return i;
        }

        return -1;
    }

    void BackendManager::AddInputInterface(SystemInputInterface* backendInput)
    {
        CurrentSystemInputInterface = backendInput;
    }

    void BackendManager::RemoveInputInterface(SystemInputInterface* backendInput)
    {
        CurrentSystemInputInterface = nullptr;
    }

    SystemInputInterface* BackendManager::GetInputInterface()
    {
        return CurrentSystemInputInterface;
    }

    void BackendManager::AddMainWindowInterface(MainWindowInterface* backendMainWindow)
    {
        MainWindowInterfaces.push_back(backendMainWindow);
    }

    void BackendManager::RemoveMainWindowInterface(MainWindowInterface* backendMainWindow)
    {
        for(int i = 0; i < MainWindowInterfaces.size(); i++)
        {
            if(MainWindowInterfaces.at(i) == backendMainWindow)
            {
                MainWindowInterfaces.erase(MainWindowInterfaces.begin() + i);
            }
        }
    }

    MainWindowInterface* BackendManager::GetMainWindowInterface(int index)
    {
        assert(index >= 0 && index < MainWindowInterfaces.size());
        return MainWindowInterfaces.at(index);
    }

    int BackendManager::GetMainWindowInterfaceIndex(MainWindowInterface* backendMainWindow)
    {
        for(int i = 0; i < MainWindowInterfaces.size(); i++)
        {
            if(MainWindowInterfaces.at(i) == backendMainWindow)
                return i;
        }

        return -1; 
    }

    int BackendManager::GetMainWindowCount()
    {
        return MainWindowInterfaces.size();
    }
}

}
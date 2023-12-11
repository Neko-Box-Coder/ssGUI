#ifndef SSGUI_BACKEND_MANAGER_H
#define SSGUI_BACKEND_MANAGER_H

#include <vector>
#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    /*class: ssGUI::Backend::BackendManager
    Backend manager allows backend objects to communicate to each other, without reference each other. 
    Normally you don't need to deal with this class.
    
    Variables & Constructor:
    ============================== C++ ==============================
    private:
            static int MainWindowCount;
            static std::vector<BackendDrawingInterface*> BackendDrawingInterfaces;
            static std::vector<BackendMainWindowInterface*> BackendMainWindowInterfaces;
            static BackendSystemInputInterface* CurrentBackendSystemInputInterface;
    =================================================================
    ============================== C++ ==============================
    int BackendManager::MainWindowCount = 0;
    std::vector<BackendDrawingInterface*> BackendManager::BackendDrawingInterfaces;
    std::vector<BackendMainWindowInterface*> BackendManager::BackendMainWindowInterfaces;
    BackendSystemInputInterface* BackendManager::CurrentBackendSystemInputInterface = nullptr;
    =================================================================
    */
    class BackendManager
    {
        private:
            static int MainWindowCount;
            static std::vector<BackendDrawingInterface*> BackendDrawingInterfaces;
            static std::vector<BackendMainWindowInterface*> BackendMainWindowInterfaces;
            static BackendSystemInputInterface* CurrentBackendSystemInputInterface;
        
        public:
            //function: AddDrawingInterface
            static void AddDrawingInterface(BackendDrawingInterface* backendDrawing);
            
            //function: RemoveDrawingInterface
            static void RemoveDrawingInterface(BackendDrawingInterface* backendDrawing);
            
            //function: GetDrawingInterface
            static BackendDrawingInterface* GetDrawingInterface(int index);
            
            //function: GetDrawingInterfaceIndex
            static int GetDrawingInterfaceIndex(BackendDrawingInterface* backendDrawing);

            //function: AddInputInterface
            static void AddInputInterface(BackendSystemInputInterface* backendInput);
            
            //function: RemoveInputInterface
            static void RemoveInputInterface(BackendSystemInputInterface* backendInput);
            
            //function: GetInputInterface
            static BackendSystemInputInterface* GetInputInterface();
            
            //function: AddMainWindowInterface
            static void AddMainWindowInterface(BackendMainWindowInterface* backendMainWindow);
            
            //function: RemoveMainWindowInterface
            static void RemoveMainWindowInterface(BackendMainWindowInterface* backendMainWindow);
            
            //function: GetMainWindowInterface
            static BackendMainWindowInterface* GetMainWindowInterface(int index);
            
            //function: GetMainWindowInterfaceIndex
            static int GetMainWindowInterfaceIndex(BackendMainWindowInterface* backendMainWindow);
            
            //function: GetMainWindowCount
            static int GetMainWindowCount();
    };
}

}

#endif
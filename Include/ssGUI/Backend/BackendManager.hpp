#ifndef SSGUI_BACKEND_MANAGER
#define SSGUI_BACKEND_MANAGER

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
        static std::vector<ssGUI::Backend::BackendDrawingInterface*> BackendDrawingInterfaces;
        static std::vector<ssGUI::Backend::BackendMainWindowInterface*> BackendMainWindowInterfaces;
        static ssGUI::Backend::BackendSystemInputInterface* BackendSystemInputInterface;
    =================================================================
    ============================== C++ ==============================
    int BackendManager::MainWindowCount = 0;
    std::vector<ssGUI::Backend::BackendDrawingInterface*> BackendManager::BackendDrawingInterfaces;
    std::vector<ssGUI::Backend::BackendMainWindowInterface*> BackendManager::BackendMainWindowInterfaces;
    ssGUI::Backend::BackendSystemInputInterface* BackendManager::BackendSystemInputInterface = nullptr;
    =================================================================
    */
    class BackendManager
    {
        private:
            static int MainWindowCount;
            static std::vector<ssGUI::Backend::BackendDrawingInterface*> BackendDrawingInterfaces;
            static std::vector<ssGUI::Backend::BackendMainWindowInterface*> BackendMainWindowInterfaces;
            static ssGUI::Backend::BackendSystemInputInterface* BackendSystemInputInterface;
        
        public:
            //function: AddDrawingInterface
            static void AddDrawingInterface(ssGUI::Backend::BackendDrawingInterface* backendDrawing);
            
            //function: RemoveDrawingInterface
            static void RemoveDrawingInterface(ssGUI::Backend::BackendDrawingInterface* backendDrawing);
            
            //function: GetDrawingInterface
            static ssGUI::Backend::BackendDrawingInterface* GetDrawingInterface(int index);
            
            //function: GetDrawingInterfaceIndex
            static int GetDrawingInterfaceIndex(ssGUI::Backend::BackendDrawingInterface* backendDrawing);

            //function: AddInputInterface
            static void AddInputInterface(ssGUI::Backend::BackendSystemInputInterface* backendInput);
            
            //function: RemoveInputInterface
            static void RemoveInputInterface(ssGUI::Backend::BackendSystemInputInterface* backendInput);
            
            //function: GetInputInterface
            static ssGUI::Backend::BackendSystemInputInterface* GetInputInterface();
            
            //function: AddMainWindowInterface
            static void AddMainWindowInterface(ssGUI::Backend::BackendMainWindowInterface* backendMainWindow);
            
            //function: RemoveMainWindowInterface
            static void RemoveMainWindowInterface(ssGUI::Backend::BackendMainWindowInterface* backendMainWindow);
            
            //function: GetMainWindowInterface
            static ssGUI::Backend::BackendMainWindowInterface* GetMainWindowInterface(int index);
            
            //function: GetMainWindowInterfaceIndex
            static int GetMainWindowInterfaceIndex(ssGUI::Backend::BackendMainWindowInterface* backendMainWindow);
            
            //function: GetMainWindowCount
            static int GetMainWindowCount();
    };
}

}

#endif
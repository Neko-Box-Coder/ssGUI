#ifndef SSGUI_BACKEND_MANAGER_H
#define SSGUI_BACKEND_MANAGER_H

#include <vector>
#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"

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
        static std::vector<DrawingInterface*> DrawingInterfaces;
        static std::vector<MainWindowInterface*> MainWindowInterfaces;
        static SystemInputInterface* CurrentSystemInputInterface;
    =================================================================
    ============================== C++ ==============================
    int BackendManager::MainWindowCount = 0;
    std::vector<DrawingInterface*> BackendManager::DrawingInterfaces;
    std::vector<MainWindowInterface*> BackendManager::MainWindowInterfaces;
    SystemInputInterface* BackendManager::CurrentSystemInputInterface = nullptr;
    =================================================================
    */
    class BackendManager
    {
        private:
            static int MainWindowCount;
            static std::vector<DrawingInterface*> DrawingInterfaces;
            static std::vector<MainWindowInterface*> MainWindowInterfaces;
            static SystemInputInterface* CurrentSystemInputInterface;
        
        public:
            //function: AddDrawingInterface
            static void AddDrawingInterface(DrawingInterface* backendDrawing);
            
            //function: RemoveDrawingInterface
            static void RemoveDrawingInterface(DrawingInterface* backendDrawing);
            
            //function: GetDrawingInterface
            static DrawingInterface* GetDrawingInterface(int index);
            
            //function: GetDrawingInterfaceIndex
            static int GetDrawingInterfaceIndex(DrawingInterface* backendDrawing);

            //function: AddInputInterface
            static void AddInputInterface(SystemInputInterface* backendInput);
            
            //function: RemoveInputInterface
            static void RemoveInputInterface(SystemInputInterface* backendInput);
            
            //function: GetInputInterface
            static SystemInputInterface* GetInputInterface();
            
            //function: AddMainWindowInterface
            static void AddMainWindowInterface(MainWindowInterface* backendMainWindow);
            
            //function: RemoveMainWindowInterface
            static void RemoveMainWindowInterface(MainWindowInterface* backendMainWindow);
            
            //function: GetMainWindowInterface
            static MainWindowInterface* GetMainWindowInterface(int index);
            
            //function: GetMainWindowInterfaceIndex
            static int GetMainWindowInterfaceIndex(MainWindowInterface* backendMainWindow);
            
            //function: GetMainWindowCount
            static int GetMainWindowCount();
    };
}

}

#endif
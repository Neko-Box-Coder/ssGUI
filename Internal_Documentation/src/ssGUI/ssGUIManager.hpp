#ifndef SSGUI_MANAGER
#define SSGUI_MANAGER

#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"
#include "ssGUI/Enums/GUIObjectType.hpp"
#include "ssGUI/DataClasses/InputStatus.hpp"
#include "ssGUI/Backend/BackendFactory.hpp"

#include <stack>
#include <queue>

#if SLOW_UPDATE
    //For slowing update for debugging purposes
    #include <thread>
    #include <chrono>
#endif

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUIManager
    This manages all the GUI Objects, system input, update and render order, etc...
    There's should only be 1 instance of ssGUIManager.

    You only need to add the MainWindow object and run it.
    ================== C++ ==================
    ssGUIManager manager = ssGUIManager();
    manager.AddGUIObject(mainWindowObject);
    manager.StartRunning();
    =========================================

    You can subscribe to the update function (called every frame) by adding a listener to <AddOnUpdateEventListener>

    You can also get the current instance of ssGUIManager without having to get a reference of it.
    > ssGUIManager* currentInstance = ssGUIManager::GetInstance();
    */
    class ssGUIManager
    {
        private:
            ssGUI::Backend::BackendSystemInputInterface* BackendInput;
            
            std::list<ssGUI::GUIObject*> MainWindowPList;      //TODO : Turn this into ssGUI::MainWindow* instead maybe
            //std::list<ssGUI::GUIObject*> GUIObjectPList;

            std::vector<std::function<void()>> OnUpdateEventListeners;
            std::vector<bool> OnUpdateEventListenersValid;
            std::queue<int> OnUpdateEventListenersNextFreeIndices;
            std::vector<std::function<void()>> OnPostRenderingUpdateEventListeners;
            std::vector<bool> OnPostRenderingUpdateEventListenersValid;
            std::queue<int> OnPostRenderingUpdateEventListenersNextFreeIndices;
            std::vector<std::function<void( std::list<ssGUI::GUIObject*>& ) >> OnCustomRenderEventListeners;
            std::vector<bool> OnCustomRenderEventListenersValid;
            std::queue<int> OnCustomRenderEventListenersNextFreeIndices;

            bool IsCustomRendering;
            
            static ssGUI::ssGUIManager* CurrentInstanceP;

            void Internal_Update();
            void PollInputs();
            void CheckMainWindowExistence();
            void GUIObjectsEventCallback();
            void Render();
            void UpdateObjects();
            void UpdateCursor();
            
            ssGUI::GUIObject* FindParentWindowP(ssGUI::GUIObject& obj);
            //void AssginParentToChildren(ssGUI::GUIObject& targetObj, ssGUI::GUIObject* newParentP);

        public:
            ssGUIManager();
            virtual ~ssGUIManager();

            //function: AddGUIObject
            //Only main window should be added
            void AddGUIObject(ssGUI::GUIObject* obj);

            //function :UseCustomRendering
            void UseCustomRendering(bool customRendering);

            //function: RemoveGUIObject
            void RemoveGUIObject(ssGUI::GUIObject* obj);

            //void ChangeGUIObjectOrder(ssGUI::GUIObject& obj, int order);
            //int GetGUIObjectOrder(ssGUI::GUIObject& obj);

            //function: GetGUIObjectCount
            int GetGUIObjectCount();

            //function: StartRunning
            //This will block the thread until all <MainWindow>s are closed
            void StartRunning();

            //function: GetBackendInputInterface
            ssGUI::Backend::BackendSystemInputInterface* GetBackendInputInterface();

            //function: GetInstance
            static ssGUI::ssGUIManager* GetInstance();

            //function: AddOnUpdateEventListener
            int AddOnUpdateEventListener(std::function<void()> event);

            //function: RemoveOnUpdateEventListener
            void RemoveOnUpdateEventListener(int index);

            //function: AddOnPostRenderingUpdateEventListener
            int AddOnPostRenderingUpdateEventListener(std::function<void()> event);

            //function: RemoveOnPostRenderingUpdateEventListener
            void RemoveOnPostRenderingUpdateEventListener(int index);

            //function: AddOnCustomRenderEventListener
            int AddOnCustomRenderEventListener(std::function<void( std::list<ssGUI::GUIObject*>& ) > event);

            //function: RemoveOnCustomRenderEventListener
            void RemoveOnCustomRenderEventListener(int index);

            //function: Clear
            inline void Clear()
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
    };
}

#endif
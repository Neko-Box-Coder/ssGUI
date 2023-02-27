#ifndef SSGUI_MANAGER_H
#define SSGUI_MANAGER_H

#include "ssGUI/GUIObjectClasses/MainWindow.hpp"
#include "ssGUI/Enums/GUIObjectType.hpp"
#include "ssGUI/DataClasses/InputStatus.hpp"

#include <stack>
#include <queue>

#if SSGUI_SLOW_UPDATE
    //For slowing update for debugging purposes
    #include <thread>
    #include <chrono>
#endif

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::ssGUIManager
    This manages all the GUI Objects, system input, update and render order, etc...
    There's should only be 1 instance of ssGUIManager.

    You only need to add the MainWindow object and run it.
    ================== C++ ==================
    ssGUIManager manager = ssGUIManager();
    manager.AddGUIObject(mainWindowObject);
    manager.StartRunning();
    =========================================

    You can subscribe to the update function (called every frame) by adding a listener using <AddPostGUIUpdateEventListener>

    You can also get the current instance of ssGUIManager without having to get a reference of it.
    > ssGUIManager* currentInstance = ssGUIManager::GetInstance();
    */
    class ssGUIManager
    {
        private:
            ssGUI::Backend::BackendSystemInputInterface* BackendInput;
            
            std::list<ssGUI::GUIObject*> MainWindowPList;      //TODO : Turn this into ssGUI::MainWindow* instead maybe

            std::vector<std::function<void()>> PreGUIUpdateEventListeners;
            std::vector<bool> PreGUIUpdateEventListenersValid;
            std::queue<int> PreGUIUpdateEventListenersNextFreeIndices;

            std::vector<std::function<void()>> PostGUIUpdateEventListeners;
            std::vector<bool> PostGUIUpdateEventListenersValid;
            std::queue<int> PostGUIUpdateEventListenersNextFreeIndices;

            std::vector<std::function<void(ssGUI::MainWindow*)>> PostGUIRenderEventListeners;
            std::vector<bool> PostGUIRenderEventListenersValid;
            std::queue<int> PostGUIRenderEventListenersNextFreeIndices;

            std::vector<std::function<void( std::list<ssGUI::GUIObject*>& ) >> OnCustomRenderEventListeners;
            std::vector<bool> OnCustomRenderEventListenersValid;
            std::queue<int> OnCustomRenderEventListenersNextFreeIndices;

            bool IsCustomRendering;
            bool ForceRendering;

            float TargetFrameInterval;
            int FrameTimeIndex;
            std::vector<int> FrameTimes;

            
            static ssGUI::ssGUIManager* CurrentInstanceP;

            void Internal_Update();
            void PollInputs();
            void CheckMainWindowExistence();
            void Render();
            void UpdateObjects();
            void UpdateCursor();
            
            ssGUI::GUIObject* FindParentWindowP(ssGUI::GUIObject& obj);

        public:
            ssGUIManager();
            virtual ~ssGUIManager();

            //function: AddGUIObject
            //Only main window should be added, the reason why it is accepting <ssGUI::GUIObject> is for future compatibility.
            void AddGUIObject(ssGUI::GUIObject* obj);

            //function :UseCustomRendering
            //If true, it will not render any objects and call the custom rendering event, which you can add using <AddOnCustomRenderEventListener>.
            void UseCustomRendering(bool customRendering);

            //function: RemoveGUIObject (Obselete)
            //Removes the main window object added by <AddGUIObject>. <ssGUI::Hierarchy::Delete> should be used instead to remove any <ssGUI::MainWindow>.
            void RemoveGUIObject(ssGUI::GUIObject* obj);

            //function: GetGUIObjectCount
            //Returns the number of main window added by <AddGUIObject>
            int GetGUIObjectCount();

            //function: StartRunning
            //Starts the ssGUIManager.
            //This will block the thread until all <MainWindow>s are closed
            void StartRunning();

            //function: GetBackendInputInterface
            //Gets the <ssGUI::Backend::BackendSystemInputInterface> used by <ssGUIManager>
            ssGUI::Backend::BackendSystemInputInterface* GetBackendInputInterface();

            //function: GetInstance
            //Static method that allow accessing <ssGUIManager> from anywhere. If there's no instance found, nullptr is returned.
            static ssGUI::ssGUIManager* GetInstance();

            //function: AddPreGUIUpdateEventListener
            //Adds event callback that gets triggered before the update function is called for GUI objects.
            //Returns index that can be used to remove the event callback later.
            int AddPreGUIUpdateEventListener(std::function<void()> event);

            //function: RemovePreGUIUpdateEventListener
            //Removes event callback that gets triggered before the update function is called for GUI objects
            void RemovePreGUIUpdateEventListener(int index);

            //function: AddPostGUIUpdateEventListener
            //Adds event callback that gets triggered after the update function (before render function) is called for GUI objects.
            //Returns index that can be used to remove the event callback later.
            int AddPostGUIUpdateEventListener(std::function<void()> event);

            //function: RemovePostGUIUpdateEventListener
            //Removes event callback that gets triggered after the update function (before render function) is called for GUI objects
            void RemovePostGUIUpdateEventListener(int index);

            //function: AddPostGUIRenderEventListener
            //Adds event callback that gets triggered after the render function is called for GUI objects.
            //Returns index that can be used to remove the event callback later.
            int AddPostGUIRenderEventListener(std::function<void(ssGUI::MainWindow*)> event);

            //function: RemovePostGUIRenderEventListener
            //Removes event callback that gets triggered after the render function is called for GUI objects
            void RemovePostGUIRenderEventListener(int index);

            //function: AddOnCustomRenderEventListener
            //Adds event callback that is responsible of rendering all the GUI Objects instead of using the default rendering by <ssGUIManager>
            //Returns index that can be used to remove the event callback later.
            int AddOnCustomRenderEventListener(std::function<void( std::list<ssGUI::GUIObject*>& ) > event);

            //function: RemoveOnCustomRenderEventListener
            //Removes the event callback that is responsible of rendering all the GUI Objects instead of using the default rendering by <ssGUIManager>
            void RemoveOnCustomRenderEventListener(int index);

            //function: SetForceRendering
            //Sets if we force all GUI objects to redraw
            void SetForceRendering(bool force);

            //function: IsForceRendering
            //Returns if we force all GUI objects to redraw
            bool IsForceRendering();

            //function: SetTargetFramerate
            //Sets the target framerate (FPS), negative to disable it
            void SetTargetFramerate(float framerate);

            //function: GetTargetFramerate
            //Returns the target framerate (FPS), negative if disabled
            float GetTargetFramerate();

            //function: Clear
            //Clears the console
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
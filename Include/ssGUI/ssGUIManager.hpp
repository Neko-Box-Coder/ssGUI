#ifndef SSGUI_MANAGER_H
#define SSGUI_MANAGER_H

#include "ssGUI/GUIObjectClasses/MainWindow.hpp"
#include "ssGUI/Enums/GUIObjectType.hpp"
#include "ssGUI/DataClasses/InputStatus.hpp"

#include <type_traits>
#include <stack>
#include <queue>

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::ssGUIManager
    This manages all the GUI Objects, system input, update and render order, etc...
    There's should only be 1 instance of ssGUIManager.

    You only need to add the MainWindow object and run it.
    ================== C++ ==================
    ssGUIManager manager = ssGUIManager();
    auto* mainWindow = manager.AddRootGUIObject<ssGUI::MainWindow>();
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
            ssGUI::Enums::CursorType LastCursor;
            std::string LastCursorName;

            float TargetFrameInterval;
            int FrameTimeIndex;
            std::vector<int> FrameTimes;

            
            static ssGUI::ssGUIManager* CurrentInstanceP;

            void InitializeMainWindows();

            void Internal_Update();
            bool CheckMainWindowExistence();
            void Render();
            
            ssGUI::GUIObject* FindParentWindowP(ssGUI::GUIObject& obj);

        public:
            ssGUIManager();
            virtual ~ssGUIManager();

            //function: AddRootGUIObject (Obselete)
            //This is only used internally, use the template version (below) instead.
            //Only main window should be added, the reason why it is accepting <ssGUI::GUIObject> is for future compatibility.
            void AddRootGUIObject(ssGUI::GUIObject* obj);
            
            //function: AddRootGUIObject
            //Adds a GUI Object (Has to be MainWindow for now) to this ssGUIManager, the lifetime is managed by ssGUIManager. 
            template<typename T>
            T* AddRootGUIObject()
            {
                if(std::is_base_of<ssGUI::MainWindow, T>::value)
                {
                    auto* mainWindow = ssGUI::Factory::Create<T>();
                    AddRootGUIObject(mainWindow);
                    return mainWindow;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_MANAGER_TAG, "You cannot add non MainWindow object as root GUI Object");
                    return nullptr;
                }
            }
            
            //function: AddGUIObjectAsChild
            //Adds a GUI Object as a child to a GUIObject under this ssGUIManager, the lifetime is managed by ssGUIManager. 
            template<typename T>
            T* AddGUIObjectAsChild(ssGUI::GUIObject* parent)
            {
                if(std::is_base_of<ssGUI::GUIObject, T>::value)
                {
                    auto* guiObject = ssGUI::Factory::Create<T>();
                    guiObject->SetParent(parent);
                    return guiObject;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_MANAGER_TAG, "You cannot add non GUI object to manager");
                    return nullptr;
                }
            }

            //function :UseCustomRendering
            //If true, it will not render any objects and call the custom rendering event, which you can add using <AddOnCustomRenderEventListener>.
            void UseCustomRendering(bool customRendering);

            //function: RemoveGUIObject (Obselete)
            //Removes the main window object added by <AddRootGUIObject>. <ssGUI::Hierarchy::Delete> should be used instead to remove any <ssGUI::MainWindow>.
            void RemoveGUIObject(ssGUI::GUIObject* obj);

            //function: GetGUIObjectCount
            //Returns the number of main window added by <AddRootGUIObject>
            int GetGUIObjectCount();

            //function: StartRunning
            //Starts the ssGUIManager.
            //This will block the thread until all <MainWindow>s are closed
            void StartRunning();
            
            //function: StartRunningAsync
            //This will start the ssGUIManager without blocking the thread.
            //Call <StepFrame> to process and render the next frame.
            //Alternatively, you can call the functions separately. See <StepFrame> for more details.
            void StartRunningAsync();
            
            //function: IsRunningNeededAsync
            //Checks if it is necessary to continue to the next frame.
            //If false, **DO NOT** call any other functions as otherwise the behaviour is undefined.
            bool IsRunningNeededAsync();
            
            //function: PollInputsAsync
            //Updates all the input between the last frame up to this moment.
            void PollInputsAsync();
            
            //function: InvokePreGUIObjectsUpdateEventAsync
            //Calls any user callback before triggering GUI objects' update
            void InvokePreGUIObjectsUpdateEventAsync();
            
            //function: UpdateObjectsAsync
            //Calls the GUI Objects main logic, this includes any <Extensions: ssGUI::ExtensionsExtension> 
            //and <EventCallbacks: ssGUI::EventCallback> as well.
            void UpdateObjectsAsync();
            
            //function: CleanUpDeletedObjectsAsync
            //Cleans up any GUI Objects marked as deleted
            void CleanUpDeletedObjectsAsync();
            
            //function: InvokePostGUIObjectsUpdateEventAsync
            //Calls any user callback after triggering GUI objects' update
            void InvokePostGUIObjectsUpdateEventAsync();
            
            //function: RenderObjectsAsync
            //Renders GUI objects
            void RenderObjectsAsync();
            
            //function: UpdateCursorAsync
            //Updates (and loads) the cursor if it is changed
            void UpdateCursorAsync();
            
            //function: StepFrame
            //This is a bundled function which calls all the functions needed for a frame in order for ssGUI to work properly.
            //This calls in order:
            //1. <IsRunningNeededAsync>
            //2. <PollInputsAsync>
            //3. <InvokePreGUIObjectsUpdateEventAsync>
            //4. <UpdateObjectsAsync>
            //5. <CleanUpDeletedObjectsAsync>
            //6. <InvokePostGUIObjectsUpdateEventAsync>
            //7. <CleanUpDeletedObjectsAsync>
            //8. <RenderObjectsAsync>
            //9. <UpdateCursorAsync>
            bool StepFrame();

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

            //function: AddPreGUIRenderEventListener
            //See <AddPostGUIUpdateEventListener>
            int AddPreGUIRenderEventListener(std::function<void()> event);

            //function: RemovePostGUIUpdateEventListener
            //Removes event callback that gets triggered after the update function (before render function) is called for GUI objects
            void RemovePostGUIUpdateEventListener(int index);

            //function: RemovePreGUIRenderEventListener
            //See <RemovePostGUIUpdateEventListener>
            void RemovePreGUIRenderEventListener(int index);

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

            //function: SetRedrawEveryFrame
            //Sets if we force all GUI objects to redraw every frame
            void SetRedrawEveryFrame(bool redraw);

            //function: IsRedrawEveryFrame
            //Returns if we force all GUI objects to redraw every frame
            bool IsRedrawEveryFrame();

            //function: SetTargetFramerate
            //Sets the target framerate (FPS), negative to disable it
            void SetTargetFramerate(float framerate);

            //function: GetTargetFramerate
            //Returns the target framerate (FPS), negative if disabled
            float GetTargetFramerate();
            
            //function: IsButtonOrKeyDown
            //See <ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistCurrentFrame> and
            //<ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistLastFrame>
            bool IsButtonOrKeyDown(ssGUI::Enums::GenericButtonAndKeyInput input) const;
            
            //function: IsButtonOrKeyDown
            //See <ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistCurrentFrame> and
            //<ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistLastFrame>
            template<typename T>
            bool IsButtonOrKeyDown(T input) const
            {
                return IsButtonOrKeyDown(static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(input));
            }
            
            //function: IsButtonOrKeyHeld
            //See <ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistCurrentFrame> and
            //<ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistLastFrame>
            bool IsButtonOrKeyHeld(ssGUI::Enums::GenericButtonAndKeyInput input) const;
            
            //function: IsButtonOrKeyHeld
            //See <ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistCurrentFrame> and
            //<ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistLastFrame>
            template<typename T>
            bool IsButtonOrKeyHeld(T input) const
            {
                return IsButtonOrKeyHeld(static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(input));
            }
            
            //function: IsButtonOrKeyUp
            //See <ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistCurrentFrame> and
            //<ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistLastFrame>
            bool IsButtonOrKeyUp(ssGUI::Enums::GenericButtonAndKeyInput input) const;
            
            //function: IsButtonOrKeyUp
            //See <ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistCurrentFrame> and
            //<ssGUI::Backend::BackendSystemInputInterface::IsButtonOrKeyPressExistLastFrame>
            template<typename T>
            bool IsButtonOrKeyUp(T input) const
            {
                return IsButtonOrKeyUp(static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(input));
            }
            
            //function: GetMousePosition
            //See <ssGUI::Backend::BackendSystemInputInterface::GetCurrentMousePosition>
            glm::ivec2 GetMousePosition(ssGUI::MainWindow* mainWindow) const;
            
            //function: GetMousePositionDelta
            //See <ssGUI::Backend::BackendSystemInputInterface::GetCurrentMousePosition> and
            //<ssGUI::Backend::BackendSystemInputInterface::GetLastMousePosition>
            glm::ivec2 GetMousePositionDelta(ssGUI::MainWindow* mainWindow) const;
            
            //function: GetMouseScrollDelta
            //See <ssGUI::Backend::BackendSystemInputInterface::GetCurrentMouseScrollDelta>            
            glm::vec2 GetMouseScrollDelta() const;
            
            //function: GetTextInput
            //See <ssGUI::Backend::BackendSystemInputInterface::GetTextInput>            
            void GetTextInput(std::wstring& textInput) const;
            
            //function: GetTextInput
            //See <ssGUI::Backend::BackendSystemInputInterface::GetTextInput>            
            void GetTextInput(std::string& textInput) const;
            
            //function: SetCursorType
            //See <ssGUI::Backend::BackendSystemInputInterface::SetCursorType>
            void SetCursorType(ssGUI::Enums::CursorType cursorType);
            
            //function: GetCursorType
            //See <ssGUI::Backend::BackendSystemInputInterface::GetCursorType>
            ssGUI::Enums::CursorType GetCursorType() const;
            
            //function: CreateCustomCursor
            //See <ssGUI::Backend::BackendSystemInputInterface::CreateCustomCursor>            
            void CreateCustomCursor(ssGUI::ImageData* customCursor, std::string cursorName, glm::ivec2 cursorSize, glm::ivec2 hotspot);
            
            //function: SetCurrentCustomCursor
            //See <ssGUI::Backend::BackendSystemInputInterface::SetCurrentCustomCursor>            
            void SetCurrentCustomCursor(std::string cursorName);
            
            //function: GetCurrentCustomCursor
            //See <ssGUI::Backend::BackendSystemInputInterface::GetCurrentCustomCursor>            
            void GetCurrentCustomCursor(ssGUI::ImageData& customCursor, glm::ivec2& hotspot);
            
            //function: GetCurrentCustomCursorName
            //See <ssGUI::Backend::BackendSystemInputInterface::GetCurrentCustomCursorName>            
            void GetCurrentCustomCursorName(std::string& name);
            
            //function: GetCustomCursor
            //See <ssGUI::Backend::BackendSystemInputInterface::GetCustomCursor>            
            void GetCustomCursor(ssGUI::ImageData& customCursor, std::string cursorName, glm::ivec2& hotspot);
            
            //function: HasCustomCursor
            //See <ssGUI::Backend::BackendSystemInputInterface::HasCustomCursor>
            bool HasCustomCursor(const std::string& cursorName);
            
            //function: ClearClipboard
            //See <ssGUI::Backend::BackendSystemInputInterface::ClearClipboard>            
            bool ClearClipboard();
            
            //function: ClipbaordHasText
            //See <ssGUI::Backend::BackendSystemInputInterface::ClipbaordHasText>            
            bool ClipbaordHasText();
            
            //function: ClipbaordHasImage
            //See <ssGUI::Backend::BackendSystemInputInterface::ClipbaordHasImage>            
            bool ClipbaordHasImage();
            
            //function: SetClipboardImage
            //See <ssGUI::Backend::BackendSystemInputInterface::SetClipboardImage>            
            bool SetClipboardImage(const ssGUI::ImageData& imgData);
            
            //function: SetClipboardText
            //See <ssGUI::Backend::BackendSystemInputInterface::SetClipboardText>
            bool SetClipboardText(const std::wstring& str);
            
            //function: SetClipboardText
            //See <ssGUI::Backend::BackendSystemInputInterface::SetClipboardText>            
            bool SetClipboardText(const std::string& str);
            
            //function: GetClipboardImage
            //See <ssGUI::Backend::BackendSystemInputInterface::GetClipboardImage>            
            bool GetClipboardImage(ssGUI::ImageData& imgData);
            
            //function: GetClipboardText
            //See <ssGUI::Backend::BackendSystemInputInterface::GetClipboardText>            
            bool GetClipboardText(std::wstring& str);
            
            //function: GetClipboardText
            //See <ssGUI::Backend::BackendSystemInputInterface::GetClipboardText>            
            bool GetClipboardText(std::string& str);
            
            //function: GetElapsedTimeInMillisecond
            //See <ssGUI::Backend::BackendSystemInputInterface::GetElapsedTime>            
            uint64_t GetElapsedTimeInMillisecond() const;
            
            void PrintGUIObjectTree() const;

            //function: Clear
            //Clears the console
            void Clear();
    };
}

#endif
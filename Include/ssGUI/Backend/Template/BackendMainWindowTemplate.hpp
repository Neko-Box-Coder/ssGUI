#ifndef SSGUI_BACKEND_MAIN_WINDOW_TEMPLATE
#define SSGUI_BACKEND_MAIN_WINDOW_TEMPLATE

#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendMainWindowInterface
    class BackendMainWindowTemplate : BackendMainWindowInterface
    {
        private:
        
        public:
            BackendMainWindowTemplate();
            ~BackendMainWindowTemplate() override;
            //function: SetPosition
            //Sets the MainWindow position (distance from top-left) on the screen
            void SetPosition(glm::ivec2 pos) override;

            //function: GetPosition
            //Gets the MainWindow position (distance from top-left) on the screen
            glm::ivec2 GetPosition() const override;

            //function: SyncPositionOffset
            //Updates the distance between the top-left corner of the titlebar and top-left corner of the content
            void SyncPositionOffset() override;

            //function: GetPositionOffset
            //Gets the distance between the top-left corner of the titlebar and top-left corner of the content
            glm::ivec2 GetPositionOffset() const override;

            //function: SetSize
            //Sets the size of the main window
            void SetSize(glm::ivec2 size) override;

            //function: GetSize
            //Gets the size of the window
            glm::ivec2 GetSize() const override;

            //function: IsClosed
            //Returns if the main window is closed or not
            bool IsClosed() const override;

            //function: Close
            //Closes the main window
            void Close() override;
            
            //function: AbortClosing
            //Stops the main window from closing. Needs to be called when it is being closed in order for it to work.
            void AbortClosing() override;

            //function: AddOnCloseEvent
            //Adds the function to be called when the main window closes. Returns an index that can be used to remove the function for being called.
            int AddOnCloseEvent(std::function<void()> func) override;

            //function: RemoveOnCloseEvent
            //Removes the function to be called when the main window closes.
            void RemoveOnCloseEvent(int index) override;

            //function: SetTitle
            //Sets the title of the main window
            void SetTitle(std::wstring title) override;

            //function: GetTitle
            //Gets the title of the main window
            std::wstring GetTitle() const override;

            //function: SetIcon
            //Sets the icon of the main window
            void SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage) override;

            //function: SetVisible
            //Sets if the main window is visible or not
            void SetVisible(bool visible) override;

            //function: IsVisible
            //Returns if the main window is visible or not
            bool IsVisible() const override;

            //function: SetVSync
            //Sets if vSync is on or not
            void SetVSync(bool vSync) override;

            //function: IsVSync
            //Returns if vSync is on or not
            bool IsVSync() const override;

            //function: SetFocus
            //Sets if the main window to be focused.
            //If true, it essentially bring it to the front
            //If false, it will deselect the main window if supported (SFML not supported)
            void SetFocus(bool focus, bool externalByUser) override;
            
            //function: IsFocused
            //Returns if the main window is focused. If not focused, some inputs will not be captured.
            bool IsFocused() const override;

            //function: AddFocusChangedByUserEvent
            //Adds the function to be called when the main window gained or lost focus. Returns an index that can be used to remove the function for being called.
            int AddFocusChangedByUserEvent(std::function<void(bool focused)> func) override;

            //function: RemoveFocusChangedByUserEvent
            //Removes the function to be called when the main window gained or lost focus
            void RemoveFocusChangedByUserEvent(int index) override;

            //function: SetMSAA
            //Sets the main window's anti aliasing level
            void SetMSAA(int level) override;

            //function: GetMSAA
            //Gets the main window's anti aliasing level
            int GetMSAA() const override;

            //function: SetTitlebar
            //Sets if the main window has titlebar or not
            void SetTitlebar(bool titlebar) override;

            //function: HasTitlebar
            //Returns if the main window has titlebar or not. Note that if the main window is not in normal mode, this will return false.
            bool HasTitlebar() const override;

            //function: SetResizable
            //Sets if the main window is resizable or not
            void SetResizable(bool resizable) override;

            //function: IsResizable
            //Returns if the main window is resizable or not. Note that if the main window is not in normal mode, this will return false.
            bool IsResizable() const override;

            //function: SetCloseButton
            //Sets if the main window has close button or not
            void SetCloseButton(bool closeButton) override;

            //function: HasCloseButton
            //Returns if the main window has close button or not. Note that if the main window is not in normal mode, this will return false.
            bool HasCloseButton() const override;

            //function: SetWindowMode
            //Sets the main window mode
            void SetWindowMode(ssGUI::Enums::WindowMode WindowMode) override;

            //function: GetWindowMode
            //Gets the main window mode
            ssGUI::Enums::WindowMode GetWindowMode() const override;

            //function: SetGLContext
            //Sets this window as the current MainWindow context
            bool SetGLContext() override;
            
            //function: Clone
            BackendMainWindowInterface* Clone() override;

            //function: GetRawHandle
            void* GetRawHandle() override;
    };
}

}

#endif
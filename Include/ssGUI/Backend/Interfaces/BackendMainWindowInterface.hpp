#ifndef SSGUI_BACKEND_MAIN_WINDOW_INTERFACE_H
#define SSGUI_BACKEND_MAIN_WINDOW_INTERFACE_H

#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Enums/CursorType.hpp"
#include "ssGUI/Enums/WindowMode.hpp"
#include "glm/vec2.hpp"
#include <string>
#include <functional>

namespace ssGUI
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendMainWindowInterface
    class BackendMainWindowInterface
    {   
        public:
            BackendMainWindowInterface(){}
            virtual ~BackendMainWindowInterface() = 0;

            //function: SetWindowPosition
            //Sets the MainWindow position (distance from top-left) on the screen
            virtual void SetWindowPosition(glm::ivec2 pos) = 0;

            //function: GetWindowPosition
            //Gets the MainWindow position (distance from top-left) on the screen
            virtual glm::ivec2 GetWindowPosition() const = 0;

            //function: GetPositionOffset
            //Gets the offset from top-left corner of the rendering area and the top-left corner of the titlebar.
            //The offset should always be positive, meaning adding the offset to <GetWindowPosition> will get the position of the rendering area.
            virtual glm::ivec2 GetPositionOffset() const = 0;

            //function: SetWindowSize
            //Sets the size of the main window
            virtual void SetWindowSize(glm::ivec2 size) = 0;

            //function: GetWindowSize
            //Gets the size of the window
            virtual glm::ivec2 GetWindowSize() const = 0;

            //function: SetRenderSize
            //Sets the rendering (client) size of the window
            virtual void SetRenderSize(glm::ivec2 size) = 0;
            
            //function: GetRenderSize
            //Gets the rendering (client) size of the window
            virtual glm::ivec2 GetRenderSize() const = 0;

            //function: IsClosed
            //Returns if the main window is closed or not
            virtual bool IsClosed() const = 0;

            //function: Close
            //Closes the main window
            virtual void Close() = 0;
            
            //function: AbortClosing
            //Stops the main window from closing. Needs to be called when it is being closed in order for it to work.
            virtual void AbortClosing() = 0;

            //function: AddOnCloseEvent
            //Adds the function to be called when the main window closes. Returns an index that can be used to remove the function for being called.
            virtual int AddOnCloseEvent(std::function<void()> func) = 0;

            //function: RemoveOnCloseEvent
            //Removes the function to be called when the main window closes.
            virtual void RemoveOnCloseEvent(int index) = 0;

            //function: SetTitle
            //Sets the title of the main window
            virtual void SetTitle(std::wstring title) = 0;

            //function: GetTitle
            //Gets the title of the main window
            virtual std::wstring GetTitle() const = 0;

            //function: SetIcon
            //Sets the icon of the main window
            virtual void SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage) = 0;

            //function: SetVisible
            //Sets if the main window is visible or not
            virtual void SetVisible(bool visible) = 0;

            //function: IsVisible
            //Returns if the main window is visible or not
            virtual bool IsVisible() const = 0;

            //function: SetVSync
            //Sets if vSync is on or not
            virtual void SetVSync(bool vSync) = 0;

            //function: IsVSync
            //Returns if vSync is on or not
            virtual bool IsVSync() const = 0;

            //function: SetFocus
            //Sets if the main window to be focused.
            //If true, it essentially bring it to the front
            //If false, it will deselect the main window if supported (SFML not supported)
            virtual void SetFocus(bool focus, bool externalByUser) = 0;
            
            //function: IsFocused
            //Returns if the main window is focused. If not focused, some inputs will not be captured.
            virtual bool IsFocused() const = 0;

            //function: AddFocusChangedByUserEvent
            //Adds the function to be called when the main window gained or lost focus. Returns an index that can be used to remove the function for being called.
            virtual int AddFocusChangedByUserEvent(std::function<void(bool focused)> func) = 0;

            //function: RemoveFocusChangedByUserEvent
            //Removes the function to be called when the main window gained or lost focus
            virtual void RemoveFocusChangedByUserEvent(int index) = 0;

            //function: SetMSAA
            //Sets the main window's anti aliasing level
            virtual void SetMSAA(int level) = 0;

            //function: GetMSAA
            //Gets the main window's anti aliasing level
            virtual int GetMSAA() const = 0;

            //function: SetTitlebar
            //Sets if the main window has titlebar (and border) or not
            virtual void SetTitlebar(bool titlebar) = 0;

            //function: HasTitlebar
            //Returns if the main window has titlebar (and border) or not. Note that if the main window is not in normal mode, this will return false.
            virtual bool HasTitlebar() const = 0;

            //function: SetResizable
            //Sets if the main window is resizable or not
            virtual void SetResizable(bool resizable) = 0;

            //function: IsResizable
            //Returns if the main window is resizable or not. Note that if the main window is not in normal mode, this will return false.
            virtual bool IsResizable() const = 0;

            //function: SetCloseButton
            //Sets if the main window has close button or not
            virtual void SetCloseButton(bool closeButton) = 0;

            //function: HasCloseButton
            //Returns if the main window has close button or not. Note that if the main window is not in normal mode, this will return false.
            virtual bool HasCloseButton() const = 0;

            //function: SetWindowMode
            //Sets the main window mode
            virtual void SetWindowMode(ssGUI::Enums::WindowMode WindowMode) = 0;

            //function: GetWindowMode
            //Gets the main window mode
            virtual ssGUI::Enums::WindowMode GetWindowMode() const = 0;

            //function: SetGLContext
            //Sets this window as the current MainWindow context
            virtual bool SetGLContext() = 0;
            
            //function: Clone
            virtual BackendMainWindowInterface* Clone() = 0;

            //function: GetRawHandle
            virtual void* GetRawHandle() = 0;
    };

    inline BackendMainWindowInterface::~BackendMainWindowInterface(){}   //Pure virtual destructor needs to be defined
}

}

#endif
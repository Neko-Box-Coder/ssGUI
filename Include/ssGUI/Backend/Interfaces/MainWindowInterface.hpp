#ifndef SSGUI_BACKEND_MAIN_WINDOW_INTERFACE_HPP
#define SSGUI_BACKEND_MAIN_WINDOW_INTERFACE_HPP

#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"
#include "ssGUI/Enums/WindowDecorationOptions.hpp"
#include "ssGUI/Enums/WindowMode.hpp"

#include <string>
#include <functional>


//TODO(NOW): Allow to create a window from a given raw handle




namespace ssGUI
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::MainWindowInterface
    class MainWindowInterface
    {   
        public:
            MainWindowInterface(){}
            virtual ~MainWindowInterface() = 0;

            //function: Initialize
            virtual bool Initialize() = 0;

            //function: SetWindowPosition
            //Sets the main window position (distance from top-left) on the screen
            virtual void SetWindowPosition(glm::ivec2 pos) = 0;

            //function: GetWindowPosition
            //Gets the main window position (distance from top-left) on the screen
            virtual glm::ivec2 GetWindowPosition() const = 0;

            /*
            function: GetPositionOffset
            
            Gets the offsets from the render area to the window 
              for the top left and bottom right corner.
            
            The offsets will always be positive.
            */
            virtual void GetDecorationOffsets(glm::ivec2& topLeft, glm::ivec2& bottomRight) const = 0;

            //function: SetWindowSize
            //Sets the size of the main window, not to be confused with <SetRenderSize>
            virtual void SetWindowSize(glm::ivec2 size) = 0;

            //function: GetWindowSize
            //Gets the size of the main window, not to be confused with <GetRenderSize>
            virtual glm::ivec2 GetWindowSize() const = 0;

            //function: SetRenderSize
            //Sets the rendering (client) size of the window, which does not include window decorations
            virtual void SetRenderSize(glm::ivec2 size) = 0;
            
            //function: GetRenderSize
            //Gets the rendering (client) size of the window, which does not include window decorations
            virtual glm::ivec2 GetRenderSize() const = 0;

            //function: IsClosed
            //Returns if the main window is closed or not
            virtual bool IsClosed() const = 0;

            //function: Close
            //Closes the main window, which will trigger the close events added by <AddOnCloseEvent>
            virtual void Close() = 0;
            
            //function: AbortClosing
            //Stops the main window from closing. 
            //Needs to be called inside the events added by <AddOnCloseEvent>
            virtual void AbortClosing() = 0;

            //function: AddOnCloseEvent
            //Adds the callback to be called when the main window closes. 
            //Returns an id that can be used to remove the function for being called.
            virtual int AddOnCloseEvent(std::function<void(MainWindowInterface* mainWindow)> func) = 0;

            //function: RemoveOnCloseEvent
            //Removes the function to be called when the main window closes.
            virtual void RemoveOnCloseEvent(int id) = 0;

            //function: SetTitle
            //Sets the title of the main window
            virtual void SetTitle(std::u32string title) = 0;

            //function: SetTitle
            //Sets the title of the main window
            virtual void SetTitle(std::string title) = 0;

            //function: GetTitle
            //Gets the title of the main window
            virtual void GetTitle(std::u32string& outTitle) const = 0;

            //function: GetTitle
            //Gets the title of the main window
            virtual void GetTitle(std::string& outTitle) const = 0;

            //function: SetIcon
            //Sets the icon of the main window
            virtual void SetIcon(const ImageInterface& iconImage) = 0;

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
            //Adds the callback to be called when the main window gained or lost focus. 
            //Returns an id that can be used to remove the function for being called.
            virtual int 
                AddFocusChangedByUserEvent(std::function<void(  MainWindowInterface* mainWindow, 
                                                                bool focused)> func) = 0;

            //function: RemoveFocusChangedByUserEvent
            //Removes the function to be called when the main window gained or lost focus
            virtual void RemoveFocusChangedByUserEvent(int id) = 0;

            //function: SetAntiAliasingLevel
            //Sets the main window's anti aliasing level
            virtual void SetAntiAliasingLevel(int level) = 0;

            //function: GetAntiAliasingLevel
            //Gets the main window's anti aliasing level
            virtual int GetAntiAliasingLevel() const = 0;

            //function: SetResizable
            //Sets if the main window is resizable or not
            virtual void SetResizable(bool resizable) = 0;

            //function: IsResizable
            //Returns if the main window is resizable or not. 
            //Note that if the main window is not in normal mode, this will return false.
            virtual bool IsResizable() const = 0;
            
            //TODO: Add user handled moving and resizing

            //function: SetDecorationOptions
            //Sets the window decoration options, this is just a request to the display server,
            //  therefore it is not necessary that it will be honored.
            virtual bool SetDecorationOptions(Enums::WindowDecorationOptions options) = 0;
            
            //function: GetDecorationOptions
            //Gets the window decoration options
            virtual Enums::WindowDecorationOptions GetDecorationOptions() const = 0;
            
            //function: SetWindowMode
            //Sets the main window mode
            virtual void SetWindowMode(Enums::WindowMode WindowMode) = 0;

            //function: GetWindowMode
            //Gets the main window mode
            virtual Enums::WindowMode GetWindowMode() const = 0;

            /*
            function: Clone
            Clones the current main window with the same settings, 
              dispose it with <ssGUI::Factory::Dispose> when not needed.
            
            Events such as <AddOnCloseEvent> and <AddFocusChangedByUserEvent> are not copied.
            
            Cloning a closed window is an *undefined* behaviour, 
              same goes to window that is in "invisible" state.
            
            This function is tested limitedly, creating a new <MainWindow> is advised.
            */
            virtual MainWindowInterface* Clone() = 0;

            //function: GetRawHandle
            virtual void* GetRawHandle() const = 0;
    };

    //Pure virtual destructor needs to be defined
    inline MainWindowInterface::~MainWindowInterface(){}
}

}

#endif
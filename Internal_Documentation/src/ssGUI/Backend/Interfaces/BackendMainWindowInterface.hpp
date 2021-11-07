#ifndef SSGUI_BACKEND_MAIN_WINDOW_INTERFACE
#define SSGUI_BACKEND_MAIN_WINDOW_INTERFACE

#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Enums/CursorType.hpp"
#include "glm/vec2.hpp"
#include <string>
#include <functional>
//#include "ssGUI/Backend/BackendDrawingInterface.hpp"

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: BackendMainWindowInterface
    class BackendMainWindowInterface
    {
        private:
            // BackendMainWindowInterface(const BackendMainWindowInterface&);
            // BackendMainWindowInterface& operator=(const BackendMainWindowInterface&);
        
        public:
            BackendMainWindowInterface(){}
            //function: SetPosition
            virtual void SetPosition(glm::ivec2 pos) = 0;
            //function: GetPosition
            virtual glm::ivec2 GetPosition() const = 0;

            //function: SyncPositionOffset
            virtual void SyncPositionOffset() = 0;
            //function: GetPositionOffset
            virtual glm::ivec2 GetPositionOffset() const = 0;

            //function: SetSize
            virtual void SetSize(glm::ivec2 size) = 0;
            //function: GetSize
            virtual glm::ivec2 GetSize() const = 0;

            //function: IsClosed
            virtual bool IsClosed() const = 0;
            //function: Close
            virtual void Close() = 0;
            //function: AbortClosing
            virtual void AbortClosing() = 0;

            //function: AddOnCloseEvent
            virtual int AddOnCloseEvent(std::function<void()> func) = 0;
            //function: RemoveOnCloseEvent
            virtual void RemoveOnCloseEvent(int index) = 0;

            //function: SetTitle
            virtual void SetTitle(std::string title) = 0;
            //function: GetTitle
            virtual std::string GetTitle() const = 0;

            //function: SetIcon
            virtual void SetIcon(const ssGUI::Backend::BackendImageInterface& iconImage) = 0;

            //function: SetVisible
            virtual void SetVisible(bool visible) = 0;
            //function: IsVisible
            virtual bool IsVisible() const = 0;

            //function: SetVSync
            virtual void SetVSync(bool vSync) = 0;
            //function: IsVSync
            virtual bool IsVSync() const = 0;

            //function: SetFocus
            virtual void SetFocus(bool focus) = 0;
            //function: IsFocused
            virtual bool IsFocused() const = 0;

            //function: SetMSAA
            virtual void SetMSAA(int level) = 0;
            //function: GetMSAA
            virtual int GetMSAA() const = 0;

            //function: SetTitlebar
            virtual void SetTitlebar(bool titlebar) = 0;
            //function: HasTitlebar
            virtual bool HasTitlebar() const = 0;

            //function: SetResizable
            virtual void SetResizable(bool resizable) = 0;
            //function: IsResizable
            virtual bool IsResizable() const = 0;

            //function: SetCloseButton
            virtual void SetCloseButton(bool closeButton) = 0;
            //function: HasCloseButton
            virtual bool HasCloseButton() const = 0;
            
            //function: Clone
            virtual BackendMainWindowInterface* Clone() = 0;

            //function: GetRawHandle
            virtual void* GetRawHandle() = 0;
    };
}

#endif
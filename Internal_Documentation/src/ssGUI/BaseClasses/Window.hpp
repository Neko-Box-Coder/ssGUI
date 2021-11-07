#ifndef SSGUI_WINDOW
#define SSGUI_WINDOW

#include "ssGUI/BaseClasses/BaseGUIObject.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include "ssGUI/Enums/ResizeType.hpp"
#include "ssGUI/EventCallbacks/WindowCloseEventCallback.hpp"
#include <cmath>
#include <functional>

//namespace: ssGUI
namespace ssGUI
{
    //class: Window
    class Window : public BaseGUIObject
    {
        private:
            //Window status
            bool Titlebar;
            int TitlebarHeight;
            ssGUI::Enums::ResizeType ResizeType;
            bool Closable;
            bool Closed;
            bool IsClosingAborted;

            //Resize/Drag settings
            int ResizeHitbox;
            bool IsResizingTop;
            bool IsResizingBot;
            bool IsResizingLeft;
            bool IsResizingRight;
            bool IsDragging;
            glm::ivec2 OnTransformBeginPosition;
            glm::ivec2 OnTransformBeginSize;
            glm::ivec2 MouseDownOffset;
            glm::ivec2 MouseDownPosition;
            Window& operator=(Window const& other) = default;

        protected:
            Window(Window const& other) = default;

        public:
            Window();
            virtual ~Window() override;
            
            //function: Close
            virtual void Close();
            
            //function: IsClosed
            virtual bool IsClosed() const;
            
            //function: AbortClosing
            virtual void AbortClosing();

            //function: Internal_OnClose
            //Internal callback. You should not be calling this function normally.
            virtual void Internal_OnClose();

            //function: SetTitlebar
            virtual void SetTitlebar(bool set);
            
            //function: HasTitlebar
            virtual bool HasTitlebar() const;
            
            //function: SetTitlebarHeight
            virtual void SetTitlebarHeight(int height);
            
            //function: GetTitlebarHeight
            virtual int GetTitlebarHeight() const;
            
            //function: SetResizeType
            virtual void SetResizeType(ssGUI::Enums::ResizeType resizeType);
            
            //function: GetResizeType
            virtual ssGUI::Enums::ResizeType GetResizeType() const;
            
            //function: GetResizeHitbox
            virtual int GetResizeHitbox() const;
            
            //function: SetResizeHitbox
            virtual void SetResizeHitbox(int hitbox);
            
            //function: SetClosable
            virtual void SetClosable(bool closable);
            
            //function: IsClosable
            virtual bool IsClosable() const;

            //function: AddOnCloseEventListener
            virtual int AddOnCloseEventListener(std::function<void()> onClose);
            
            //function: RemoveOnCloseEventListener
            virtual void RemoveOnCloseEventListener(int index);
            
            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Draw
            virtual void Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus) override;
            
            //function: Clone
            virtual GUIObject* Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren) override;
    };
}






#endif
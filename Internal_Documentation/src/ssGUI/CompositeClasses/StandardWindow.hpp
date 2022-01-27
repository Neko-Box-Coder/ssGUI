#ifndef SSGUI_STANDARD_WINDOW
#define SSGUI_STANDARD_WINDOW

#include "ssGUI/BaseClasses/Window.hpp"
#include "ssGUI/ssGUITags.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class Text;
    class Button;
    class Image;

    //class: StandardWindow
    //WIP
    class StandardWindow : public Window
    {
        private:
            StandardWindow& operator=(StandardWindow const& other) = default;
        
        protected:
            int HorizontalPadding;
            int VerticalPadding;

            int WindowTitle;
            int WindowIcon;
            int CloseButton;

            StandardWindow(StandardWindow const& other);

            virtual void UpdateTitleText();
            virtual void UpdateIconImage();
            virtual void UpdateCloseButton();

        public:
            StandardWindow();
            virtual ~StandardWindow() override;

            virtual void SetWindowTitleObject(ssGUI::Text* text);

            virtual ssGUI::Text* GetWindowTitleObject() const;

            virtual void SetWindowIconObject(ssGUI::Image* image);
            
            virtual ssGUI::Image* GetWindowIconObject() const;
            
            virtual void SetCloseButtonObject(ssGUI::Button* button);
            
            virtual ssGUI::Button* GetCloseButtonObject() const;

            virtual void SetHorizontalPadding(int padding);

            virtual int GetHorizontalPadding() const;

            virtual void SetVerticalPadding(int padding);

            virtual int GetVerticalPadding() const;


            //function: SetTitlebar
            virtual void SetTitlebar(bool set) override;

            //function: SetClosable
            virtual void SetClosable(bool closable) override;


            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Internal_Draw
            //virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            //virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Clone
            virtual GUIObject* Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren) override;
    };
}


#endif
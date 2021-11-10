#ifndef SSGUI_MAIN_WINDOW
#define SSGUI_MAIN_WINDOW


#include "ssGUI/Backend/BackendFactory.hpp"
#include <functional>

#include "ssGUI/BaseClasses/Window.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //class: MainWindow
    //This class should not be overwritten unless you know what you are doing
    class MainWindow : public Window
    {
        private:
            ssGUI::Backend::BackendMainWindowInterface* BackendMainWindow;
            ssGUI::Backend::BackendDrawingInterface* BackendDrawing;

            uint64_t LastSyncTime;
            MainWindow& operator=(MainWindow const& other);

        protected:
            MainWindow(MainWindow const& other);

        public:
            //MainWindow(MainWindow& mainWindow);
            MainWindow();
            virtual ~MainWindow() override;
            
            
            //function: Render
            virtual void Render();
            
            //function: GetBackendWindowInterface
            virtual ssGUI::Backend::BackendMainWindowInterface* GetBackendWindowInterface();
            
            //function: GetBackendDrawingInterface
            virtual ssGUI::Backend::BackendDrawingInterface* GetBackendDrawingInterface();
            
            //function: IsFocused
            virtual bool IsFocused() const;
            
            //function: SetFocus
            virtual void SetFocus(bool focus);
            
            //function: GetPositionOffset
            virtual glm::ivec2 GetPositionOffset();
            
            //function: GetPosition
            virtual glm::ivec2 GetPosition() const override;
            
            //function: SetPosition
            virtual void SetPosition(glm::ivec2 position) override;
            
            //function: GetGlobalPosition
            virtual glm::ivec2 GetGlobalPosition() override;
            
            //function: SetGlobalPosition
            virtual void SetGlobalPosition(glm::ivec2 position) override;
            
            //function: GetSize
            virtual glm::ivec2 GetSize() const override;
            
            //function: SetSize
            virtual void SetSize(glm::ivec2 size) override;

            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const override;
            
            //function: Close
            virtual void Close() override;
            
            //function: AbortClosing
            virtual void AbortClosing() override;
            
            //function: Internal_OnClose
            virtual void Internal_OnClose() override; 
            
            //function: Draw
            //Although it says it is drawing, it actually doesn't draw anything.
            //Instead, it sets the window state instead
            virtual void Draw();
            
            //function: Draw
            virtual void Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
    
            //function: Clone
            virtual GUIObject* Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren) override;
    };
}

#endif
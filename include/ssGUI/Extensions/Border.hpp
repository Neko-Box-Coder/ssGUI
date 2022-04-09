#ifndef SSGUI_BORDER
#define SSGUI_BORDER


#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp" //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "glm/vec4.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{   
    /*class: Border
    Draws a border according to the *size* of the GUI Object
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        glm::u8vec4 BorderColour;
        int BorderWidth;
        ssGUI::GUIObject* Container;
        bool Enabled;
    =================================================================
    ============================== C++ ==============================
    Border::Border() : BorderColour(glm::u8vec4(0, 0, 0, 255)), BorderWidth(1), Container(nullptr), Enabled(true)
    {}
    =================================================================
    */
    class Border : public Extension
    {
        private:
            Border& operator=(Border const& other);
        
        protected:
            glm::u8vec4 BorderColour;
            int BorderWidth;
            ssGUI::GUIObject* Container;
            bool Enabled;

            virtual void DrawBorder();
            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

            Border(Border const& other);

        public:
            static const std::string EXTENSION_NAME;

            Border();
            virtual ~Border() override;
            
            //function: GetBorderColour
            virtual glm::u8vec4 GetBorderColour() const;
            
            //function: SetBorderColour
            virtual void SetBorderColour(glm::u8vec4 colour);
            
            //function: GetBorderWidth
            virtual int GetBorderWidth() const;
            
            //function: SetBorderWidth
            virtual void SetBorderWidth(int width);

            //Override from Extension
            //function: SetEnabled           
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            //See <Extension::Clone>
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif
#ifndef SSGUI_BORDER
#define SSGUI_BORDER


#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp" //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "glm/vec4.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{   
    /*class: ssGUI::Extensions::Border
    Draws a border according to the *size* of the GUI Object
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;
        bool Enabled;
        glm::u8vec4 BorderColor;
        int BorderWidth;

        //TODO : Maybe change to private enum class. Just too lazy to do it atm :P
        int8_t BorderSides;     //(0000 [Bottom bit] [Right bit] [Top bit] [Left bit])
    =================================================================
    ============================== C++ ==============================
    Border::Border() : Container(nullptr), Enabled(true), BorderColor(glm::u8vec4(0, 0, 0, 255)), BorderWidth(1), BorderSides(15)
    {}
    =================================================================
    */
    class Border : public Extension
    {
        public:
            friend class ssGUI::Factory;
        
        private:
            Border& operator=(Border const& other);
        
        protected:
            ssGUI::GUIObject* Container;
            bool Enabled;
            glm::u8vec4 BorderColor;
            int BorderWidth;

            //TODO : Maybe change to private enum class. Just too lazy to do it atm :P
            int8_t BorderSides;     //(0000 [Bottom bit] [Right bit] [Top bit] [Left bit])

            virtual void DrawBorder();
            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

            Border();
            virtual ~Border() override;
            Border(Border const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

        public:
            static const std::string EXTENSION_NAME;
            
            //function: GetBorderColor
            virtual glm::u8vec4 GetBorderColor() const;
            
            //function: SetBorderColor
            virtual void SetBorderColor(glm::u8vec4 colour);
            
            //function: GetBorderWidth
            virtual int GetBorderWidth() const;
            
            //function: SetBorderWidth
            virtual void SetBorderWidth(int width);

            //function: ShowBorderLeft
            virtual void ShowBorderLeft(bool show);

            //function: ShowBorderTop
            virtual void ShowBorderTop(bool show);

            //function: ShowBorderRight
            virtual void ShowBorderRight(bool show);

            //function: ShowBorderBottom
            virtual void ShowBorderBottom(bool show);

            //function: IsBorderLeftShowing
            virtual bool IsBorderLeftShowing() const;

            //function: IsBorderTopShowing
            virtual bool IsBorderTopShowing() const;

            //function: IsBorderRightShowing
            virtual bool IsBorderRightShowing() const;

            //function: IsBorderBottomShowing
            virtual bool IsBorderBottomShowing() const;

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
            virtual Border* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif
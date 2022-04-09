#ifndef SSGUI_CUSTOM_EXTENSION
#define SSGUI_CUSTOM_EXTENSION

#include "ssGUI/Extensions/Extension.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: Shape
    Shape extension allows adding shapes in runtime on the GUI Object without too much effort 

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        struct AdditionalShape
        {
            std::vector<glm::vec2> Vertices;
            std::vector<glm::u8vec4> Colors;
            bool BehindGUI;
        };

        ssGUI::GUIObject* Container;
        bool Enabled;

        bool ExtensionPreRender;
        std::vector<AdditionalShape> AdditionalShapes;
    =================================================================
    ============================== C++ ==============================
    Shape::Shape() : Container(nullptr), Enabled(true), ExtensionPreRender(true), AdditionalShapes()
    {}
    =================================================================
    */
    class Shape : public Extension
    {
        private:
            Shape& operator=(Shape const& other);

        protected:
            struct AdditionalShape
            {
                std::vector<glm::vec2> Vertices;
                std::vector<glm::u8vec4> Colors;
                bool BehindGUI;
            };

            ssGUI::GUIObject* Container;
            bool Enabled;

            bool ExtensionPreRender;
            std::vector<AdditionalShape> AdditionalShapes;

            Shape(Shape const& other);

            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            constexpr double pi() { return std::atan(1)*4; };

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
        
        public:
            static const std::string EXTENSION_NAME;

            Shape();
            virtual ~Shape() override;

            virtual void AddShape(std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject);
            virtual void AddShape(std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject, int index);

            virtual void AddRectangle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject);
            virtual void AddRectangle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject, int index);

            virtual void AddCircle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject);
            virtual void AddCircle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject, int index);

            virtual std::vector<glm::vec2>* GetVertices(int index);
            virtual std::vector<glm::u8vec4>* GetColors(int index);

            virtual void RemoveShape(int index);

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
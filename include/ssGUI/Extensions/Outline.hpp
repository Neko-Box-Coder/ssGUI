#ifndef SSGUI_OUTLINE
#define SSGUI_OUTLINE

#include "ssGUI/Extensions/Extension.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: CustomExtension
    [Insert extension summary here]

    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::GUIObject* Container;
        bool Enabled;

        int OutlineThickness;
        bool SimpleOutline;
        glm::u8vec4 OutlineColor;
        std::vector<int> TargetShapes;
        std::vector<int> TargetVertices;

        std::vector<int> VerticesToOutline;
        std::vector<int> VerticesToOutlinePrevVertices;
        std::vector<int> VerticesToOutlineNextVertices;
        std::vector<int> VerticesToOutlineNextNextVertices;
    =================================================================
    ============================== C++ ==============================
    CustomExtension::CustomExtension() : Container(nullptr), Enabled(true)
    {}
    =================================================================
    */
    class Outline : public Extension
    {
        private:
            ssGUI::GUIObject* Container;
            bool Enabled;

            int OutlineThickness;
            bool SimpleOutline;
            glm::u8vec4 OutlineColor;
            std::vector<int> TargetShapes;
            std::vector<int> TargetVertices;

            std::vector<int> VerticesToOutline;
            std::vector<int> VerticesToOutlinePrevVertices;
            std::vector<int> VerticesToOutlineNextVertices;
            std::vector<int> VerticesToOutlineNextNextVertices;
    
            Outline& operator=(Outline const& other);

        protected:
            Outline(Outline const& other);

            virtual void GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int> const & drawingCounts);

            virtual void UpdateVerticesForOutline();

            virtual void ConstructComplexOutline();

            virtual void ConstructSimpleOutline();

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

        public:
            static const std::string EXTENSION_NAME;

            Outline();
            virtual ~Outline() override;

            //function: SetOutlineThickness
            //Sets the thickness of the outline, in pixel
            virtual void SetOutlineThickness(int thickness);

            //function: GetOutlineThickness
            //Returns the thickness of the outline, in pixel
            virtual int GetOutlineThickness() const;

            /*function: SetSimpleOutline
            Sets if it uses simple outline drawing or not.

            If true, the outline will basically be a slightly larger version of the GUI object
            drawn behind the GUI object.
            If false, each two vertices of the GUI object will generate 2 more outline vertices and form a new shape it.
            This will generate a lot more shapes than the simple version.*/
            virtual void SetSimpleOutline(bool simpleOutline);

            /*function: IsSimpleOutline
            Returns if it uses simple outline drawing or not.

            If true, the outline will basically be a slightly larger version of the GUI object
            drawn behind the GUI object.
            If false, each two vertices of the GUI object will generate 2 more outline vertices and form a new shape it.
            This will generate a lot more shapes than the simple version.*/
            virtual bool IsSimpleOutline() const;

            //function: SetOutlineColor
            //Sets the color of the outline
            virtual void SetOutlineColor(glm::u8vec4 color);

            //function: GetOutlineColor
            //Returns the color of the outline
            virtual glm::u8vec4 GetOutlineColor() const;

            //function: AddTargetShape
            //Adds the shapeIndex that indicates the index of the shape to be outlined in GUI object.
            //Note that if you have added any target vertices (<AddTargetVertex>), this will be *ignored*.
            virtual void AddTargetShape(int shapeIndex);

            //function: GetTargetShape
            //Returns the shapeIndex that indicates the index of the shape to be outlined in GUI object,
            //by specifying the location shapedIndex that is stored in this extension.
            virtual int GetTargetShape(int location) const;

            //function: SetTargetShape
            //Sets the shapeIndex that indicates the index of the shape to be outlined in GUI object,
            //by specifying the location shapedIndex that is stored in this extension.
            virtual void SetTargetShape(int location, int shapeIndex);

            //function: GetTargetShapesCount
            //Returns the number of shapes to be outlined (number of shapeIndex) that are stored in this extension. 
            virtual int GetTargetShapesCount() const;

            //function: RemoveTargetShape
            //Removes the shapeIndex that indicates the index of the shape to be outlined in GUI object,
            //by specifying the location shapedIndex that is stored in this extension.
            virtual void RemoveTargetShape(int location);

            //function: ClearTargetShapes
            //Clears all the shapeIndex entries in this extension 
            virtual void ClearTargetShapes();

            //function: AddTargetVertex
            //Adds the target vertex that indicates the index of the vertex to be outlined in GUI object,
            //by specifying the location vertexIndex that is stored in this extension.
            //Note that if you are adding any target vertices, any target shapes added (<AddTargetShape>) this will be ignored.
            virtual void AddTargetVertex(int vertexIndex);

            //function: GetTargetVertex
            //Returns the vertexIndex that indicates the index of the vertex to be outlined in GUI object,
            //by specifying the location vertexIndex that is stored in this extension.
            virtual int GetTargetVertex(int location) const;

            //function: SetTargetVertex
            //Sets the vertexIndex that indicates the index of the vertex to be outlined in GUI object,
            //by specifying the location vertexIndex that is stored in this extension.
            virtual void SetTargetVertex(int location, int vertexIndex);

            //function: GetTargetVerticesCount
            //Returns the number of indices to be outlined (number of vertexIndex) that are stored in this extension. 
            virtual int GetTargetVerticesCount() const;

            //function: RemoveTargetVertex
            //Removes the vertexIndex that indicates the index of the vertex to be outlined in GUI object,
            //by specifying the location vertexIndex that is stored in this extension.
            virtual void RemoveTargetVertex(int location);

            //function: ClearTargetVertices
            //Clears all the vertexIndex entries in this extension
            virtual void ClearTargetVertices();

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;

            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

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
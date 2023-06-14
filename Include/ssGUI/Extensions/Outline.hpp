#ifndef SSGUI_OUTLINE_H
#define SSGUI_OUTLINE_H

#include "ssGUI/DataClasses/ShapeModifier.hpp"
#include "ssGUI/DataClasses/TargetShape.hpp"
#include "ssGUI/Extensions/Extension.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"
#include <unordered_set>

namespace ssGUI 
{ 

//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::Outline
    Outline allows to create a colored outline surrounding the target GUI Object shape/vertices.

    A GUI Object can be made up of multiple shapes, by default the outline extension outline the first shape of the GUI Object.

    By default, the outline is drawn inside the shape. This however can be set by <SetInnerOutline>.

    You can also target specific vertices of the GUI Object to be outlined (<AddTargetVertex>), in which case <AddTargetShape> will be overriden and ignored.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;                            //See <BindToObject>
        bool Enabled;                                           //See <IsEnabled>

        float OutlineThickness;                                 //See <GetOutlineThickness>
        bool SimpleOutline;                                     //See <IsSimpleOutline>
        bool InnerOutline;                                      //See <IsInnerOutline>
        glm::u8vec4 OutlineColor;                               //See <GetOutlineColor>
        std::vector<int> TargetShapes;                          //See <GetTargetShape>
        std::vector<int> TargetVertices;                        //See <GetTargetVertex>

        std::vector<int> VerticesToOutline;                     //(Internal variable) Used to identify vertices indices to outline
        std::vector<int> VerticesToOutlinePrevVertices;         //(Internal variable) Used to identify the previous vertex of vertex index to outline
        std::vector<int> VerticesToOutlineNextVertices;         //(Internal variable) Used to identify the next vertex of vertex index to outline
        std::vector<int> VerticesToOutlineNextNextVertices;     //(Internal variable) Used to identify the one after next vertex of vertex index to outline
        std::vector<int> VerticesToOutlineShapeIndex;           //(Internal variable) Used to identify the shapes index to outline
        std::vector<bool> VerticesToOutlineShapeStartFlag;      //(Internal variable) Used to identify which vertex is the start vertex of the shapes
    =================================================================
    ============================== C++ ==============================
    Outline::Outline() :    Container(nullptr),
                            Enabled(true),
                            OutlineThickness(1.1),
                            SimpleOutline(false),
                            InnerOutline(true),
                            OutlineColor(0, 0, 0, 255),
                            TargetShapes{0},
                            TargetVertices(),
                            VerticesToOutline(),
                            VerticesToOutlinePrevVertices(),
                            VerticesToOutlineNextVertices(),
                            VerticesToOutlineNextNextVertices(),
                            VerticesToOutlineShapeIndex(),
                            VerticesToOutlineShapeStartFlag()
    {}
    =================================================================
    */
    class Outline : public Extension
    {
        public:
            friend class ssGUI::Factory;
        
        private:    
            Outline& operator=(Outline const& other);

        protected:
            ssGUI::GUIObject* Container;                            //See <BindToObject>
            bool Enabled;                                           //See <IsEnabled>

            float OutlineThickness;                                 //See <GetOutlineThickness>
            bool SimpleOutline;                                     //See <IsSimpleOutline>
            bool InnerOutline;                                      //See <IsInnerOutline>
            glm::u8vec4 OutlineColor;                               //See <GetOutlineColor>

            ssGUI::ShapeModifier ModifiedShapes;

            //std::unordered_map<std::string, std::vector<int>> VerticesToOutline;    //(Internal variable) Used to identify vertices indices to outline
            //std::vector<std::string> VerticesToOutlineShapeIndex;                   //(Internal variable) Used to identify the shapes index to outline

            Outline();
            virtual ~Outline() override;
            Outline(Outline const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int>const & drawingCounts, int& shapeIndex);
            //virtual void UpdateVerticesForOutline();

            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            double pi() const { return std::atan(1)*4; };

            //Return angle in radians. Positive if angle between a and b is clockwise
            virtual double GetAngle(glm::vec2 a, glm::vec2 b);

            virtual bool FindInnerOutlinesIntersection(glm::vec2 curVertex, glm::vec2 prevVertex, glm::vec2 nextVertex, float outlineThickness, glm::vec2& intersection);

            virtual void PlotArc(glm::vec2 start, glm::vec2 end, glm::vec2 circlePos, std::vector<glm::vec2>& plottedPoints);

            virtual void ConstructComplexOutline(bool isInner);

            virtual void ConstructSimpleOutline();

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

        public:
            static const std::string EXTENSION_NAME;
            static const std::string OUTLINE_SHAPES_NAME;

            //function: SetOutlineThickness
            //Sets the thickness of the outline, in pixel
            virtual void SetOutlineThickness(float thickness);

            //function: GetOutlineThickness
            //Returns the thickness of the outline, in pixel
            virtual float GetOutlineThickness() const;

            //function: SetInnerOutline
            //Sets if the outlines are drawn inside the shape or not
            virtual void SetInnerOutline(bool inner);

            //function: IsInnerOutline
            //Returns if the outlines are drawn inside the shape or not
            virtual bool IsInnerOutline() const;

            /*function: SetSimpleOutline
            Sets if it uses simple outline drawing or not. Note this has no effect if <IsInnerOutline> is true.

            If true, the outline will basically be a slightly larger version of the GUI object
            drawn behind the GUI object.

            If false, each two vertices of the GUI object will generate 2 more outline vertices and form a new shape it.
            This will generate a lot more shapes than the simple version.*/
            virtual void SetSimpleOutline(bool simpleOutline);

            /*function: IsSimpleOutline
            Returns if it uses simple outline drawing or not. Note this has no effect if <IsInnerOutline> is true.

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
            //See <ssGUI::ShapeModifier::AddTargetShape>
            virtual int AddTargetShape(ssGUI::TargetShape targetShape);

            //function: GetTargetShape
            //See <ssGUI::ShapeModifier::GetTargetShape>
            virtual ssGUI::TargetShape GetTargetShape(int location) const;

            //function: SetTargetShape
            //See <ssGUI::ShapeModifier::SetTargetShape>
            virtual void SetTargetShape(int location, ssGUI::TargetShape targetShape);

            //function: GetTargetShapesCount
            //See <ssGUI::ShapeModifier::GetTargetShapesCount>
            virtual int GetTargetShapesCount() const;

            //function: RemoveTargetShape
            //See <ssGUI::ShapeModifier::RemoveTargetShape>
            virtual void RemoveTargetShape(int location);

            //function: ClearTargetShapes
            //See <ssGUI::ShapeModifier::ClearTargetShapes>
            virtual void ClearTargetShapes();

            //function: AddTargetVertex
            //See <ssGUI::ShapeModifier::AddTargetVertex>
            virtual int AddTargetVertex(ssGUI::TargetShape targetShape, int vertexIndex);

            //function: GetTargetVertex
            //See <ssGUI::ShapeModifier::GetTargetVertex>
            virtual VerticesIndicesForShape GetTargetVertices(int location) const;
            
            //function: GetTargetVertex
            //See <ssGUI::ShapeModifier::GetTargetVertex>
            virtual VerticesIndicesForShape GetTargetVertices(ssGUI::TargetShape targetShape) const;

            //function: SetTargetVertex
            //See <ssGUI::ShapeModifier::SetTargetVertex>
            virtual void SetTargetVertices(ssGUI::TargetShape targetShape, const std::vector<int>& vertices);

            //function: SetTargetVertex
            //See <ssGUI::ShapeModifier::SetTargetVertex>
            virtual void SetTargetVertices(int location, const std::vector<int>& vertices);

            //function: GetTargetVerticesCount
            //See <ssGUI::ShapeModifier::GetTargetVerticesCount>
            virtual int GetTargetVerticesCount() const;

            //function: RemoveTargetVertex
            //See <ssGUI::ShapeModifier::RemoveTargetVertex>
            virtual void RemoveTargetVertex(int location);

            //function: ClearTargetVertices
            //See <ssGUI::ShapeModifier::ClearTargetVertices>
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
            virtual void Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow) override;

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
            virtual Outline* Clone() override;
    };
}

}

#endif
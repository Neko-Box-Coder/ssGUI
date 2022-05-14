#ifndef SSGUI_OUTLINE
#define SSGUI_OUTLINE

#include "ssGUI/Extensions/Extension.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: ssGUI::Extensions::Outline
    Outline allows to create a colored outline surrounding the target GUI Object shape/vertices.

    A GUI Object can be made up of multiple shapes, by default the outline extension outline the first shape of the GUI Object.

    By default, the outline is drawn inside the shape. This however can be set by <SetInnerOutline>.

    You can also target specific vertices of the GUI Object to be outlined (<AddTargetVertex>), in which case <AddTargetShape> will be overriden and ignored.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;
        bool Enabled;

        float OutlineThickness;
        bool SimpleOutline;
        bool InnerOutline;
        glm::u8vec4 OutlineColor;
        std::vector<int> TargetShapes;
        std::vector<int> TargetVertices;

        std::vector<int> VerticesToOutline;
        std::vector<int> VerticesToOutlinePrevVertices;
        std::vector<int> VerticesToOutlineNextVertices;
        std::vector<int> VerticesToOutlineNextNextVertices;
        std::vector<int> VerticesToOutlineShapeIndex;
        std::vector<bool> VerticesToOutlineShapeStartFlag;
    =================================================================
    ============================== C++ ==============================
    Outline::Outline() : Container(nullptr), Enabled(true), OutlineThickness(1.1), SimpleOutline(false), InnerOutline(true), 
                            OutlineColor(glm::u8vec4(0, 0, 0, 255)), TargetShapes{0}, TargetVertices(), VerticesToOutline(), 
                            VerticesToOutlinePrevVertices(), VerticesToOutlineNextVertices(), VerticesToOutlineNextNextVertices(), 
                            VerticesToOutlineShapeIndex(), VerticesToOutlineShapeStartFlag()
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
            ssGUI::GUIObject* Container;
            bool Enabled;

            float OutlineThickness;
            bool SimpleOutline;
            bool InnerOutline;
            glm::u8vec4 OutlineColor;
            std::vector<int> TargetShapes;
            std::vector<int> TargetVertices;

            std::vector<int> VerticesToOutline;
            std::vector<int> VerticesToOutlinePrevVertices;
            std::vector<int> VerticesToOutlineNextVertices;
            std::vector<int> VerticesToOutlineNextNextVertices;
            std::vector<int> VerticesToOutlineShapeIndex;
            std::vector<bool> VerticesToOutlineShapeStartFlag;

            Outline();
            virtual ~Outline() override;
            Outline(Outline const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int>const & drawingCounts, int& shapeIndex);
            virtual void UpdateVerticesForOutline();

            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            constexpr double pi() { return std::atan(1)*4; };

            //Return angle in radians. Positive if angle between a and b is anti-clockwise
            virtual double GetAngle(glm::vec2 a, glm::vec2 b);

            virtual bool FindInnerOutlinesIntersection(glm::vec2 curVertex, glm::vec2 prevVertex, glm::vec2 nextVertex, float outlineThickness, glm::vec2& intersection);

            virtual void PlotArc(glm::vec2 start, glm::vec2 end, glm::vec2 circlePos, std::vector<glm::vec2>& plottedPoints);

            virtual void ConstructComplexOutline(bool isInner);

            virtual void ConstructSimpleOutline();

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

        public:
            static const std::string EXTENSION_NAME;

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
            virtual Outline* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif
#ifndef SSGUI_ROUNDED_CORNERS
#define SSGUI_ROUNDED_CORNERS
#include <cmath>
#include <map>
#include "glm/trigonometric.hpp"
#include "glm/geometric.hpp"
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/BaseClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: RoundedCorners
    This extension rounds the corner of the selected shapes or vertices on the GUI object. This works best with anti-aliasing.

    You should get familiar with <GUIObject::Extension_GetDrawingVertices>, <GUIObject::Extension_GetDrawingUVs>, <GUIObject::Extension_GetDrawingColours>, 
    <GUIObject::Extension_GetDrawingCounts> if using this.

    The indices for TargetShapes are mapped to <GUIObject::Extension_GetDrawingCounts> indicating the index of a shape.

    The indices for TargetVertices are mapped to <GUIObject::Extension_GetDrawingVertices> indicating the index of a vertex.

    You can add shapes to be rounded by calling <AddTargetShape>. You can also round specific vertices added by calling
    <AddTargetVertex>. However, target shape will be *ignored* if target vertices are used.

    Variables & Constructor:
    ============================== C++ ==============================
    private:
        ssGUI::GUIObject* Container;
        bool Enabled;

        int RoundedCornersRadius;
        std::vector<int> TargetShapes;
        std::vector<int> TargetVertices;

        std::vector<int> VerticesToRound;
        std::vector<int> VerticesToRoundPrevVertices;
        std::vector<int> VerticesToRoundNextVertices;
    =================================================================
    ============================== C++ ==============================
    RoundedCorners::RoundedCorners() : Container(nullptr), Enabled(true), RoundedCornersRadius(20), TargetShapes{0}, TargetVertices(), VerticesToRound(), 
                                        VerticesToRoundPrevVertices(), VerticesToRoundNextVertices()
    {}
    =================================================================
    */
    class RoundedCorners : public Extension
    {
        private:
            RoundedCorners& operator=(RoundedCorners const& other);

            ssGUI::GUIObject* Container;
            bool Enabled;

            int RoundedCornersRadius;
            std::vector<int> TargetShapes;
            std::vector<int> TargetVertices;

            std::vector<int> VerticesToRound;
            std::vector<int> VerticesToRoundPrevVertices;
            std::vector<int> VerticesToRoundNextVertices;


        protected:
            RoundedCorners(RoundedCorners const& other);

            //Return angle in radians. Positive if clockwise
            virtual double GetAngle(glm::ivec2 a, glm::ivec2 b);
            
            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            constexpr double pi() { return std::atan(1)*4; };
            
            //https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
            virtual glm::dvec3 Barycentric(glm::ivec2 samplePoint, glm::ivec2 a, glm::ivec2 b, glm::ivec2 c);

            virtual void PlotArcPoints(glm::ivec2 a, glm::ivec2 b, glm::ivec2 c, std::vector<glm::ivec2>& plottedPoints);
            
            virtual void GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int> const & drawingCounts);
            virtual void UpdateVerticesForRounding();

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;

        public:
            static const std::string EXTENSION_NAME;

            RoundedCorners();
            virtual ~RoundedCorners() override;

            //function: SetRoundedCornersRadius
            //Sets the radius for the rounded corners. Note that it will use the largest possible radius if this is not available.
            virtual void SetRoundedCornersRadius(int radius);

            //function: GetRoundedCornersRadius
            //Returns the radius for the rounded corners
            virtual int GetRoundedCornersRadius() const;

            //function: AddTargetShape
            //Adds the shapeIndex that indicates the index of the shape to be rounded drawn in GUI object.
            //Note that if you have added any target vertices (<AddTargetVertex>), this will be *ignored*.
            virtual void AddTargetShape(int shapeIndex);

            //function: GetTargetShape
            //Returns the shapeIndex that indicates the index of the shape to be rounded drawn in GUI object,
            //by specifying the location shapedIndex that is stored in this extension.
            virtual int GetTargetShape(int location) const;

            //function: SetTargetShape
            //Sets the shapeIndex that indicates the index of the shape to be rounded drawn in GUI object,
            //by specifying the location shapedIndex that is stored in this extension.
            virtual void SetTargetShape(int location, int shapeIndex);

            //function: GetTargetShapesCount
            //Returns the number of shapes to be rounded (number of shapeIndex) that are stored in this extension. 
            virtual int GetTargetShapesCount() const;

            //function: RemoveTargetShape
            //Removes the shapeIndex that indicates the index of the shape to be rounded drawn in GUI object,
            //by specifying the location shapedIndex that is stored in this extension.
            virtual void RemoveTargetShape(int location);

            //function: ClearTargetShapes
            //Clears all the shapeIndex entries in this extension 
            virtual void ClearTargetShapes();

            //function: AddTargetVertex
            //Adds the target vertex that indicates the index of the vertex to be rounded drawn in GUI object,
            //by specifying the location vertexIndex that is stored in this extension.
            //Note that if you are adding any target vertices, any target shapes added (<AddTargetShape>) this will be ignored.
            virtual void AddTargetVertex(int vertexIndex);

            //function: GetTargetVertex
            //Returns the vertexIndex that indicates the index of the vertex to be rounded drawn in GUI object,
            //by specifying the location vertexIndex that is stored in this extension.
            virtual int GetTargetVertex(int location) const;

            //function: SetTargetVertex
            //Sets the vertexIndex that indicates the index of the vertex to be rounded drawn in GUI object,
            //by specifying the location vertexIndex that is stored in this extension.
            virtual void SetTargetVertex(int location, int vertexIndex);

            //function: GetTargetVerticesCount
            //Returns the number of indices to be rounded (number of vertexIndex) that are stored in this extension. 
            virtual int GetTargetVerticesCount() const;

            //function: RemoveTargetVertex
            //Removes the vertexIndex that indicates the index of the vertex to be rounded drawn in GUI object,
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
            virtual void Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
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
#ifndef SSGUI_ROUNDED_CORNERS_H
#define SSGUI_ROUNDED_CORNERS_H
#include <cmath>
#include <map>
#include "glm/trigonometric.hpp"
#include "glm/geometric.hpp"
#include "ssGUI/DataClasses/ShapeModifier.hpp"
#include "ssGUI/DataClasses/TargetShape.hpp"
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject

namespace ssGUI 
{
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::RoundedCorners
    This extension rounds the corner of the selected shapes or vertices on the GUI object. This works best with anti-aliasing.

    You should get familiar with <Renderer::Extension_GetDrawingVertices>, <Renderer::Extension_GetDrawingUVs>, <Renderer::Extension_GetDrawingColours>, 
    <Renderer::Extension_GetDrawingCounts> if using this.

    The indices for TargetShapes are mapped to <Renderer::Extension_GetDrawingCounts> indicating the index of a shape.

    The indices for TargetVertices are mapped to <Renderer::Extension_GetDrawingVertices> indicating the index of a vertex.

    You can add shapes to be rounded by calling <AddTargetShape>. You can also round specific vertices added by calling
    <AddTargetVertex>. However, target shape will be *ignored* if target vertices are used.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:        
        ssGUI::GUIObject* Container;                            //See <BindToObject>
        bool Enabled;                                           //See <IsEnabled>
        float RoundedCornersRadius;                             //See <GetRoundedCornersRadius>
        ssGUI::ShapeModifier ModifiedShapes;                    //(Internal variable) Used to store list of shapes to be modified
    =================================================================
    ============================== C++ ==============================
    RoundedCorners::RoundedCorners() :  Container(nullptr),
                                        Enabled(true),
                                        RoundedCornersRadius(10),
                                        ModifiedShapes()
    {
        ModifiedShapes.AddTargetShape(ssGUI::TargetShape(0));
    }
    =================================================================
    */
    class RoundedCorners : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            RoundedCorners& operator=(RoundedCorners const& other);

        protected:        
            ssGUI::GUIObject* Container;                            //See <BindToObject>
            bool Enabled;                                           //See <IsEnabled>
            float RoundedCornersRadius;                             //See <GetRoundedCornersRadius>
            ssGUI::ShapeModifier ModifiedShapes;                    //(Internal variable) Used to store list of shapes to be modified

            RoundedCorners();
            virtual ~RoundedCorners() override;
            RoundedCorners(RoundedCorners const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            //Return angle in radians. Positive if angle between a and b is clockwise
            virtual double GetAngle(glm::vec2 a, glm::vec2 b);
            
            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            double pi() const { return std::atan(1)*4; };
            
            //https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
            virtual glm::vec3 Barycentric(glm::vec2 samplePoint, glm::vec2 a, glm::vec2 b, glm::vec2 c);

            virtual void PlotArcPoints(glm::vec2 a, glm::vec2 b, glm::vec2 c, std::vector<glm::vec2>& plottedPoints);
            
            virtual void GetStartEndVertexIndex(int currentIndex, int& startIndex, int& endIndex, std::vector<int> const & drawingCounts);

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

        public:
            static const std::string EXTENSION_NAME;

            //function: SetRoundedCornersRadius
            //Sets the radius for the rounded corners. Note that it will use the largest possible radius if this is not available.
            virtual void SetRoundedCornersRadius(float radius);

            //function: GetRoundedCornersRadius
            //Returns the radius for the rounded corners
            virtual float GetRoundedCornersRadius() const;

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
            virtual void Internal_Update(   bool isPreUpdate, 
                                            ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) override;
            
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
            virtual RoundedCorners* Clone() override;
    };
}

}



#endif
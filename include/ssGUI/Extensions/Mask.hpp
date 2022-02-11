#ifndef SSGUI_MASK
#define SSGUI_MASK


#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/BaseClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "ssGUI/BaseClasses/Window.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"
#include <vector>
#include <cmath>
#include <queue>

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: Mask
    Mask extension serves 2 purposes:
    - Provides function to mask GUI Objects
    - Manages <MaskEnforcer> that actually uses the function in this extension to mask the GUI objects that the <MaskEnforcer> attached to.
    This includes adding (and removing) <MaskEnforcer> automatically to the children or to a specific GUI object. 
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;
        bool Enabled;
        bool MaskChildren;
        bool MaskContainer;
        bool FollowContainer;
        glm::ivec2 FollowPositionOffset;
        glm::ivec2 FollowSizePadding;

        glm::ivec2 GlobalPosition;
        glm::ivec2 Size;

        int ChildAddedEventIndex;
        int ChildRemovedEventIndex;
    =================================================================
    ============================== C++ ==============================
    Mask::Mask() :  Container(nullptr), Enabled(true), MaskChildren(true), MaskContainer(false), FollowContainer(true), 
                    FollowPositionOffset(), FollowSizePadding(), GlobalPosition(), Size(),
                    ChildAddedEventIndex(-1), ChildRemovedEventIndex(-1)
    {}
    =================================================================    
    */
    class Mask : public Extension
    {
        private:
            Mask& operator=(Mask const& other);
        
        protected:
            ssGUI::GUIObject* Container;
            bool Enabled;
            bool MaskChildren;
            bool MaskContainer;
            bool FollowContainer;
            glm::ivec2 FollowPositionOffset;
            glm::ivec2 FollowSizePadding;

            glm::ivec2 GlobalPosition;
            glm::ivec2 Size;

            int ChildAddedEventIndex;
            int ChildRemovedEventIndex;

            virtual bool IsContained(glm::ivec2 point, glm::ivec2 min, glm::ivec2 max) const;
            virtual bool LineToLineIntersection(    glm::ivec2 linePointA, glm::ivec2 linePointB, 
                                            glm::ivec2 linePointC, glm::ivec2 linePointD, glm::ivec2& intersection);
            virtual bool GetAxesValues(    glm::ivec2 axis, glm::ivec2 axis2, glm::ivec2 samplePoint, double& axisValue, double& axis2Value);
/*             bool LineToShapeIntersection(   glm::ivec2 linePointA, glm::ivec2 linePointB,
                                            std::vector<glm::ivec2>& shape, int& shapeIntersectionIndex, glm::ivec2& intersection);*/
            virtual bool CheckLinesOverlapping(int minA, int maxA, int minB, int maxB);
            virtual bool IsAABBOverlap(std::vector<glm::ivec2>& shapeVerticies, int shapeOffset, int shapeVertexCount, glm::ivec2 maskMin, glm::ivec2 maskMax,
                                glm::ivec2& shapeMin, glm::ivec2& shapeMax);
            
            virtual void GetIntersections(std::vector<glm::ivec2>& intersections, std::vector<int>& shapeIntersectIndices, std::vector<int>& maskIntersectIndices,
                                    std::vector<glm::ivec2>& shapeVerticies, int shapeOffset, int shapeVertexCount, std::vector<glm::ivec2>& maskVerticies);
            
            virtual void FromNewShapeWithIntersections(std::vector<glm::ivec2>& currentShapeVertices, std::vector<glm::ivec2>& currentShapeUVs, 
                                                std::vector<glm::u8vec4>& currentShapeColours, std::vector<bool>& currentVertexChanged,
                                                std::vector<glm::ivec2>& originalVerticies, std::vector<glm::u8vec4>& originalColours,
                                                std::vector<glm::ivec2>& originalUVs,
                                                glm::ivec2 maskMin, glm::ivec2 maskMax, int shapeOffset, int shapeVertexCount, 
                                                std::vector<glm::ivec2>& maskVerticies, std::vector<glm::ivec2>& intersections,
                                                std::vector<int>& shapeIntersectIndices, std::vector<int>& maskIntersectIndices);

            virtual void SampleNewUVsAndColoursForShapes(std::vector<glm::ivec2>& originalVerticies, std::vector<glm::ivec2>& originalUVs, std::vector<glm::u8vec4>& originalColours,
                                                std::vector<int>& verticesCount, std::vector<glm::ivec2>& newVertices, std::vector<glm::ivec2>& newUVs, 
                                                std::vector<glm::u8vec4>& newColours, std::vector<bool>& changed, std::vector<int>& newVerticesCount);


//            int GetIndicesDistance(int count, int startIndex, int endIndex);
            virtual int GetNextIndex( int indexOffset, int count, int currentIndex );
            virtual int GetPreviousIndex( int indexOffset, int count, int currentIndex );
            virtual bool GetSampleIndicesFromShape(std::vector<glm::ivec2>& vertices, int closestIndices[], glm::ivec2 samplePoint);

            virtual void AddMaskEnforcerToChildren(ssGUI::GUIObject* parent, bool includeParent);
            virtual void RemoveMaskEnforcerToChildren(ssGUI::GUIObject* parent, bool includeParent);

            Mask(Mask const& other);

        public:
            static const std::string EXTENSION_NAME;

            Mask();
            virtual ~Mask() override;

            //function: SetMaskChildren
            //If true, this will add <MaskEnforcer> automatically to the children
            virtual void SetMaskChildren(bool maskChildren);
            
            //function: GetMaskChildren
            //If true, this will add <MaskEnforcer> automatically to the children
            virtual bool GetMaskChildren() const;
            
            //function: SetMaskContainer
            //If true, a <MaskEnforcer> will be added to the container. This should normally be false.
            virtual void SetMaskContainer(bool maskContainer);
            
            //function: GetMaskContainer
            //If true, a <MaskEnforcer> will be added to the container. This should normally be false.
            virtual bool GetMaskContainer() const;
            
            //function: SetFollowContainer
            //If true, the mask will follow the container (Setting the position of the mask to be the same as Container) and will be the same size as the container. This should normally be true.
            virtual void SetFollowContainer(bool followContainer);
            
            //function: GetFollowContainer
            //If true, the mask will follow the container (Setting the position of the mask to be the same as Container) and will be the same size as the container. This should normally be true.
            virtual bool GetFollowContainer() const;
            
            //function: SetFollowPositionOffset
            virtual void SetFollowPositionOffset(glm::ivec2 positionOffset);
            
            //function: GetFollowPositionOffset
            virtual glm::ivec2 GetFollowPositionOffset() const;
            
            //function: SetFollowSizePadding
            //Padding horizontally and vertically (by shrinking the size of the mask)
            virtual void SetFollowSizePadding(glm::ivec2 sizePadding);
            
            //function: GetFollowSizePadding
            //Padding horizontally and vertically (by shrinking the size of the mask)
            virtual glm::ivec2 GetFollowSizePadding() const;
            
            //function: SetGlobalPosition
            //Sets the global position for the mask
            virtual void SetGlobalPosition(glm::ivec2 globalPosition);
            
            //function: GetGlobalPosition
            //Gets the global position for the mask
            virtual glm::ivec2 GetGlobalPosition() const;
            
            //function: SetSize
            //Sets the size of the mask
            virtual void SetSize(glm::ivec2 size);
            
            //function: GetSize
            //Gets the size of the mask
            virtual glm::ivec2 GetSize() const;

            //function: IsPointContainedInMask
            //If true, the (global) point is inside the mask
            virtual bool IsPointContainedInMask(glm::ivec2 point) const;
            
            //function: Internal_OnRecursiveChildAdded
            //(Internal ssGUI function) Trigger function when a child is added
            virtual void Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child);
            
            //function: Internal_OnRecursiveChildRemoved
            //(Internal ssGUI function) Trigger function when a child is removed
            virtual void Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child);
            
            //function: MaskObject
            //Public function for masking a GUI object
            virtual void MaskObject(ssGUI::GUIObject* obj, glm::ivec2 renderOffset);

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
            virtual void Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::ivec2 mainWindowPositionOffset) override;
            
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
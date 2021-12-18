#ifndef SSGUI_MASK
#define SSGUI_MASK


#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/BaseClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "ssGUI/BaseClasses/Window.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildrenAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildrenRemovedEventCallback.hpp"
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
    //class: Mask
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

            int OnChildAddedEventIndex;
            int OnChildRemovedEventIndex;

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
            virtual void SetMaskChildren(bool maskChildren);
            
            //function: GetMaskChildren
            virtual bool GetMaskChildren() const;
            
            //function: SetMaskContainer
            virtual void SetMaskContainer(bool maskContainer);
            
            //function: GetMaskContainer
            virtual bool GetMaskContainer() const;
            
            //function: SetFollowContainer
            virtual void SetFollowContainer(bool followContainer);
            
            //function: GetFollowContainer
            virtual bool GetFollowContainer() const;
            
            //function: SetFollowPositionOffset
            virtual void SetFollowPositionOffset(glm::ivec2 positionOffset);
            
            //function: GetFollowPositionOffset
            virtual glm::ivec2 GetFollowPositionOffset() const;
            
            //function: SetFollowSizePadding
            virtual void SetFollowSizePadding(glm::ivec2 sizePadding);
            
            //function: GetFollowSizePadding
            virtual glm::ivec2 GetFollowSizePadding() const;
            
            //function: SetGlobalPosition
            virtual void SetGlobalPosition(glm::ivec2 globalPosition);
            
            //function: GetGlobalPosition
            //If followingContainer is true, Global position is only valid until the last update
            virtual glm::ivec2 GetGlobalPosition() const;
            
            //function: SetSize
            virtual void SetSize(glm::ivec2 size);
            
            //function: GetSize
            virtual glm::ivec2 GetSize() const;

            //function: IsPointContainedInMask
            virtual bool IsPointContainedInMask(glm::ivec2 point) const;
            
            //function: Internal_OnRecursiveChildAdded
            virtual void Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child);
            
            //function: Internal_OnRecursiveChildRemoved
            virtual void Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child);
            
            //function: MaskObject
            virtual void MaskObject(ssGUI::GUIObject* obj, glm::ivec2 renderOffset);

            //Override from Extension
            virtual void SetEnabled(bool enabled) override;

            virtual bool IsEnabled() const override;

            //function: Update
            virtual void Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;;
            
            //function: Draw
            virtual void Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;
            
            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

#endif
#ifndef SSGUI_SHAPE_MODIFIER_HPP
#define SSGUI_SHAPE_MODIFIER_HPP

#include "ssGUI/DataClasses/TargetShape.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "glm/vec2.hpp"
#include <unordered_map>
#include <unordered_set>
#include <vector>

//namespace: ssGUI
namespace ssGUI
{
    //struct: ssGUI::VerticesIndicesForShape
    struct VerticesIndicesForShape
    {
        ssGUI::TargetShape Shape;
        std::vector<int> VerticesIndices;
        
        VerticesIndicesForShape(ssGUI::TargetShape shape, std::vector<int> verticesIndices) :    Shape(shape),
                                                                                                VerticesIndices(verticesIndices)
        {}
        
        VerticesIndicesForShape(ssGUI::TargetShape& shape, std::vector<int>& verticesIndices) :  Shape(shape),
                                                                                                VerticesIndices(verticesIndices)
        {}
        
        VerticesIndicesForShape(ssGUI::TargetShape&& shape, std::vector<int>&& verticesIndices) :    Shape(shape),
                                                                                                    VerticesIndices(verticesIndices)
        {}
        
        VerticesIndicesForShape() = default;
    };

    //class: ssGUI::ShapeModifier
    class ShapeModifier
    {
        private:
            std::unordered_set<std::string> TargetShapesNameSet;
            std::unordered_set<int> TargetShapesIndexSet;

            std::unordered_map<std::string, std::vector<int>> TargetVerticesNameMap;
            std::unordered_map<int, std::vector<int>> TargetVerticesIndexMap;
        
        public:
            std::vector<ssGUI::TargetShape> TargetShapes;
            std::vector<VerticesIndicesForShape> TargetVertices;
            
            //function: UpdateShapesToBeModified
            inline void UpdateShapesToBeModified(int firstShapeIndex)
            {
                TargetShapesNameSet.clear();
                TargetShapesIndexSet.clear();
                TargetVerticesNameMap.clear();
                TargetVerticesIndexMap.clear();
                
                if(!TargetVertices.empty())
                {
                    for(int i = 0; i < TargetVertices.size(); i++)
                    {
                        if(TargetVertices[i].Shape.ShapeIndex != NO_SHAPE_INDEX)
                        {
                            std::vector<int>& verts = TargetVerticesIndexMap[TargetVertices[i].Shape.ShapeIndex + firstShapeIndex];
                            verts.insert(verts.end(), TargetVertices[i].VerticesIndices.begin(), TargetVertices[i].VerticesIndices.end());
                        }
                        else if(!TargetVertices[i].Shape.ShapeName.empty())
                        {
                            std::vector<int>& verts = TargetVerticesNameMap[TargetVertices[i].Shape.ShapeName];
                            verts.insert(verts.end(), TargetVertices[i].VerticesIndices.begin(), TargetVertices[i].VerticesIndices.end());
                        }
                        else
                            ssGUI_WARNING(ssGUI_DATA_TAG, "TargetVertices' shape is not properly set!!");
                    }
                }
                else
                {
                    for(int i = 0; i < TargetShapes.size(); i++)
                    {
                        if(TargetShapes[i].ShapeIndex != NO_SHAPE_INDEX)
                            TargetShapesIndexSet.insert(TargetShapes[i].ShapeIndex + firstShapeIndex);
                        else if(!TargetShapes[i].ShapeName.empty())
                            TargetShapesNameSet.insert(TargetShapes[i].ShapeName);
                        else
                            ssGUI_WARNING(ssGUI_DATA_TAG, "TargetShape is not properly set!!");
                    }
                }
            }
            
            //function: IsModifyWholeShape
            inline bool IsModifyWholeShape()
            {
                return TargetVertices.empty();
            }
            
            //function: IsShapeModified
            inline bool IsShapeModified(std::string shapeName)
            {
                return TargetShapesNameSet.find(shapeName) != TargetShapesNameSet.end();
            }
            
            //function: IsShapeModified
            inline bool IsShapeModified(int rawShapeIndex)
            {
                return TargetShapesIndexSet.find(rawShapeIndex) != TargetShapesIndexSet.end();
            }
            
            //function: GetVerticesToBeModified
            inline std::vector<int>* GetVerticesToBeModified(std::string shapeName)
            {
                if(TargetVerticesNameMap.find(shapeName) != TargetVerticesNameMap.end())
                    return &TargetVerticesNameMap[shapeName];
                else
                    return nullptr;
            }
            
            //function: GetVerticesToBeModified
            inline std::vector<int>* GetVerticesToBeModified(int rawShapeIndex)
            {
                if(TargetVerticesIndexMap.find(rawShapeIndex) != TargetVerticesIndexMap.end())
                    return &TargetVerticesIndexMap[rawShapeIndex];
                else
                    return nullptr;
            }
            
            inline int GetNextIndex(int currentIndex, std::vector<glm::vec2>& verts)
            {
                glm::vec2 curVert = verts.at(currentIndex);
                
                int nextIndex = currentIndex;
                for(int i = 0; i < verts.size(); i++)
                {
                    nextIndex = (nextIndex + 1) % (int)verts.size();
                    glm::vec2 nextVert = verts.at(nextIndex);
                    
                    if(nextVert - curVert != glm::vec2())
                        return nextIndex;
                }
                
                ssGUI_ERROR(ssGUI_DATA_TAG, "GetNextIndex failed");
                return -1;
            }
            
            inline int GetPrevIndex(int currentIndex, std::vector<glm::vec2>& verts)
            {
                glm::vec2 curVert = verts.at(currentIndex);
                
                int prevIndex = currentIndex;
                for(int i = 0; i < verts.size(); i++)
                {
                    prevIndex = (prevIndex - 1) % (int)verts.size();
                    if(prevIndex < 0)
                        prevIndex = verts.size() - 1;

                    glm::vec2 prevVert = verts.at(prevIndex);
                    
                    if(prevVert - curVert != glm::vec2())
                        return prevIndex;
                }
                
                ssGUI_ERROR(ssGUI_DATA_TAG, "GetPrevIndex failed");
                return -1;
            }
            
            //================================================================
            //Functions to be proxy
            //================================================================
            
            //function: AddTargetShape
            //Adds the <ssGUI::TargetShape> to be modified in GUI object.
            //Note that if you have added any target vertices (<AddTargetVertex>), this will be *ignored*.
            //Returns the location of the added <ssGUI::TargetShape>
            inline int AddTargetShape(ssGUI::TargetShape targetShape)
            {
                TargetShapes.push_back(targetShape);
                return TargetShapes.size() - 1;
            }

            //function: GetTargetShape
            //Returns the <ssGUI::TargetShape> to be modified in GUI object,
            //by specifying the location of <ssGUI::TargetShape>.
            inline ssGUI::TargetShape GetTargetShape(int location) const
            {
                return TargetShapes[location];
            }

            //function: SetTargetShape
            //Sets the <ssGUI::TargetShape> to be modified in GUI object,
            //by specifying the location of <ssGUI::TargetShape>.
            inline void SetTargetShape(int location, ssGUI::TargetShape targetShape)
            {
                TargetShapes[location] = targetShape;
            }

            //function: GetTargetShapesCount
            //Returns the number of shapes to be modified as a whole 
            inline int GetTargetShapesCount() const
            {
                return TargetShapes.size();
            }

            //function: RemoveTargetShape
            //Removes the <ssGUI::TargetShape> to be modified in GUI object,
            //by specifying the location of <ssGUI::TargetShape>.
            inline void RemoveTargetShape(int location)
            {
                TargetShapes.erase(TargetShapes.begin() + location);
            }

            //function: ClearTargetShapes
            //Clears all the <ssGUI::TargetShape> to be modifed as a whole 
            inline void ClearTargetShapes()
            {
                TargetShapes.clear();
            }

            //function: AddTargetVertex
            //Adds the target vertex index to be modified in the <ssGUI::TargetShape> in GUI object.
            //Note that if you are adding any target vertices, any target shapes added (<AddTargetShape>) this will be ignored.
            //Returns the location of the added <ssGUI::TargetShape>
            inline int AddTargetVertex(ssGUI::TargetShape targetShape, int vertexIndex)
            {
                int foundIndex = -1;
                for(int i = 0; i < TargetVertices.size(); i++)
                {
                    if( (targetShape.ShapeIndex != NO_SHAPE_INDEX && targetShape.ShapeIndex == TargetVertices[i].Shape.ShapeIndex) ||
                        (!targetShape.ShapeName.empty() && targetShape.ShapeName == TargetVertices[i].Shape.ShapeName))
                    {
                        foundIndex = i;
                        break;
                    }
                }
            
                if(foundIndex >= 0)
                {
                    TargetVertices[foundIndex].VerticesIndices.push_back(vertexIndex);
                    return foundIndex;
                }
                else
                {
                    VerticesIndicesForShape vertsForShape;
                    vertsForShape.Shape = targetShape;
                    vertsForShape.VerticesIndices.push_back(vertexIndex);
                    TargetVertices.push_back(vertsForShape);
                    return TargetVertices.size() - 1;
                }
            }

            //function: GetTargetVertex
            //Returns the <ssGUI::VerticesIndicesForShape> of the <ssGUI::TargetShape>> to be modified in GUI object,
            //by specifying the location of <ssGUI::TargetShape>.
            inline VerticesIndicesForShape GetTargetVertices(int location) const
            {
                return VerticesIndicesForShape(TargetVertices[location].Shape, TargetVertices[location].VerticesIndices);
            }
            
            inline VerticesIndicesForShape GetTargetVertices(ssGUI::TargetShape targetShape) const
            {
                int foundIndex = -1;
                for(int i = 0; i < TargetVertices.size(); i++)
                {
                    if( (targetShape.ShapeIndex != NO_SHAPE_INDEX && targetShape.ShapeIndex == TargetVertices[i].Shape.ShapeIndex) ||
                        (!targetShape.ShapeName.empty() && targetShape.ShapeName == TargetVertices[i].Shape.ShapeName))
                    {
                        foundIndex = i;
                        break;
                    }
                }
                
                if(foundIndex >= 0)
                    return VerticesIndicesForShape(TargetVertices[foundIndex].Shape, TargetVertices[foundIndex].VerticesIndices);
                else
                    return VerticesIndicesForShape();
            }
            
            //function: SetTargetVertex
            //Sets the vertices of the <ssGUI::TargetShape> to be modified in GUI object,
            //by specifying the location of <ssGUI::TargetShape>.
            inline void SetTargetVertices(ssGUI::TargetShape targetShape, const std::vector<int>& vertices)
            {
                int foundIndex = -1;
                for(int i = 0; i < TargetVertices.size(); i++)
                {
                    if( (targetShape.ShapeIndex != NO_SHAPE_INDEX && targetShape.ShapeIndex == TargetVertices[i].Shape.ShapeIndex) ||
                        (!targetShape.ShapeName.empty() && targetShape.ShapeName == TargetVertices[i].Shape.ShapeName))
                    {
                        foundIndex = i;
                        break;
                    }
                }
            
                if(foundIndex >= 0)
                    TargetVertices[foundIndex].VerticesIndices = vertices;
            }

            inline void SetTargetVertices(int location, const std::vector<int>& vertices)
            {
                if(location >= 0 && location < TargetVertices.size())
                    TargetVertices[location].VerticesIndices = vertices;
            }

            //function: GetTargetVerticesCount
            //Returns the number of <ssGUI::TargetShape> to be modified per vertex
            inline int GetTargetVerticesCount() const
            {
                return TargetVertices.size();
            }

            //function: RemoveTargetVertex
            //Removes the <ssGUI::TargetShape> to be modified per vertex in GUI object,
            //by specifying the location of <ssGUI::TargetShape>.
            inline void RemoveTargetVertex(int location)
            {
                TargetVertices.erase(TargetVertices.begin() + location);
            }

            //function: ClearTargetVertices
            //Clears all <ssGUI::TargetShape> to be modifed per vertex
            inline void ClearTargetVertices()
            {
                TargetVertices.clear();
            }
    };
}


#endif
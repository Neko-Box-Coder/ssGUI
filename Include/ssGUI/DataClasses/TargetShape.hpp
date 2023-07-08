#ifndef SSGUI_TARGET_SHAPE_HPP 
#define SSGUI_TARGET_SHAPE_HPP

#include <string>

//namespace ssGUI
namespace ssGUI
{
    //const: NO_SHAPE_INDEX
    const int NO_SHAPE_INDEX = -10000;

    //struct: ssGUI::TargetShape
    struct TargetShape
    {
        //var: ShapeIndex
        //The index of specified shape, relative to the first shape of the GUI Object.
        //0 will mean the first shape created by the GUI Object, -1 will mean this field is not used instead.
        //See <ssGUI::Renderer::Extension_GetGUIObjectFirstShapeIndex> for more details of the first shape.
        int ShapeIndex = NO_SHAPE_INDEX;
        
        //var: ShapeName
        //The name of the specified shape, empty if this field is not used.
        std::string ShapeName = "";
    
        TargetShape() = default;
        
        inline TargetShape(int shapeIndex) :    ShapeIndex(shapeIndex),
                                                ShapeName() 
        {}
        
        inline TargetShape(std::string shapeName) : ShapeIndex(NO_SHAPE_INDEX),
                                                    ShapeName(shapeName)
        {}
    };
}

#endif
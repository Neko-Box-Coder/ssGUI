#include "ssGUI/Extensions/BoxShadow.hpp" //Or the location of your custom extension header

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <cmath>

namespace ssGUI
{

namespace Extensions
{
    BoxShadow::BoxShadow() :    Container(nullptr),
                                Enabled(true),
                                PositionOffset(0, 0),
                                SizeOffset(5, 5),
                                BlurRadius(10),
                                ShadowColor(0, 0, 0, 127)
    {}

    BoxShadow::~BoxShadow()
    {}

    BoxShadow::BoxShadow(BoxShadow const& other)
    {
        Container = nullptr;
        Enabled = other.IsEnabled();
        PositionOffset = other.GetPositionOffset();
        SizeOffset = other.GetSizeOffset();
        BlurRadius = other.GetBlurRadius();
        ShadowColor = other.GetShadowColor();
    }

    double BoxShadow::GetAngle(glm::vec2 a, glm::vec2 b)
    {
        glm::vec3 a3 = glm::vec3(a, 0);
        glm::vec3 b3 = glm::vec3(b, 0);

        return atan2(glm::cross(a3, b3).z, glm::dot(glm::vec2(a), glm::vec2(b)));
    }

    void BoxShadow::PlotArc(glm::vec2 start, glm::vec2 end, glm::vec2 circlePos, std::vector<glm::vec2>& plottedPoints)
    {
        glm::vec2 cirOriginline = glm::vec2(1, 0);
        glm::vec2 startDir = (start - circlePos);
        glm::vec2 endDir = (end - circlePos);
        double arcRadius = glm::distance(start, circlePos);
        double startToEndAngle = GetAngle(startDir, endDir);

        bool invalidAngle = false;
        if(startToEndAngle < 0)
        {
            ssGUI_WARNING(ssGUI_EXT_TAG, "anti-clockwise placements of vertices detected. Rounded corners failed.");
            invalidAngle = true;
        }
        else if(startToEndAngle > pi())
        {
            ssGUI_WARNING(ssGUI_EXT_TAG, "Angle between 2 tangents should not be larger than 180 degrees. Rounded corners failed.");
            invalidAngle = true;
        }

        if(invalidAngle)
        {
            ssGUI_ERROR(ssGUI_EXT_TAG, "startToEndAngle: "<<startToEndAngle);
            ssGUI_ERROR(ssGUI_EXT_TAG, "start: "<<start.x<<", "<<start.y);
            ssGUI_ERROR(ssGUI_EXT_TAG, "end: "<<end.x<<", "<<end.y);
            ssGUI_ERROR(ssGUI_EXT_TAG, "circlePos: "<<circlePos.x<<", "<<circlePos.y);
            ssLOG_EXIT_PROGRAM();
            return;
        }

        double originLineToStartAngle = GetAngle(cirOriginline, startDir);
        originLineToStartAngle = originLineToStartAngle < 0 ? 2 * pi() + originLineToStartAngle : originLineToStartAngle;

        //https://stackoverflow.com/questions/15525941/find-points-on-circle
        //Plot the arc
        //std::vector<glm::ivec2> arcVertices = std::vector<glm::ivec2>();
        // ssGUI_DEBUG(ssGUI_EXT_TAG, "points: "<<((int)(arcRadius * startToEndAngle * 1) + 2));        
        for(int i = 0; i < (int)(arcRadius * startToEndAngle * 1) + 2; i++)
        {
            double currentAngle = originLineToStartAngle + startToEndAngle * ((double)i / (double)((int)(arcRadius * startToEndAngle * 1) + 1));
            glm::dvec2 plotPoint = glm::dvec2(cos(currentAngle), sin(currentAngle)) * (double)arcRadius;
            plottedPoints.push_back(/*glm::ivec2(round(plotPoint.x), round(plotPoint.y))*/glm::vec2(plotPoint) + circlePos);
        }
    }

    void BoxShadow::ConstructRenderInfo()
    {
        ssGUI_LOG_FUNC();
        //Getting all the rendering details from container
        std::vector<ssGUI::DrawingEntity>& drawingEntities = Container->Extension_GetDrawingEntities();

        std::vector<ssGUI::DrawingEntity> newEntities;

        auto shadowPos = Container->GetGlobalPosition() + GetPositionOffset() + GetBlurRadius();
        auto shadowSize = Container->GetSize() + GetSizeOffset() - GetBlurRadius() * 2;

        shadowSize.x = shadowSize.x < 0 ? 0 : shadowSize.x;
        shadowSize.y = shadowSize.y < 0 ? 0 : shadowSize.y;

        {
            ssGUI::DrawingEntity curEntity;
            curEntity.EntityName = BOX_SHADOW_CENTER_SHAPE_NAME;
            curEntity.Vertices.push_back(shadowPos);
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, 0));
            curEntity.Vertices.push_back(shadowPos + shadowSize);
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, shadowSize.y));

            curEntity.Colors.push_back(ShadowColor);
            curEntity.Colors.push_back(ShadowColor);
            curEntity.Colors.push_back(ShadowColor);
            curEntity.Colors.push_back(ShadowColor);

            newEntities.push_back(curEntity);
        }

        auto outerShadowColor = GetShadowColor();
        outerShadowColor.a = 0;

        auto constructCorner = [&](std::string entityName, glm::vec2 shadowVertexPos, glm::vec2 cornerStartOffset, glm::vec2 cornerEndOffset)
        {
            ssGUI::DrawingEntity curEntity;
            curEntity.EntityName = entityName;
            curEntity.Vertices.push_back(shadowVertexPos);
            PlotArc(shadowVertexPos + cornerStartOffset, 
                    shadowVertexPos + cornerEndOffset, 
                    shadowVertexPos, curEntity.Vertices);

            for(int i = 0; i < curEntity.Vertices.size(); i++)
                curEntity.Colors.push_back(outerShadowColor);
            
            curEntity.Colors[0] = GetShadowColor();
            newEntities.push_back(curEntity);
        };

        //Corners
        //Top left corner
        constructCorner(BOX_SHADOW_TOP_LEFT_SHAPE_NAME, shadowPos, glm::vec2(-GetBlurRadius(), 0), glm::vec2(0, -GetBlurRadius()));

        //Top right corner
        constructCorner(BOX_SHADOW_TOP_RIGHT_SHAPE_NAME, shadowPos + glm::vec2(shadowSize.x, 0), glm::vec2(0, -GetBlurRadius()), glm::vec2(GetBlurRadius(), 0));

        //Bottom right corner
        constructCorner(BOX_SHADOW_BOTTOM_RIGHT_SHAPE_NAME, shadowPos + shadowSize, glm::vec2(GetBlurRadius(), 0), glm::vec2(0, GetBlurRadius()));

        //Bottom left corner
        constructCorner(BOX_SHADOW_BOTTOM_LEFT_SHAPE_NAME, shadowPos + glm::vec2(0, shadowSize.y), glm::vec2(0, GetBlurRadius()), glm::vec2(-GetBlurRadius(), 0));
        
        //4 sides
        //Top side
        {
            ssGUI::DrawingEntity curEntity;
            curEntity.EntityName = BOX_SHADOW_TOP_SHAPE_NAME;
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, 0) + glm::vec2(0, -GetBlurRadius()));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, 0) + glm::vec2(0, -GetBlurRadius()));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, 0) + glm::vec2(0, 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, 0) + glm::vec2(0, 0));

            curEntity.Colors.push_back(outerShadowColor);
            curEntity.Colors.push_back(outerShadowColor);
            curEntity.Colors.push_back(GetShadowColor());
            curEntity.Colors.push_back(GetShadowColor());
        
            newEntities.push_back(curEntity);
        }

        //Right side
        {
            ssGUI::DrawingEntity curEntity;
            curEntity.EntityName = BOX_SHADOW_RIGHT_SHAPE_NAME;
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, 0) + glm::vec2(0, 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, 0) + glm::vec2(GetBlurRadius(), 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, shadowSize.y) + glm::vec2(GetBlurRadius(), 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, shadowSize.y) + glm::vec2(0, 0));

            curEntity.Colors.push_back(GetShadowColor());
            curEntity.Colors.push_back(outerShadowColor);
            curEntity.Colors.push_back(outerShadowColor);
            curEntity.Colors.push_back(GetShadowColor());
        
            newEntities.push_back(curEntity);
        }

        //Bottom side
        {
            ssGUI::DrawingEntity curEntity;
            curEntity.EntityName = BOX_SHADOW_BOTTOM_SHAPE_NAME;
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, shadowSize.y) + glm::vec2(0, 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, shadowSize.y) + glm::vec2(0, 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(shadowSize.x, shadowSize.y) + glm::vec2(0, GetBlurRadius()));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, shadowSize.y) + glm::vec2(0, GetBlurRadius()));

            curEntity.Colors.push_back(GetShadowColor());
            curEntity.Colors.push_back(GetShadowColor());
            curEntity.Colors.push_back(outerShadowColor);
            curEntity.Colors.push_back(outerShadowColor);
        
            newEntities.push_back(curEntity);
        }

        //Left side
        {
            ssGUI::DrawingEntity curEntity;
            curEntity.EntityName = BOX_SHADOW_LEFT_SHAPE_NAME;
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, 0) + glm::vec2(-GetBlurRadius(), 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, 0) + glm::vec2(0, 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, shadowSize.y) + glm::vec2(0, 0));
            curEntity.Vertices.push_back(shadowPos + glm::vec2(0, shadowSize.y) + glm::vec2(-GetBlurRadius(), 0));

            curEntity.Colors.push_back(outerShadowColor);
            curEntity.Colors.push_back(GetShadowColor());
            curEntity.Colors.push_back(GetShadowColor());
            curEntity.Colors.push_back(outerShadowColor);
        
            newEntities.push_back(curEntity);
        }

        drawingEntities.insert(drawingEntities.end(), newEntities.begin(), newEntities.end());
    }

    void BoxShadow::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {
        ConstructRenderInfo();
    }

    //Defining the extension name
    const std::string BoxShadow::EXTENSION_NAME = "Box Shadow";

    void BoxShadow::SetPositionOffset(glm::vec2 offset)
    {
        PositionOffset = offset;
    }

    glm::vec2 BoxShadow::GetPositionOffset() const
    {
        return PositionOffset;
    }

    void BoxShadow::SetSizeOffset(glm::vec2 offset)
    {
        SizeOffset = offset;
    }

    glm::vec2 BoxShadow::GetSizeOffset() const
    {
        return SizeOffset;
    }

    void BoxShadow::SetBlurRadius(float radius)
    {
        BlurRadius = radius;
    }

    float BoxShadow::GetBlurRadius() const
    {
        return BlurRadius;
    }

    void BoxShadow::SetShadowColor(glm::u8vec4 color)
    {
        ShadowColor = color;
    }

    glm::u8vec4 BoxShadow::GetShadowColor() const
    {
        return ShadowColor;
    }

    void BoxShadow::SetEnabled(bool enabled)
    {
        Enabled = enabled;

        //Redrawing the object if this affects the rendering
        if(Container != nullptr)
            Container->RedrawObject();
    }

    bool BoxShadow::IsEnabled() const
    {
        return Enabled;
    }

    //Extension methods
    void BoxShadow::Internal_Update(bool isPreUpdate, 
                                    ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow)
    {
        ssGUI_LOG_FUNC();

        if(!Enabled || Container == nullptr)
            return;
    }

    void BoxShadow::Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset)
    {
        ssGUI_LOG_FUNC();

        if(!Enabled || Container == nullptr || !isPreRender)
            return;

        if(Container->IsRedrawNeeded())
            ConstructRenderInfo();
    }

    std::string BoxShadow::GetExtensionName()
    {
        return EXTENSION_NAME;
    }

    void BoxShadow::BindToObject(ssGUI::GUIObject* bindObj)
    {
        Container = bindObj;
    }

    void BoxShadow::Copy(ssGUI::Extensions::Extension* extension)
    {
        if(extension->GetExtensionName() != EXTENSION_NAME)
            return;

        ssGUI::Extensions::BoxShadow* BoxShadow = static_cast<ssGUI::Extensions::BoxShadow*>(extension);

        Enabled = BoxShadow->IsEnabled();
        PositionOffset = BoxShadow->GetPositionOffset();
        SizeOffset = BoxShadow->GetSizeOffset();
        BlurRadius = BoxShadow->GetBlurRadius();
        ShadowColor = BoxShadow->GetShadowColor();
    }

    ObjectsReferences* BoxShadow::Internal_GetObjectsReferences()
    {
        return nullptr;
    }

    BoxShadow* BoxShadow::Clone()
    {
        BoxShadow* temp = new BoxShadow(*this);
        return temp;
    }
}

}
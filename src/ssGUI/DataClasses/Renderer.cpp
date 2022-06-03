#include "ssGUI/DataClasses/Renderer.hpp"

#include "ssGUI/DataClasses/Hierarchy.hpp"
#include "ssGUI/DataClasses/EventCallbackManager.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/EventCallbacks/BackgroundColorChangedEventCallback.hpp"

namespace ssGUI
{
    Renderer::Renderer(Renderer const& other)
    {
        Visible = other.IsSelfVisible(); 
        BackgroundColour = other.GetBackgroundColor();
        Redraw = other.Redraw;
        AcceptRedrawRequest = other.AcceptRedrawRequest;
        DrawingVerticies = other.DrawingVerticies; 
        DrawingUVs = other.DrawingUVs; 
        DrawingColours = other.DrawingColours; 
        DrawingCounts = other.DrawingCounts; 
        DrawingProperties = other.DrawingProperties; 
        GUIObjectShapeIndex = other.GUIObjectShapeIndex; 
        GUIObjectVertexIndex = other.GUIObjectVertexIndex; 
        LastDrawingVerticies = other.LastDrawingVerticies; 
        LastDrawingUVs = other.LastDrawingUVs; 
        LastDrawingColours = other.LastDrawingColours; 
        LastDrawingCounts = other.LastDrawingCounts; 
        LastDrawingProperties = other.LastDrawingProperties;
        CurrentHierarchy = nullptr;
        CurrentEventCallbackManager = nullptr;
        CurrentObject = nullptr;
    }

    void Renderer::ConstructRenderInfo()
    {
        //This should be overriden.
    }

    void Renderer::ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        //This can be overriden if needed.
        ConstructRenderInfo();
    }

    void Renderer::CacheRendering()
    {
        LastDrawingVerticies.assign(DrawingVerticies.begin(), DrawingVerticies.end());
        LastDrawingUVs.assign(DrawingUVs.begin(), DrawingUVs.end());
        LastDrawingColours.assign(DrawingColours.begin(), DrawingColours.end());
        LastDrawingCounts.assign(DrawingCounts.begin(), DrawingCounts.end());
        LastDrawingProperties.assign(DrawingProperties.begin(), DrawingProperties.end());
    }

    void Renderer::DisableRedrawObjectRequest()
    {
        AcceptRedrawRequest = false;
    }

    void Renderer::EnableRedrawObjectRequest()
    {
        AcceptRedrawRequest = true;
    }

    void Renderer::UpdateGUIObjectVertexAndShapeIndex()
    {
        GUIObjectShapeIndex = DrawingCounts.size();
        GUIObjectVertexIndex = DrawingVerticies.size();
    }
    
    Renderer::Renderer() : Visible(true), BackgroundColour(glm::u8vec4(255, 255, 255, 255)), Redraw(true), AcceptRedrawRequest(true),
                            DrawingVerticies(), DrawingUVs(), DrawingColours(), DrawingCounts(), DrawingProperties(), GUIObjectShapeIndex(-1), 
                            GUIObjectVertexIndex(-1), LastDrawingVerticies(), LastDrawingUVs(), LastDrawingColours(), 
                            LastDrawingCounts(), LastDrawingProperties(), CurrentHierarchy(nullptr), CurrentEventCallbackManager(nullptr),
                            CurrentObject(nullptr)
    {}

    Renderer::~Renderer()
    {}

    void Renderer::SetDependentComponents(ssGUI::Hierarchy* hierarchy, ssGUI::EventCallbackManager* eventCallbackManager, ssGUI::GUIObject* obj)
    {
        CurrentHierarchy = hierarchy;
        CurrentEventCallbackManager = eventCallbackManager;
        CurrentObject = obj;
    }

    void Renderer::SetVisible(bool visible)
    {
        Visible = visible;
        RedrawObject();
    }

    bool Renderer::IsVisible() const
    {
        if(!IsSelfVisible())
            return false;
        else
        {
            auto currentParent = CurrentHierarchy->GetParent();
            while (currentParent != nullptr)
            {
                if(!currentParent->IsSelfVisible())
                    return false;

                currentParent = currentParent->GetParent();   
            }
            
            return true;
        }
    }

    bool Renderer::IsSelfVisible() const
    {
        return Visible;
    }

    void Renderer::SetBackgroundColor(glm::u8vec4 color)
    {
        BackgroundColour = color;
        if(CurrentEventCallbackManager->IsAnyEventCallbackExist<ssGUI::EventCallbacks::BackgroundColorChangedEventCallback>())
            CurrentEventCallbackManager->GetAnyEventCallback<ssGUI::EventCallbacks::BackgroundColorChangedEventCallback>()->Notify(CurrentObject);
        RedrawObject();
    }

    glm::u8vec4 Renderer::GetBackgroundColor() const
    {
        //std::cout<<"GetBackgroundColor: "<<BackgroundColour.r<<","<<BackgroundColour.g<<","<<BackgroundColour.b<<","<<BackgroundColour.a<<"\n";
        return BackgroundColour;
    }

    std::vector<glm::vec2>& Renderer::Extension_GetDrawingVertices()
    {
        return DrawingVerticies;
    }

    std::vector<glm::vec2>& Renderer::Extension_GetDrawingUVs()
    {
        return DrawingUVs;
    }

    std::vector<glm::u8vec4>& Renderer::Extension_GetDrawingColours()
    {
        return DrawingColours;
    }

    std::vector<int>& Renderer::Extension_GetDrawingCounts()
    {
        return DrawingCounts;
    }

    std::vector<ssGUI::DrawingProperty>& Renderer::Extension_GetDrawingProperties()
    {
        return DrawingProperties;
    }

    int Renderer::Extension_GetGUIObjectFirstShapeIndex() const
    {
        return GUIObjectShapeIndex;
    }

    int Renderer::Extension_GetGUIObjectFirstVertexIndex() const
    {
        return GUIObjectVertexIndex;
    }

    void Renderer::RedrawObject()
    {
        if(AcceptRedrawRequest)
            Redraw = true;
    }

    bool Renderer::IsRedrawNeeded() const
    {
        return Redraw;
    }
}
#include "ssGUI/DataClasses/Renderer.hpp"

#include "ssGUI/DataClasses/Hierarchy.hpp"
#include "ssGUI/DataClasses/EventCallbackManager.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

namespace ssGUI
{
    Renderer::Renderer(Renderer const& other)
    {
        Enabled = other.IsSelfEnabled(); 
        BackgroundColour = other.GetBackgroundColor();
        Redraw = other.Redraw;
        AcceptRedrawRequest = other.AcceptRedrawRequest;
        DrawingEntities = other.DrawingEntities; 
        GUIObjectShapeIndex = other.GUIObjectShapeIndex; 
        LastDrawingEntities = other.LastDrawingEntities;
        CurrentHierarchy = nullptr;
        CurrentEventCallbackManager = nullptr;
        CurrentObject = nullptr;
    }

    void Renderer::ConstructRenderInfo()
    {
        //This should be overriden.
    }

    void Renderer::ConstructRenderInfo(ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset)
    {
        //This can be overriden if needed.
        ConstructRenderInfo();
    }

    void Renderer::CacheRendering()
    {
        LastDrawingEntities.assign(DrawingEntities.begin(), DrawingEntities.end());
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
        GUIObjectShapeIndex = DrawingEntities.size();
    }
    
    Renderer::Renderer() :  Enabled(true),
                            BackgroundColour(255, 255, 255, 255),
                            Redraw(true),
                            AcceptRedrawRequest(true),
                            DrawingEntities(),
                            GUIObjectShapeIndex(-1),
                            LastDrawingEntities(),
                            CurrentHierarchy(nullptr),
                            CurrentEventCallbackManager(nullptr),
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

    //TODO: It was called SetVisible before so that's why it is in Renderer. Maybe move this to Hierarachy class?
    void Renderer::SetEnabled(bool enabled)
    {
        if(Enabled == enabled)
            return;

        Enabled = enabled;
        RedrawObject();
    }

    bool Renderer::IsEnabled() const
    {
        if(!IsSelfEnabled())
            return false;
        else
        {
            auto currentParent = CurrentHierarchy->GetParent();
            while (currentParent != nullptr)
            {
                if(!currentParent->IsSelfEnabled())
                    return false;

                currentParent = currentParent->GetParent();   
            }
            
            return true;
        }
    }

    bool Renderer::IsSelfEnabled() const
    {
        return Enabled;
    }

    void Renderer::SetBackgroundColor(glm::u8vec4 color)
    {
        if(color == BackgroundColour)
            return;

        BackgroundColour = color;

        if(CurrentEventCallbackManager->IsEventCallbackExist(ssGUI::Enums::EventType::BACKGROUND_COLOR_CHANGED))
            CurrentEventCallbackManager->GetEventCallback(ssGUI::Enums::EventType::BACKGROUND_COLOR_CHANGED)->Notify(CurrentObject);
        
        RedrawObject();
    }

    glm::u8vec4 Renderer::GetBackgroundColor() const
    {
        //std::cout<<"GetBackgroundColor: "<<BackgroundColour.r<<","<<BackgroundColour.g<<","<<BackgroundColour.b<<","<<BackgroundColour.a<<"\n";
        return BackgroundColour;
    }

    std::vector<ssGUI::DrawingEntity>& Renderer::Extension_GetDrawingEntities()
    {
        return DrawingEntities;
    }

    int Renderer::Extension_GetGUIObjectFirstShapeIndex() const
    {
        return GUIObjectShapeIndex;
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
#ifndef SSGUI_RENDERER
#define SSGUI_RENDERER

#include "ssGUI/DataClasses/DrawingProperties.hpp"
#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include <vector>

//namespace: ssGUI
namespace ssGUI
{
    class Hierarchy;
    class EventCallbackManager;
    class GUIObject;
    
    /*class: ssGUI::Renderer
    This manages all the rendering information for a GUI Object

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        bool Visible;
        glm::u8vec4 BackgroundColour;
        bool Redraw;
        bool AcceptRedrawRequest;

        //Rendering
        std::vector<glm::vec2> DrawingVerticies;
        std::vector<glm::vec2> DrawingUVs;
        std::vector<glm::u8vec4> DrawingColours;
        std::vector<int> DrawingCounts;
        std::vector<ssGUI::DrawingProperty> DrawingProperties;
        int GUIObjectShapeIndex;
        int GUIObjectVertexIndex;

        //Cache rendering
        std::vector<glm::vec2> LastDrawingVerticies;
        std::vector<glm::vec2> LastDrawingUVs;
        std::vector<glm::u8vec4> LastDrawingColours;
        std::vector<int> LastDrawingCounts;
        std::vector<ssGUI::DrawingProperty> LastDrawingProperties;

        ssGUI::Hierarchy* CurrentHierarchy;
        ssGUI::EventCallbackManager* CurrentEventCallbackManager;
        ssGUI::GUIObject* CurrentObject;
    =================================================================
    ============================== C++ ==============================
    Renderer::Renderer() : Visible(true), BackgroundColour(glm::u8vec4(255, 255, 255, 255)), Redraw(true), AcceptRedrawRequest(true),
                            DrawingVerticies(), DrawingUVs(), DrawingColours(), DrawingCounts(), DrawingProperties(), GUIObjectShapeIndex(-1), 
                            GUIObjectVertexIndex(-1), LastDrawingVerticies(), LastDrawingUVs(), LastDrawingColours(), 
                            LastDrawingCounts(), LastDrawingProperties(), CurrentHierarchy(nullptr), CurrentEventCallbackManager(nullptr),
                            CurrentObject(nullptr)
    {}
    =================================================================
    */
    class Renderer
    {
        protected:
            bool Visible;
            glm::u8vec4 BackgroundColour;
            bool Redraw;
            bool AcceptRedrawRequest;

            //Rendering
            std::vector<glm::vec2> DrawingVerticies;
            std::vector<glm::vec2> DrawingUVs;
            std::vector<glm::u8vec4> DrawingColours;
            std::vector<int> DrawingCounts;
            std::vector<ssGUI::DrawingProperty> DrawingProperties;
            int GUIObjectShapeIndex;
            int GUIObjectVertexIndex;

            //Cache rendering
            std::vector<glm::vec2> LastDrawingVerticies;
            std::vector<glm::vec2> LastDrawingUVs;
            std::vector<glm::u8vec4> LastDrawingColours;
            std::vector<int> LastDrawingCounts;
            std::vector<ssGUI::DrawingProperty> LastDrawingProperties;

            ssGUI::Hierarchy* CurrentHierarchy;
            ssGUI::EventCallbackManager* CurrentEventCallbackManager;
            ssGUI::GUIObject* CurrentObject;

            Renderer(Renderer const& other);
            virtual void ConstructRenderInfo();
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset);
            virtual void CacheRendering();
            virtual void DisableRedrawObjectRequest();
            virtual void EnableRedrawObjectRequest();
            virtual void UpdateGUIObjectVertexAndShapeIndex();
        
        public:
            Renderer();
            virtual ~Renderer();

            virtual void SetDependentComponents(ssGUI::Hierarchy* hierarchy, ssGUI::EventCallbackManager* eventCallbackManager, ssGUI::GUIObject* obj);
            
            //TODO : Maybe change Visible to Enabled?

            //function: SetVisible
            //Sets the visibility of this GUI Object. Note that the logic of this GUI Object will not be called if not visible.
            virtual void SetVisible(bool visible);
            
            //function: IsVisible
            //Returns the visibility of this GUI Object. Note that the logic of this GUI Object will not be called if not visible.
            virtual bool IsVisible() const;

            //function: IsSelfVisible
            //Returns the visibility of itself excluding the visiblity status of its parent. 
            //Meaning even if the parent with visibility is false, it will return true if this GUI Object's visibility is not set to false.
            virtual bool IsSelfVisible() const;

            //function: SetBackgroundColor
            //Sets the background color of this GUI Object
            virtual void SetBackgroundColor(glm::u8vec4 color);
            
            //function: GetBackgroundColor
            //Gets the background color of this GUI Object
            virtual glm::u8vec4 GetBackgroundColor() const;

            //function: Extension_GetDrawingVertices
            //This returns a list of vertices for all the shapes that will be drawn by this GUI object.
            //A shape is formed by having the vertices drawn in clockwise direction. Undefined behaviour if they are listed in anti-clockwise direction.
            //This function is mainly be called by <Extension::Internal_Draw>.
            virtual std::vector<glm::vec2>& Extension_GetDrawingVertices();
            
            //function: Extension_GetDrawingUVs
            //This returns the UVs that are mapped to each vertex at the same index location.
            //If no texture is used, you should still maintain the number of UVs same as the number of vertices.
            //This function is mainly be called by <Extension::Internal_Draw>.
            virtual std::vector<glm::vec2>& Extension_GetDrawingUVs();
            
            //function: Extension_GetDrawingColours
            //This returns the colors that are mapped to each vertex at the same index location.
            //If texture is used, this essentially affects the tint of the image, with white as no tinting at all.
            //This function is mainly be called by <Extension::Internal_Draw>.
            virtual std::vector<glm::u8vec4>& Extension_GetDrawingColours();
            
            //function: Extension_GetDrawingCounts
            //This returns the number of vertices each shape has, with the order same as vertices and others.
            //So for example if the first value is 4, then the first 4 vertices form a shape. So on and so forth...
            //This function is mainly be called by <Extension::Internal_Draw>.
            virtual std::vector<int>& Extension_GetDrawingCounts();
            
            //function: Extension_GetDrawingProperties
            //This returns the property of each shape, mapped to the same index location as <Extension_GetDrawingCounts>.
            //This indicates if the shape is just a colored shape, an image or font. 
            //This function is mainly be called by <Extension::Internal_Draw>.
            virtual std::vector<ssGUI::DrawingProperty>& Extension_GetDrawingProperties();

            /*function: Extension_GetGUIObjectFirstShapeIndex
            This returns the index of first shape created by the GUI Object.
            So for example, 
            ================ Text ====================
            [Extension created Shape A] [Extension created Shape B] [GUI Object created Shape C] [Extension created Shape D]
             rendered first ────────────────────────────────────────────────────────────────────────────────────► rendered last
            ==========================================
            This will return shape C as this is the first shape created by GUI object.
            So to get shape A, you will need to -2 from this index.
            Note that this doesn't check if the GUI Object creates any shapes, 
            it just returns the index *if* the GUI object creates any shapes.
            So it will be invalid to use this index on any empty GUI objects that does not create any shapes.*/
            virtual int Extension_GetGUIObjectFirstShapeIndex() const;

            //function: Extension_GetGUIObjectFirstVertexIndex
            //This returns the index of first vertex created by the GUI Object
            //Again similar to <Extension_GetGUIObjectFirstShapeIndex> but with indices instead
            virtual int Extension_GetGUIObjectFirstVertexIndex() const;

            //function: RedrawObject
            //Forces the GUI Object to be redrawn. *Do not* call this in <Internal_Draw>.
            virtual void RedrawObject();

            //function: IsRedrawNeeded
            //Returns if the GUI Object needs to be redrawn
            virtual bool IsRedrawNeeded() const;
    };
}

#endif
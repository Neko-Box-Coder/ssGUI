#ifndef SSGUI_RENDERER_H
#define SSGUI_RENDERER_H

#include "ssGUI/DataClasses/DrawingEntity.hpp"
#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"
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
        bool Enabled;                                                   //See <IsEnabled>
        glm::u8vec4 BackgroundColour;                                   //See <GetBackgroundColor>
        bool Redraw;                                                    //See <IsRedrawNeeded>
        bool AcceptRedrawRequest;                                       //(Internal variable) Used to disable self-calling when calling draw logic

        //Rendering
        std::vector<glm::vec2> DrawingVerticies;                        //See <Extension_GetDrawingVertices>
        std::vector<glm::vec2> DrawingUVs;                              //See <Extension_GetDrawingUVs>
        std::vector<glm::u8vec4> DrawingColours;                        //See <Extension_GetDrawingColours>
        std::vector<int> DrawingCounts;                                 //See <Extension_GetDrawingCounts>
        std::vector<ssGUI::DrawingProperty> DrawingProperties;          //See <Extension_GetDrawingProperties>
        int GUIObjectShapeIndex;                                        //See <Extension_GetGUIObjectFirstShapeIndex>
        int GUIObjectVertexIndex;                                       //See <Extension_GetGUIObjectFirstVertexIndex>

        //Cache rendering
        std::vector<glm::vec2> LastDrawingVerticies;                    //(Internal variable) Used to do cached rendering
        std::vector<glm::vec2> LastDrawingUVs;                          //(Internal variable) Used to do cached rendering
        std::vector<glm::u8vec4> LastDrawingColours;                    //(Internal variable) Used to do cached rendering
        std::vector<int> LastDrawingCounts;                             //(Internal variable) Used to do cached rendering
        std::vector<ssGUI::DrawingProperty> LastDrawingProperties;      //(Internal variable) Used to do cached rendering

        ssGUI::Hierarchy* CurrentHierarchy;                             //(Internal variable) Used to get parent when <IsEnabled> is called
        ssGUI::EventCallbackManager* CurrentEventCallbackManager;       //(Internal variable) Used to call event callbacks listeners
        ssGUI::GUIObject* CurrentObject;                                //(Internal variable) Used to register the container for event callbacks listeners 
    =================================================================
    ============================== C++ ==============================
    Renderer::Renderer() :  Enabled(true),
                            BackgroundColour(255, 255, 255, 255),
                            Redraw(true),
                            AcceptRedrawRequest(true),
                            DrawingVerticies(),
                            DrawingUVs(),
                            DrawingColours(),
                            DrawingCounts(),
                            DrawingProperties(),
                            GUIObjectShapeIndex(-1),
                            GUIObjectVertexIndex(-1),
                            LastDrawingVerticies(),
                            LastDrawingUVs(),
                            LastDrawingColours(),
                            LastDrawingCounts(),
                            LastDrawingProperties(),
                            CurrentHierarchy(nullptr),
                            CurrentEventCallbackManager(nullptr),
                            CurrentObject(nullptr)
    {}
    =================================================================
    */
    class Renderer
    {
        protected:
            bool Enabled;                                                   //See <IsEnabled>
            glm::u8vec4 BackgroundColour;                                   //See <GetBackgroundColor>
            bool Redraw;                                                    //See <IsRedrawNeeded>
            bool AcceptRedrawRequest;                                       //(Internal variable) Used to disable self-calling when calling draw logic

            //Rendering
            std::vector<ssGUI::DrawingEntity> DrawingEntities;              //See <Extension_GetDrawingEntities>
            int GUIObjectShapeIndex;                                        //See <Extension_GetGUIObjectFirstShapeIndex>

            //Cache rendering
            std::vector<ssGUI::DrawingEntity> LastDrawingEntities;          //(Internal variable) Used to do cached rendering

            ssGUI::Hierarchy* CurrentHierarchy;                             //(Internal variable) Used to get parent when <IsEnabled> is called
            ssGUI::EventCallbackManager* CurrentEventCallbackManager;       //(Internal variable) Used to call event callbacks listeners
            ssGUI::GUIObject* CurrentObject;                                //(Internal variable) Used to register the container for event callbacks listeners 

            Renderer(Renderer const& other);
            virtual void ConstructRenderInfo();
            virtual void ConstructRenderInfo(ssGUI::Backend::DrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset);
            virtual void CacheRendering();
            virtual void DisableRedrawObjectRequest();
            virtual void EnableRedrawObjectRequest();
            virtual void UpdateGUIObjectVertexAndShapeIndex();
        
        public:
            Renderer();
            virtual ~Renderer();

            virtual void SetDependentComponents(ssGUI::Hierarchy* hierarchy, ssGUI::EventCallbackManager* eventCallbackManager, ssGUI::GUIObject* obj);
            
            //function: SetEnabled
            //Sets if this GUI Object is enabled. Note that the logic of this GUI Object will not be called if not enabled.
            virtual void SetEnabled(bool enabled);
            
            //function: IsEnabled
            //Returns if this GUI Object is enabled. Note that the logic of this GUI Object will not be called if not enabled.
            //Also Note that this can be affected by parent's enable status.
            virtual bool IsEnabled() const;

            //function: IsSelfEnabled
            //Returns if itself is enabled excluding the enable status of its parent. 
            //Meaning even if the parent with visibility is false, it will return true if this GUI Object's enable is not set to false.
            virtual bool IsSelfEnabled() const;

            //function: SetBackgroundColor
            //Sets the background color of this GUI Object
            virtual void SetBackgroundColor(glm::u8vec4 color);
            
            //function: GetBackgroundColor
            //Gets the background color of this GUI Object
            virtual glm::u8vec4 GetBackgroundColor() const;

            //function: Extension_GetDrawingEntities
            //This returns the property of each shape, mapped to the same index location as <Extension_GetDrawingCounts>.
            //This indicates if the shape is just a colored shape, an image or font. 
            //This function is mainly be called by <ssGUI::Extensions::Extension::Internal_Draw>.
            virtual std::vector<ssGUI::DrawingEntity>& Extension_GetDrawingEntities();

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

            //function: RedrawObject
            //Forces the GUI Object to be redrawn. *Do not* call this in <Internal_Draw>.
            virtual void RedrawObject();

            //function: IsRedrawNeeded
            //Returns if the GUI Object needs to be redrawn
            virtual bool IsRedrawNeeded() const;
    };
}

#endif
#ifndef SSGUI_CUSTOM_EXTENSION_H
#define SSGUI_CUSTOM_EXTENSION_H

#include <unordered_set>
#include "ssGUI/Extensions/Extension.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"

namespace ssGUI 
{ 
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::Shape
    Shape extension allows adding/removing shapes in runtime on the GUI Object without too much effort. This would not work with <ssGUI::MainWindow>

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        struct AdditionalShape                                  //(Internal structure) Used to store the additional shape apply on the GUI Object
        {
            std::vector<glm::vec2> Vertices;
            std::vector<glm::u8vec4> Colors;
            int ID;
            bool BehindGUI;
        };

        ssGUI::GUIObject* Container;                        //See <BindToObject>
        bool Enabled;                                       //See <IsEnabled>

        bool ExtensionPreRender;                            //(Internal variable) Used to track if the current drawing is before or after the GUI Object
        std::vector<AdditionalShape> AdditionalShapes;      //See <AddAdditionalPolygon>
        std::unordered_set<int> GUIObjectShapesToRemove;    //See <RemoveGUIObjectShape>
        int NextID;                                         //(Internal variable) Used to return unique shape ID
    =================================================================
    ============================== C++ ==============================
    Shape::Shape() :    Container(nullptr),
                        Enabled(true),
                        ExtensionPreRender(true),
                        AdditionalShapes(),
                        GUIObjectShapesToRemove(),
                        NextID(0)
    {}
    =================================================================
    */
    class Shape : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            Shape& operator=(Shape const& other);

        protected:
            struct AdditionalShape                                  //(Internal structure) Used to store the additional shape apply on the GUI Object
            {
                std::vector<glm::vec2> Vertices;
                std::vector<glm::u8vec4> Colors;
                int ID;
                bool BehindGUI;
            };

            ssGUI::GUIObject* Container;                        //See <BindToObject>
            bool Enabled;                                       //See <IsEnabled>

            bool ExtensionPreRender;                            //(Internal variable) Used to track if the current drawing is before or after the GUI Object
            std::vector<AdditionalShape> AdditionalShapes;      //See <AddAdditionalPolygon>
            std::unordered_set<int> GUIObjectShapesToRemove;    //See <RemoveGUIObjectShape>
            int NextID;                                         //(Internal variable) Used to return unique shape ID

            Shape();
            virtual ~Shape() override;
            Shape(Shape const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            double pi() const { return std::atan(1)*4; };

            virtual void ConstructAdditionalPolygon(AdditionalShape& targetShape, std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject);
            virtual void ConstructAdditionalRectangle(AdditionalShape& targetShape, glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject);
            virtual void ConstructAdditionalCircle(AdditionalShape& targetShape, glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject);
            virtual void ConstructAdditionalLine(AdditionalShape& targetShape, glm::vec2 start, glm::vec2 end, float startThickness, float endThickness, glm::u8vec4 startColor, glm::u8vec4 endColor, bool behindGUIObject);

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
        
        public:
            static const std::string EXTENSION_NAME;

            /*function: AddAdditionalPolygon
            Adding additional polygon in addition to GUI Object. 
            The additional shape will be appended in the list of additional shapes stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalPolygon(std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject);
            
            /*function: AddAdditionalPolygon
            Adding additional polygon in addition to GUI Object. 
            The additional shape will be added in the list of additional shapes at index position stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalPolygon(std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject, int index);

            /*function: AddAdditionalPolygon
            Adding additional empty polygon in addition to GUI Object. 
            The additional shape will be appended in the list of additional shapes stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalPolygon();

            /*function: AddAdditionalRectangle
            Adding additional rectangle in addition to GUI Object. 
            The additional shape will be appended in the list of additional shapes stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalRectangle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject);
            
            /*function: AddAdditionalRectangle
            Adding additional rectangle in addition to GUI Object. 
            The additional shape will be added in the list of additional shapes at index position stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalRectangle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject, int index);

            /*function: AddAdditionalRectangle
            Adding additional empty rectangle in addition to GUI Object. 
            The additional shape will be appended in the list of additional shapes stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalRectangle();

            /*function: AddAdditionalCircle
            Adding additional circle in addition to GUI Object. 
            The additional shape will be appended in the list of additional shapes stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalCircle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject);
            
            /*function: AddAdditionalCircle
            Adding additional circle in addition to GUI Object.
            The additional shape will be added in the list of additional shapes at index position stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalCircle(glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject, int index);

            /*function: AddAdditionalCircle
            Adding additional empty circle in addition to GUI Object. 
            The additional shape will be appended in the list of additional shapes stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalCircle();

            /*function: AddAdditionalLine
            Adding additional line in addition to GUI Object. 
            The additional line will be appended in the list of additional shapes stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalLine(glm::vec2 start, glm::vec2 end, float startThickness, float endThickness, glm::u8vec4 startColor, glm::u8vec4 endColor, bool behindGUIObject);
            
            /*function: AddAdditionalLine
            Adding additional line in addition to GUI Object.
            The additional line will be added in the list of additional shapes at index position stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalLine(glm::vec2 start, glm::vec2 end, float startThickness, float endThickness, glm::u8vec4 startColor, glm::u8vec4 endColor, bool behindGUIObject, int index);

            /*function: AddAdditionalLine
            Adding additional empty line in addition to GUI Object. 
            The additional line will be appended in the list of additional shapes stored in this extension.
            This returns an ID for uniquely identifying the shape just added within this extension for getting, setting and removing it.*/
            virtual int AddAdditionalLine();

            /*function: SetAdditionalPolygon
            This sets the additional shape with target id. The additional shape with target id is not limited to the same type,
            meaning rectangle or circle additional shape can be set to polygon using this function.*/
            virtual void SetAdditionalPolygon(int id, std::vector<glm::vec2>const & vertices, std::vector<glm::u8vec4>const & colors, bool behindGUIObject);
            
            /*function: SetAdditionalRectangle
            This sets the additional shape with target id. The additional shape with target id is not limited to the same type,
            meaning polygon or circle additional shape can be set to rectangle using this function.*/
            virtual void SetAdditionalRectangle(int id, glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject);
            
            /*function: SetAdditionalCircle
            This sets the additional shape with target id. The additional shape with target id is not limited to the same type,
            meaning polygon or rectangle additional shape can be set to circle using this function.*/
            virtual void SetAdditionalCircle(int id, glm::vec2 pos, glm::vec2 size, glm::u8vec4 color, bool behindGUIObject);

            /*function: SetAdditionalLine
            This sets the additional shape with target id. The additional shape with target id is not limited to the same type,
            meaning other additional shape can be set to circle using this function.*/
            virtual void SetAdditionalLine(int id, glm::vec2 start, glm::vec2 end, float startThickness, float endThickness, glm::u8vec4 startColor, glm::u8vec4 endColor, bool behindGUIObject);

            /*function: GetAdditionalShapeVerticesWithIndex
            This returns a pointer to the target shape vertices vector that is at target index stored in this extension.
            This should not be used for a long period of time as the vector can be reallocated somewhere else.
            Also remember to redraw the GUI Object if changing any vertices.
            Nullptr can be returned if index is invalid. */
            virtual std::vector<glm::vec2>* GetAdditionalShapeVerticesWithIndex(int index);

            /*function: GetAdditionalShapeVerticesWithID
            This returns a pointer to the target shape vertices vector with target id in this extension.
            This should not be used for a long period of time as the vector can be reallocated somewhere else.
            Also remember to redraw the GUI Object if changing any vertices.
            Nullptr can be returned if the id is invalid. */
            virtual std::vector<glm::vec2>* GetAdditionalShapeVerticesWithID(int id);

            /*function: GetAdditionalShapeColorsWithIndex
            This returns a pointer to the target shape colors vector that is at target index stored in this extension.
            This should not be used for a long period of time as the vector can be reallocated somewhere else.
            Also remember to redraw the GUI Object if changing any vertices colors.
            Nullptr can be returned if index is invalid. */
            virtual std::vector<glm::u8vec4>* GetAdditionalShapeColorsWithIndex(int index);
            
            /*function: GetAdditionalShapeColorsWithID
            This returns a pointer to the target shape colors vector with target id in this extension.
            This should not be used for a long period of time as the vector can be reallocated somewhere else.
            Also remember to redraw the GUI Object if changing any vertices colors.
            Nullptr can be returned if the id is invalid. */
            virtual std::vector<glm::u8vec4>* GetAdditionalShapeColorsWithID(int id);

            //function: GetAdditionalShapesCount
            //This returns the total number of additional shapes
            virtual int GetAdditionalShapesCount() const;

            //function: ClearAllAdditionalShapes
            //This removes all the additional shapes
            virtual void ClearAllAdditionalShapes();

            //function: RemoveAdditionalShapeWithIndex
            //This removes the additional shape at target index stored in this extension
            virtual void RemoveAdditionalShapeWithIndex(int index);

            //function: RemoveAdditionalShapeWithID
            //This removes the additional shape with target id stored in this extension
            virtual void RemoveAdditionalShapeWithID(int id);

            //function: RemoveGUIObjectShape
            //This removes the shape relative to the GUI Object shape with index
            virtual void RemoveGUIObjectShape(int index);

            //function: RestoreGUIObjectShape
            //This restores the shape removed by <RemoveGUIObjectShape> relative to the GUI Object shape with index
            virtual void RestoreGUIObjectShape(int index);

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow) override;
            
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
            virtual Shape* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

}

#endif
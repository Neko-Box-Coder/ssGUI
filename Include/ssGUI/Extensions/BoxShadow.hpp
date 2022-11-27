#ifndef SSGUI_BOX_SHADOW_H
#define SSGUI_BOX_SHADOW_H

#include "ssGUI/Extensions/Extension.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "glm/geometric.hpp"

namespace ssGUI 
{ 
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::BoxShadow
    Creates a box shadow beneath the GUI Object. The box shadow follows the position and size of the GUI Object.
    So if you set PositionOffset and size offset to zero, the box shadow will be completely behind the GUI Object.
    You should also set the min size of the GUI Object to be larger than blur radius, 
    otherwise the extension will draw the box shadow larger than the GUI Object.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;    //See <BindToObject>
        bool Enabled;                   //See <IsEnabled>

        glm::vec2 PositionOffset;       //See <GetPositionOffset>
        glm::vec2 SizeOffset;           //See <GetSizeOffset>
        float BlurRadius;               //See <GetBlurRadius>
        glm::u8vec4 ShadowColor;        //See <GetShadowColor>
    =================================================================
    ============================== C++ ==============================
    BoxShadow::BoxShadow() :    Container(nullptr),
                                Enabled(true),
                                PositionOffset(0, 0),
                                SizeOffset(5, 5),
                                BlurRadius(10),
                                ShadowColor(0, 0, 0, 127)
    {}
    =================================================================
    */
    class BoxShadow : public Extension
    {
        public:
            friend class ssGUI::Factory;
        
        private:
            BoxShadow& operator=(BoxShadow const& other);

        protected:
            ssGUI::GUIObject* Container;    //See <BindToObject>
            bool Enabled;                   //See <IsEnabled>

            glm::vec2 PositionOffset;       //See <GetPositionOffset>
            glm::vec2 SizeOffset;           //See <GetSizeOffset>
            float BlurRadius;               //See <GetBlurRadius>
            glm::u8vec4 ShadowColor;        //See <GetShadowColor>

            BoxShadow();
            virtual ~BoxShadow() override;
            BoxShadow(BoxShadow const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            //https://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
            double pi() const { return std::atan(1)*4; };

            //Return angle in radians. Positive if angle between a and b is clockwise
            virtual double GetAngle(glm::vec2 a, glm::vec2 b);

            virtual void PlotArc(glm::vec2 start, glm::vec2 end, glm::vec2 circlePos, std::vector<glm::vec2>& plottedPoints);

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

        public:
            static const std::string EXTENSION_NAME;


            //function: SetPositionOffset
            //Sets the position offset of the box shadow from the GUI Object position
            virtual void SetPositionOffset(glm::vec2 offset);

            //function: GetPositionOffset
            //Returns the position offset of the box shadow from the GUI Object position
            virtual glm::vec2 GetPositionOffset() const;

            //function: SetSizeOffset
            //Sets the size offset of the box shadow from the GUI Object size
            virtual void SetSizeOffset(glm::vec2 offset);

            //function: GetSizeOffset
            //Returns the size offset of the box shadow from the GUI Object size
            virtual glm::vec2 GetSizeOffset() const;

            //function: SetBlurRadius
            //Sets the blur radius of box shadow. Note that this extension don't actually "blur" anything, it is just using gradients. 
            virtual void SetBlurRadius(float radius);

            //function: GetBlurRadius
            //Returns the blur radius of box shadow. Note that this extension don't actually "blur" anything, it is just using gradients. 
            virtual float GetBlurRadius() const;

            //function: SetShadowColor
            //Sets the shadow color of box shadow
            virtual void SetShadowColor(glm::u8vec4 color);

            //function: GetShadowColor
            //Returns the shadow color of box shadow
            virtual glm::u8vec4 GetShadowColor() const;

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
            virtual BoxShadow* Clone(ssGUI::GUIObject* newContainer) override;
    };
}

}

#endif
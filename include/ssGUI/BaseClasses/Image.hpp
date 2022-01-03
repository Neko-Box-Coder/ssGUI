#ifndef SSGUI_IMAGE
#define SSGUI_IMAGE

#include "ssGUI/BaseClasses/Widget.hpp"
#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Enums/ImageFitting.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "SFML/Graphics.hpp"
#include "glm/vec4.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //class: Image
    class Image : public Widget
    {
        private:
            ssGUI::Backend::BackendImageInterface* BackendImage;
            ssGUI::Enums::ImageFitting Fitting;
            Image& operator=(Image const& other);
        
        protected:
            Image(Image const& other);

        public:
            Image();
            virtual ~Image() override;

            //function: GetBackendImageInterface
            virtual ssGUI::Backend::BackendImageInterface* GetBackendImageInterface();

            //function: GetFitting
            virtual ssGUI::Enums::ImageFitting GetFitting() const;

            //function: SetFitting
            virtual void SetFitting(ssGUI::Enums::ImageFitting fitting);

            //Overriding widget
            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Delete 
            virtual void Delete() override;

            //function: Draw
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;

            //virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface& inputInterface, bool& blockAllInput, bool& blockInputInWindow) override;

            //function: Clone
            virtual GUIObject* Clone(bool cloneChildren) override;

    };
}


#endif
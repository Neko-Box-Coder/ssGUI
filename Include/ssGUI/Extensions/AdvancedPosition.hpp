#ifndef SSGUI_SSGUI_ADVANCED_POSITION_H
#define SSGUI_SSGUI_ADVANCED_POSITION_H

#include <unordered_map>
#include "ssGUI/Enums/AlignmentHorizontal.hpp"
#include "ssGUI/Enums/AlignmentVertical.hpp"
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject

#include "ssGUI/GUIObjectClasses/MainWindow.hpp"

/*
#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"*/

namespace ssGUI 
{ 
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::AdvancedPosition
    This extension allows extra options for positioning a GUI Object
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;                            //See <BindToObject>
        bool Enabled;                                           //See <IsEnabled>

        ssGUI::Enums::AlignmentHorizontal CurrentHorizontal;    //See <GetHorizontalAnchor>
        ssGUI::Enums::AlignmentVertical CurrentVertical;        //See <GetVerticalAnchor>

        float HorizontalPixelValue;                             //See <GetHorizontalPixel>
        float VerticalPixelValue;                               //See <GetVerticalPixel>

        float HorizontalPercentageValue;                        //See <GetHorizontalPercentage>
        float VerticalPercentageValue;                          //See <GetVerticalPercentage>
    =================================================================
    ============================== C++ ==============================
    AdvancedPosition::AdvancedPosition() :  Container(nullptr),
                                            Enabled(true),
                                            CurrentHorizontal(ssGUI::Enums::AlignmentHorizontal::CENTER),
                                            CurrentVertical(ssGUI::Enums::AlignmentVertical::CENTER),
                                            HorizontalPixelValue(0),
                                            VerticalPixelValue(0),
                                            HorizontalPercentageValue(0),
                                            VerticalPercentageValue(0)
    {}
    =================================================================
    */
    class AdvancedPosition : public Extension
    {       
        //Forward declaration
        public:
            friend class ssGUI::Factory;

        private:
            AdvancedPosition& operator=(AdvancedPosition const& other);
        
        protected:
            ssGUI::GUIObject* Container;                            //See <BindToObject>
            bool Enabled;                                           //See <IsEnabled>

            ssGUI::Enums::AlignmentHorizontal CurrentHorizontal;    //See <GetHorizontalAnchor>
            ssGUI::Enums::AlignmentVertical CurrentVertical;        //See <GetVerticalAnchor>

            float HorizontalPixelValue;                             //See <GetHorizontalPixel>
            float VerticalPixelValue;                               //See <GetVerticalPixel>

            float HorizontalPercentageValue;                        //See <GetHorizontalPercentage>
            float VerticalPercentageValue;                          //See <GetVerticalPercentage>

            AdvancedPosition();
            virtual ~AdvancedPosition() override;
            AdvancedPosition(AdvancedPosition const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

        public:
            static const std::string EXTENSION_NAME;

            //function: SetHorizontalAlignment
            virtual void SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal align);

            //function: GetHorizontalAlignment
            virtual ssGUI::Enums::AlignmentHorizontal GetHorizontalAlignment() const;

            //function: SetVerticalAlignment
            virtual void SetVerticalAlignment(ssGUI::Enums::AlignmentVertical align);

            //function: GetVerticalAlignment
            virtual ssGUI::Enums::AlignmentVertical GetVerticalAlignment() const;
            
            //function: SetAlignment
            virtual void SetAlignment(ssGUI::Enums::AlignmentHorizontal hori, ssGUI::Enums::AlignmentVertical vert);

            //function: SetHorizontalPixel
            virtual void SetHorizontalPixel(float pixel);

            //function: GetHorizontalPixel            
            virtual float GetHorizontalPixel() const;

            //function: SetVerticalPixel
            virtual void SetVerticalPixel(float pixel);

            //function: GetVerticalPixel            
            virtual float GetVerticalPixel() const;
            
            //function: SetAlignmentPixel
            virtual void SetAlignmentPixel(float hori, float vert);
            
            //function:  SetHorizontalPercentage           
            //0 = 0%, 1 = 100%. This can go below 0 or over 1
            virtual void SetHorizontalPercentage(float percentage);

            //function: GetHorizontalPercentage          
            //0 = 0%, 1 = 100%. This can go below 0 or over 1
            virtual float GetHorizontalPercentage() const;

            //function: SetVerticalPercentage
            //0 = 0%, 1 = 100%. This can go below 0 or over 1
            virtual void SetVerticalPercentage(float percentage);

            //function: GetVerticalPercentage          
            //0 = 0%, 1 = 100%. This can go below 0 or over 1
            virtual float GetVerticalPercentage() const;

            //function: SetAlignmentPercentage
            virtual void SetAlignmentPercentage(float hori, float vert);

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(   bool isPreUpdate, 
                                            ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() const override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(const ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            //See <Extension::Clone>
            virtual AdvancedPosition* Clone() override;
    };
}

}



#endif
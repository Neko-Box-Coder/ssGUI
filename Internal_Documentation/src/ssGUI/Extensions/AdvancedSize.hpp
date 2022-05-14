#ifndef SSGUI_ADVANCED_SIZE
#define SSGUI_ADVANCED_SIZE

#include <unordered_map>
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject

#include "ssGUI/GUIObjectClasses/MainWindow.hpp"

/*
#include "ssGUI/EventCallbacks/RecursiveChildrenAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildrenRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"*/

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: ssGUI::Extensions::AdvancedSize
    This extension allows extra option for sizing a GUI Object
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::GUIObject* Container;
        bool Enabled;

        bool HorizontalUsePercentage;
        bool VerticalUsePercentage;

        int HorizontalPixelValue;
        int VerticalPixelValue;

        float HorizontalPercentageValue;
        float VerticalPercentageValue;

        bool OverrideDefaultSize;
    =================================================================
    ============================== C++ ==============================
    AdvancedSize::AdvancedSize() : Container(nullptr), Enabled(true), HorizontalUsePercentage(true), VerticalUsePercentage(true), 
                                    HorizontalPixelValue(50), VerticalPixelValue(50), HorizontalPercentageValue(0.2), 
                                    VerticalPercentageValue(0.2), OverrideDefaultSize(true)
    {}
    =================================================================
    */
    class AdvancedSize : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            AdvancedSize& operator=(AdvancedSize const& other);
        
        protected:
            ssGUI::GUIObject* Container;
            bool Enabled;

            bool HorizontalUsePercentage;
            bool VerticalUsePercentage;

            int HorizontalPixelValue;
            int VerticalPixelValue;

            float HorizontalPercentageValue;
            float VerticalPercentageValue;

            bool OverrideDefaultSize;

            AdvancedSize();
            virtual ~AdvancedSize() override;
            AdvancedSize(AdvancedSize const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;

        public:
            static const std::string EXTENSION_NAME;

            //function: SetHorizontalUsePercentage
            virtual void SetHorizontalUsePercentage(bool percentage);

            //function: IsHorizontalUsePercentage
            virtual bool IsHorizontalUsePercentage() const;

            //function: SetVerticalUsePercentage
            virtual void SetVerticalUsePercentage(bool percentage);

            //function: IsVerticalUsePercentage
            virtual bool IsVerticalUsePercentage() const;

            //function: SetHorizontalPixel
            virtual void SetHorizontalPixel(int pixel);

            //function: GetHorizontalPixel            
            virtual int GetHorizontalPixel() const;

            //function: SetVerticalPixel
            virtual void SetVerticalPixel(int pixel);

            //function: GetVerticalPixel            
            virtual int GetVerticalPixel() const;
            
            //function: SetHorizontalPercentage           
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

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;

            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;
            
            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;;
            
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
            virtual AdvancedSize* Clone(ssGUI::GUIObject* newContainer) override;
    };
}




#endif
#ifndef SSGUI_TABBABLE_HPP
#define SSGUI_TABBABLE_HPP

#include "ssGUI/Extensions/Extension.hpp"

namespace ssGUI 
{
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    /* clang-format off */
    
    /*class: ssGUI::Extensions::Tabbable
    Allows the attached object to be tabbed under <ssGUI::Extensions::TabArea>
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        //See <BindToObject>
        ssGUI::GUIObject* Container;
        
        //See <IsEnabled>
        bool Enabled;

        //See <GetTabAreaObject>
        ssGUIObjectIndex TabAreaObject;
        
        //See <Internal_GetObjectsReferences>
        ObjectsReferences CurrentObjectsReferences;
        
        //See <GetTopLevelParent>
        ssGUIObjectIndex TopLevelParent;
        
        //See <IsUntabbable>
        bool Untabbable;
        
        //See <GetLastMouseDragBeginPosition>
        glm::vec2 LastMouseDragBeginPosition;
    =================================================================
    ============================== C++ ==============================
    Tabbable::Tabbable() :
        Container(nullptr),
        Enabled(true),
        TabAreaObject(-1),
        CurrentObjectsReferences(),
        TopLevelParent(-1),
        Untabbable(false),
        LastMouseDragBeginPosition()
    {
    }
    =================================================================
    */
    
    /* clang-format on */
    class Tabbable : public Extension
    {
        public:
            friend class ssGUI::Factory;

        private:
            Tabbable& operator=(Tabbable const& other);
        
        protected:
            //See <BindToObject>
            ssGUI::GUIObject* Container;
            
            //See <IsEnabled>
            bool Enabled;

            //See <GetTabAreaObject>
            ssGUIObjectIndex TabAreaObject;
            
            //See <Internal_GetObjectsReferences>
            ObjectsReferences CurrentObjectsReferences;
            
            //See <GetTopLevelParent>
            ssGUIObjectIndex TopLevelParent;
            
            //See <IsUntabbable>
            bool Untabbable;
            
            //See <GetLastMouseDragBeginPosition>
            glm::vec2 LastMouseDragBeginPosition;

            Tabbable();
            virtual ~Tabbable() override;
            Tabbable(Tabbable const& other);
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(   ssGUI::Backend::DrawingInterface* drawingInterface, 
                                                ssGUI::GUIObject* mainWindow, 
                                                glm::vec2 mainWindowPositionOffset) override;

        public:
            //====================================================================
            //Group: Constants
            //====================================================================

            //string: EXTENSION_NAME
            static const std::string EXTENSION_NAME;

            //====================================================================
            //Group: Tabbable
            //====================================================================

            //function: SetTopLevelParent
            virtual void SetTopLevelParent(ssGUI::GUIObject* topLevelParent);
            
            //function: GetTopLevelParent
            virtual ssGUI::GUIObject* GetTopLevelParent() const;
            
            //function: SetUntabbable
            virtual void SetUntabbable(bool untabbable);
            
            //function: IsUntabbable
            virtual bool IsUntabbable() const;

            //function: GetLastMouseDragBeginPosition
            virtual glm::vec2 GetLastMouseDragBeginPosition() const;
            
            //function: SetLastMouseDragBeginPosition
            virtual void SetLastMouseDragBeginPosition(glm::vec2 pos);

            //function: GetTabAreaObject
            ssGUI::GUIObject* GetTabAreaObject() const;
            
            //function: SetTabAreaObject
            void SetTabAreaObject(ssGUI::GUIObject* tabArea);


            //====================================================================
            //Group: Overrides
            //====================================================================

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
                                            ssGUI::Backend::SystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw( bool isPreRender, 
                                        ssGUI::Backend::DrawingInterface* drawingInterface, 
                                        ssGUI::GUIObject* mainWindow, 
                                        glm::vec2 mainWindowPositionOffset) override;
            
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
            virtual Tabbable* Clone() override;
    };
}

}

#endif